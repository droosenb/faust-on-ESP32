/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

#include "MPU9250.h"


extern "C" {
  void app_main(void);
}


void app_main(void)
{

    MPU9250 mySensor(MPU9250_ADDRESS_AD0_HIGH);

    //mySensor.beginAccel();
    mySensor.beginMag();
    //mySensor.beginGyro();
    for(int i = 0; i < 40; i++){
        
        mySensor.accelUpdate();
	
        printf("print accel values\n");
        printf("accelX:");
        printf("%f\n", mySensor.accelX());
        printf("accelY:");
        printf("%f\n", mySensor.accelY());
        printf("accelZ:");
        printf("%f\n", mySensor.accelZ());
        
        
        mySensor.magUpdate();
        printf("print mag values\n");
        printf("magX:");
        printf("%f\n", mySensor.magX());
        printf("magY:");
        printf("%f\n", mySensor.magY());
        printf("magZ:");
        printf("%f\n", mySensor.magZ());

        
        mySensor.gyroUpdate();
        printf("print mag values\n");
        printf("gyroX:");
        printf("%f\n", mySensor.gyroX());
        printf("gyroY:");
        printf("%f\n", mySensor.gyroY());
        printf("gyroZ:");
        printf("%f\n", mySensor.gyroZ());
        
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }

}
