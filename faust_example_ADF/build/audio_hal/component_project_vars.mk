# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += /home/dirk/esp/esp-adf/components/audio_hal/include /home/dirk/esp/esp-adf/components/audio_hal/driver/include /home/dirk/esp/esp-adf/components/audio_hal/driver/es8388 /home/dirk/esp/esp-adf/components/audio_hal/driver/es8374 /home/dirk/esp/esp-adf/components/audio_hal/driver/es8311 /home/dirk/esp/esp-adf/components/audio_hal/driver/es7243 /home/dirk/esp/esp-adf/components/audio_hal/driver/zl38063 /home/dirk/esp/esp-adf/components/audio_hal/driver/zl38063/api_lib /home/dirk/esp/esp-adf/components/audio_hal/driver/zl38063/example_apps /home/dirk/esp/esp-adf/components/audio_hal/driver/zl38063/firmware
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/audio_hal -laudio_hal -L/home/dirk/esp/esp-adf/components/audio_hal/driver/zl38063/firmware -lfirmware
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += audio_hal
COMPONENT_LDFRAGMENTS += 
component-audio_hal-build: 
