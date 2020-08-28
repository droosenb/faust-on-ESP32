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

    es8388 codec;
    audio_hal_codec_config_t config; 
    config.adc_input = AUDIO_HAL_ADC_INPUT_ALL; //set the codec input configuration
    config.dac_output = AUDIO_HAL_DAC_OUTPUT_ALL; //set the codec output configuration
    config.codec_mode = AUDIO_HAL_CODEC_MODE_BOTH; //set the codec mode
    config.i2s_iface.samples = AUDIO_HAL_48K_SAMPLES; //set the sample rate (must match faust)
    config.i2s_iface.bits = AUDIO_HAL_BIT_LENGTH_32BITS; //set the bit depth (always 32)

    printf("init\n");
    codec.es8388_init(&config);
    printf("start\n");
    codec.es8388_start(ES_MODULE_ADC_DAC);
    printf("Read addresses\n");
    codec.es8388_read_all();
    printf("set voice volume\n");
    codec.es8388_set_voice_volume(70);
    //codec.es8388_set_voice_volume(0);
    //codec.es8388_deinit();

    FaustSaw* saw = new FaustSaw(48000, 32);
    saw->start();

    int unshifted = ES8388_ADDR;
    int shifted = ES8388_ADDR << 0; 
    printf ("unshifted: %u \n" , unshifted);
    printf("shifted: %u\n", shifted);
    /*
    bool mute = 0; 
    
    while(1){
        printf("status: %d\n", mute);
        codec.es8388_set_voice_mute(mute);
        mute = !mute; 
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    */
    vTaskSuspend(nullptr);
}
