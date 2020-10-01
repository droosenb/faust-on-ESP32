/************************** BEGIN BelaOSCUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __Esp32OSCUI__
#define __Esp32OSCUI__

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <map>

//temporary includes for testing. remove before publishing

#include "APIUI.h"
#include "GUI.h"

//remove comment to setup up proper ui includes

//#include "faust/gui/APIUI.h"

//includes from esp32
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "oscpkt.hh"  //independ osc parsing library
#include <vector>

#define PORT 4000       //port the ESP32 is listening to
#define OUTPORT 5000    //port the ESP32 will send data to

#define HOST_IP_ADDR  INADDR_ANY //no initial address for the host. will get later. 

#define CONFIG_EXAMPLE_IPV4 true

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      "Obiehub_North"
#define EXAMPLE_ESP_WIFI_PASS      "badpassword"
#define EXAMPLE_ESP_MAXIMUM_RETRY  5

static const char *TAG = "OSC Device"; //the tag for ESP_LOG



/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1



/**************************BEGIN OSC SECTION **************************/

class oscItem : public uiItem {
    
    protected:
        
        //OscSender* fSender;
        std::string fPath;
        
    public:
        
        oscItem(GUI* ui, const std::string& path, FAUSTFLOAT* zone)
        :uiItem(ui, zone), fPath(path) {}
        virtual ~oscItem()
        {}
    
        // // TO TEST
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            //fSender->newMessage(fPath).add(v).send();
            printf("%s %f", fPath, v);
        }
    
};

static const char *TAG = "OSC Device"; //the tag for ESP_LOG
static int s_retry_num = 0;

class Esp32OSCUI : public GUI {
    
    private:
    
        std::string fIP;
        int fInputPort;
        int fOutputPort;
        APIUI fAPIUI;
    
        std::vector<oscItem*> fOSCItems; // Pointers are kept and desallocated by the GUI class
        //OscReceiver fReceiver;
        //OscSender fSender;
        static char server_ip [16]; 

    public:

        /*
        void oscMessageReceivedAux(oscpkt::Message* msg)
        {
            std::string msgAdress = msg->addressPattern();
            int paramIndex = fAPIUI.getParamIndex(msgAdress.c_str());
            float floatArg;
            if (msg->match(msgAdress).popFloat(floatArg).isOkNoMoreArgs() && paramIndex != -1) {
                fAPIUI.setParamValue(paramIndex, floatArg);
            // "get" message with correct address
            } else if (msg->match("/get").isOkNoMoreArgs()) {
                for (int p = 0; p < fAPIUI.getParamsCount(); ++p) {
                    // show data to console
                    printf("%s %f to %f\n", fAPIUI.getParamAddress(p), fAPIUI.getParamMin(p), fAPIUI.getParamMax(p));
                    // set data by OSC
                    //fSender.newMessage(std::string(fAPIUI.getParamAddress(p))).add(fAPIUI.getParamMin(p)).add(fAPIUI.getParamMax(p)).send();
                }
            // "hello" message
            } else if (msg->match("/hello").isOkNoMoreArgs()) {
                // show datat to console.
                printf("adresses:%s, in:%i, out:%i\n", fIP.c_str(), fInputPort, fOutputPort);
                // set data by OSC
                std::string s = fAPIUI.getParamAddress(0);
                s.erase(0, 1);
                s = s.substr(0, s.find("/"));
                s.insert(0, 1, '/');
                //fSender.newMessage(s).add(std::string(fIP)).add(fInputPort).add(fOutputPort).send();
            }
        }
    
        static void oscMessageReceived(oscpkt::Message* msg, void* arg)
        {
            static_cast<Esp32OSCUI*>(arg)->oscMessageReceivedAux(msg);
        }
        */

        static void event_handler(void* arg, esp_event_base_t event_base,
                                        int32_t event_id, void* event_data)
        {
            if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
                esp_wifi_connect();
            } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
                if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
                    esp_wifi_connect();
                    s_retry_num++;
                    ESP_LOGI(TAG, "retry to connect to the AP");
                } else {
                    xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
                }
                ESP_LOGI(TAG,"connect to the AP fail");
            } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
                ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
                sprintf(server_ip, IPSTR, IP2STR(&event->ip_info.ip));
                ESP_LOGI(TAG, "got ip:%s", server_ip);
                s_retry_num = 0;
                xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            }
        }

        void wifi_init_sta(void)
        {
            s_wifi_event_group = xEventGroupCreate();

            ESP_ERROR_CHECK(esp_netif_init());

            ESP_ERROR_CHECK(esp_event_loop_create_default());
            esp_netif_create_default_wifi_sta();

            wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
            ESP_ERROR_CHECK(esp_wifi_init(&cfg));

            ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
            ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

            wifi_config_t wifi_config = {
                .sta = {
                    EXAMPLE_ESP_WIFI_SSID, //bad way to do this, not sure why its forced to do this... has something to do with c++
                    EXAMPLE_ESP_WIFI_PASS
                    /* Setting a password implies station will connect to all security modes including WEP/WPA.
                    * However these modes are deprecated and not advisable to be used. Incase your Access point
                    * doesn't support WPA2, these mode can be enabled by commenting below line */
                    // .threshold.authmode = WIFI_AUTH_WPA2_PSK,

                    // .pmf_cfg = {
                    //       .capable = true,
                    //       .required = false,
                    //   },
                },
            };
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
            ESP_ERROR_CHECK(esp_wifi_start() );

            ESP_LOGI(TAG, "wifi_init_sta finished.");

            /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
            * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
            EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                    WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                    pdFALSE,
                    pdFALSE,
                    portMAX_DELAY);

            /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
            * happened. */
            if (bits & WIFI_CONNECTED_BIT) {
                ESP_LOGI(TAG, "connected to ap SSID: %s password: %s",
                        EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
            } else if (bits & WIFI_FAIL_BIT) {
                ESP_LOGI(TAG, "Failed to connect to SSID: %s, password: %s",
                        EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
            } else {
                ESP_LOGE(TAG, "UNEXPECTED EVENT");
            }

            ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
            ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
            vEventGroupDelete(s_wifi_event_group);
        }
        
        static void osc_server_task(void *pvParameters){
            static_cast<Esp32OSCUI*>(pvParameters)->osc_server_task();
        }


        void osc_server_task()
        {
            std::vector<char> rx_buffer;
            char addr_str[128];
            int addr_family;
            int ip_protocol;

            while (1) {

        #ifdef CONFIG_EXAMPLE_IPV4

                struct sockaddr_in dest_addr;
                dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
                dest_addr.sin_family = AF_INET;
                dest_addr.sin_port = htons(PORT);
                addr_family = AF_INET;
                ip_protocol = IPPROTO_IP;
                inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

        #else // IPV6
                struct sockaddr_in6 dest_addr;
                bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
                dest_addr.sin6_family = AF_INET6;
                dest_addr.sin6_port = htons(PORT);
                addr_family = AF_INET6;
                ip_protocol = IPPROTO_IPV6;
                inet6_ntoa_r(dest_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
        #endif
                //initializez listening socket
                int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
                if (sock < 0) {
                    ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
                    break;
                }
                ESP_LOGI(TAG, "Socket created");

                int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                if (err < 0) {
                    ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
                }
                ESP_LOGI(TAG, "Socket bound, port %d", PORT);

                oscpkt::PacketReader pr;

                while (1) {
                    
                    //normal size is 100k, but this creates overflow for the esp32. 30k should be enough. 
                    rx_buffer.resize(1024*32);

                    ESP_LOGI(TAG, "Waiting for data");

                    struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
                    socklen_t socklen = sizeof(source_addr); 

                    //recieve data
                    int len = recvfrom(sock, &rx_buffer[0], (int)rx_buffer.size(), 0, (struct sockaddr *)&source_addr, &socklen);

                    // Error occurred during receiving
                    if (len < 0) {
                        ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                        break;
                    }

                    // Data received
                    else {

                        //resize and swap before reading the data
                        rx_buffer.resize(len);
                        std::vector<char> tmp(rx_buffer); 
                        tmp.swap(rx_buffer);

                        pr.init((rx_buffer.empty() ? 0 : &rx_buffer[0]), rx_buffer.size());
                        oscpkt::Message *msg;

                        if(!pr.isOk()){
                            ESP_LOGE(TAG, "ospkt error: %i", pr.getErr());
                        }
                        while (pr.isOk() && (msg = pr.popMessage()) != 0) {

                            float floatArg;
                            std::string msgAddress = msg->addressPattern();
                            
                            //param address with a value to set in faust
                            if (msg->match(msgAddress).popFloat(floatArg).isOkNoMoreArgs()) {
                                //set parameter based on floatArg
                                printf("setting %s to %f\n", msgAddress.c_str(), floatArg);
                            } 

                            // "get" message with correct address
                            else if (msg->match("/get").isOkNoMoreArgs()) {
                                //print the faust args to console and send them back via osc
                                printf("here are some faustargs\n"); //just example text

                                for (int p = 0; p < fAPIUI.getParamsCount(); p ++){

                                }
                                
                                oscpkt::Message s_msg("/address"); //just an example message
                                s_msg.pushFloat(2.5); //add a random float

                                oscpkt::PacketWriter pw; //convert the message to a packet
                                pw.startBundle().startBundle().addMessage(s_msg).endBundle().endBundle();

                                // Get the sender's ip address & configure to send back
                                if (source_addr.sin6_family == PF_INET) { //ipv4?
                                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
                                } else if (source_addr.sin6_family == PF_INET6) { //ipv6?
                                    inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
                                }
                                source_addr.sin6_port = htons(OUTPORT); //set our output port
                                
                                //send our prepared packet
                                int err = sendto(sock, pw.packetData(), pw.packetSize(), 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                                if (err < 0) {
                                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                                    break;
                                }

                            }
                                
                            // "hello" message
                            else if (msg->match("/hello").isOkNoMoreArgs()) {

                                //show data to console
                                printf("address: %s, in:%i, out:%i\n", server_ip, PORT, OUTPORT);
                                
                                oscpkt::Message s_msg("/hello"); //just an example message
                                s_msg.pushStr(server_ip); //add our device ip address
                                s_msg.pushInt32(PORT); //add our read port
                                s_msg.pushInt32(OUTPORT); //add our output port
                                
                                oscpkt::PacketWriter pw; //convert the message to a packet
                                pw.startBundle().startBundle().addMessage(s_msg).endBundle().endBundle();

                                // Get the sender's ip address & configure to send back
                                if (source_addr.sin6_family == PF_INET) { //ipv4?
                                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
                                } else if (source_addr.sin6_family == PF_INET6) { //ipv6?
                                    inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
                                }
                                source_addr.sin6_port = htons(OUTPORT); //set our output port
                                
                                //send our prepared packet
                                int err = sendto(sock, pw.packetData(), pw.packetSize(), 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                                if (err < 0) {
                                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                                    break;
                                }
                            }
                        }
                    }
                }

                if (sock != -1) {
                    ESP_LOGE(TAG, "Shutting down socket and restarting...");
                    shutdown(sock, 0);
                    close(sock);
                }
            }
            vTaskDelete(NULL);
        }

        Esp32OSCUI(const std::string& ip, int in_port, int out_port)
        :fIP(ip), fInputPort(in_port), fOutputPort(out_port)
        {}
        
        virtual ~Esp32OSCUI()
        {}

        bool run() override
        { //setting up the vector of osc items with their relevent info. However, fOSCItems is not used elsewhere
            printf("initconnect\n");

            //do some initial startup things
            esp_err_t ret = nvs_flash_init();
            if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
            }
            ESP_ERROR_CHECK(ret);

            ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");

            wifi_init_sta(); //begin wifi

            xTaskCreate(osc_server_task, "OSC_server", 4096, NULL, 5, NULL); //start our osc_server

            //oscui item stuff. not sure about this one.
            if (fOSCItems.size() == 0) {
                printf("%i widgets, OSC Adresses:\n", fAPIUI.getParamsCount());
                for (int p = 0; p < fAPIUI.getParamsCount(); ++p) {
                    printf("%s %f to %f\n", fAPIUI.getParamAddress(p), fAPIUI.getParamMin(p), fAPIUI.getParamMax(p));
                    fOSCItems.push_back(new oscItem(this, fAPIUI.getParamAddress(p), fAPIUI.getParamZone(p)));
                }
            }

            return true;
        }
    
        // -- widget's layouts
        void openTabBox(const char* label) override { fAPIUI.openTabBox(label); }
        void openHorizontalBox(const char* label) override { fAPIUI.openHorizontalBox(label); }
        void openVerticalBox(const char* label) override { fAPIUI.openVerticalBox(label); }
        void closeBox() override { fAPIUI.closeBox(); }
        
        // -- active widgets
        
        void addButton(const char* label, FAUSTFLOAT* zone) override { fAPIUI.addButton(label, zone); }
        void addCheckButton(const char* label, FAUSTFLOAT* zone) override { fAPIUI.addCheckButton(label, zone); }
        
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        { fAPIUI.addVerticalSlider(label, zone, init, min, max, step); }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        { fAPIUI.addHorizontalSlider(label, zone, init, min, max, step); }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        { fAPIUI.addNumEntry(label, zone, init, min, max, step); }
        
        // -- passive widgets
        
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) override
        { fAPIUI.addHorizontalBargraph(label, zone, min, max); }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) override
        { fAPIUI.addVerticalBargraph(label, zone, min, max); }
        
        // -- metadata declarations
        
        void declare(FAUSTFLOAT* zone, const char* key, const char* val) override { fAPIUI.declare(zone, key, val); }
};

/***************************END OSC SECTION ***************************/

#endif // __BelaOSCUI__
/**************************  END  BelaOSCUI.h **************************/
