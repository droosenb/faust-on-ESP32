/* Audio passthru

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "audio_pipeline.h"
#include "i2s_stream.h"
#include "board.h"
#include "FaustSaw.h"


static const char *TAG = "PASSTHRU";

extern "C" void app_main(void)
{
    
    ESP_LOGI(TAG, "[ 1 ] Start codec chip");
    audio_board_handle_t board_handle = audio_board_init();
    audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_BOTH,  AUDIO_HAL_CTRL_START);
    //audio_hal_set_volume(board_handle->audio_hal, 100);

    FaustSaw* saw = new FaustSaw(48000, 32);
    saw->start();
    
    // Waiting forever
    vTaskSuspend(nullptr);

}
