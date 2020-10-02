/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "es8388.h"
#include "FaustSaw.h"

extern "C"{
    void app_main(void);
}

void app_main(void)
{

    /*Configuration options for the es8388*/
    audio_hal_codec_config_t config; 
    config.adc_input = AUDIO_HAL_ADC_INPUT_ALL; //set the codec input configuration
    config.dac_output = AUDIO_HAL_DAC_OUTPUT_ALL; //set the codec output configuration
    config.codec_mode = AUDIO_HAL_CODEC_MODE_BOTH; //set the codec mode
    config.i2s_iface.samples = AUDIO_HAL_48K_SAMPLES; //set the sample rate (must match faust)
    config.i2s_iface.bits = AUDIO_HAL_BIT_LENGTH_32BITS; //set the bit depth (always 32)
    //more configuration options can be found in adf_structs.h

    /* initialize & configure */
    es8388 codec;
    codec.es8388_init(&config); //configure i2c and es8388 based on our config settings

    codec.es8388_config_i2s(config.codec_mode, &config.i2s_iface); //configure i2s based on our settings.

    audio_hal_ctrl_t state = AUDIO_HAL_CTRL_START; //create a start state to feed es8388_ctrl_state
    codec.es8388_ctrl_state(config.codec_mode, state); //start the audio codec with i/o configured based on config

     
    /* control functions */
    codec.es8388_set_voice_volume(70); //set the voice volume to 70

    codec.es8388_set_voice_mute(false); //set the voices to be unmuted (unmuted by default)

    codec.es8388_set_mic_gain(MIC_GAIN_12DB); //set the mic gain to be +12 dB

    
    /* read functions */
    codec.es8388_read_all(); //print out all the register values in order to serial

    //these commented functions break the audio codec, requiring a board restart. I have no idea why. 
    /*
    bool mute; 
    codec.es8388_get_voice_mute(&mute); //return if the voices are muted or not
    printf("mute state: %d\n", mute);
    
    int volume = 5; 
    codec.es8388_get_voice_volume(&volume); //return the voice volume
    printf("volume: %d\n", volume); 
    */

    
    /* Simple faust code */
    FaustSaw* saw = new FaustSaw(48000, 16);
    saw->start();

    bool mute = 0; 
    for(int i = 0; i < 10; i++){ //turn on and off the output of the codec several times
        codec.es8388_set_voice_mute(mute);
        mute = !mute; 
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

    /* shutdown funcions */
    state = AUDIO_HAL_CTRL_STOP; //change control state to stop
    codec.es8388_ctrl_state(config.codec_mode, state); //feed control state to codec

    codec.es8388_deinit(); //deinitalize i2c and the audio codec. 

    vTaskDelay(5000/portTICK_RATE_MS);

    esp_restart();
    
}
