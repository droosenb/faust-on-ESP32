
// default code for inserting faust stuff
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "WM8978.h" //file created by faust compiler for the audio codec
#include "FaustSawtooth.h" //faust object that you can fuck with

//run faust2esp32 -lib -[Audio Codec] [FileName] to gen these. Must unzip and update



extern "C" {
  void app_main(void);
}


void app_main(void)
{
  //used for initializing the audio codec and setting various gains
  WM8978 wm8978;
  wm8978.init();
  
  wm8978.addaCfg(1,1); //the input cfg? what are the args here
  wm8978.inputCfg(1,0,0);     
  wm8978.outputCfg(1,0); 

  wm8978.micGain(30);
  wm8978.auxGain(0);
  wm8978.lineinGain(0);
  wm8978.spkVolSet(0);
  wm8978.hpVolSet(40,40);//set hedphones to 40 gain. check doc
  wm8978.i2sCfg(2,0);

  //make the faust object
  int SR = 48000;
  int BS = 8;
  FaustSawtooth faustSawtooth(SR,BS); //the sexy faust constructor
  //how is faust controlling what is input and what is output?
  faustSawtooth.start();

  faustSawtooth.setParamValue("freq", 220);
  /*
  while(true){
    char writeBuf;
    vTaskGetRunTimeStats(&writeBuf);
    printf("%i\n", writeBuf);
  }
  */
  
}
