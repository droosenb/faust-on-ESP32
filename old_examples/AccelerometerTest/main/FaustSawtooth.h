/************************************************************************
 FAUST Architecture File
 Copyright (C) 2019 GRAME, Centre National de Creation Musicale &
 Aalborg University (Copenhagen, Denmark)
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

#ifndef faust_FaustSawtooth_h_
#define faust_FaustSawtooth_h_

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "MPU9250.h"

class dsp;
class MapUI;

class APIUI;

class FaustSawtooth
{
    private:
    
        void configureI2S(int, int, i2s_pin_config_t);
        dsp* fDSP;
        MapUI* fUI;
        float **fInChannel, **fOutChannel;
        int fBS;

        MPU9250* mpu9250;
        TaskHandle_t aHandle;
        APIUI* sUI;

        void audioTask();
        static void audioTaskHandler(void*);
        TaskHandle_t fHandle;
    
    public:
    
        FaustSawtooth(int, int);
        ~FaustSawtooth();
    
        bool start();
        void stop();

        void setParamValue(const std::string&, float);

        void accelSetup();
        void accelTask();
        static void accelTaskHandler(void* arg);
    
        void gyroSetup();
        void gyroTask();
        static void gyroTaskHandler(void* arg);
        
        void magSetup();
        void magTask();
};

#endif
