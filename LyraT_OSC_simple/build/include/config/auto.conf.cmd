deps_config := \
	/home/dirk/esp/esp-adf/esp-idf/components/app_trace/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/aws_iot/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/bt/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/driver/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/efuse/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/esp32/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/esp_adc_cal/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/esp_event/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/esp_http_client/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/esp_http_server/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/esp_https_ota/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/espcoredump/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/ethernet/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/fatfs/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/freemodbus/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/freertos/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/heap/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/libsodium/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/log/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/lwip/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/mbedtls/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/mdns/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/mqtt/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/nvs_flash/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/openssl/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/pthread/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/spi_flash/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/spiffs/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/tcpip_adapter/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/unity/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/vfs/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/wear_levelling/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/wifi_provisioning/Kconfig \
	/home/dirk/esp/esp-adf/esp-idf/components/app_update/Kconfig.projbuild \
	/home/dirk/esp/esp-adf/components/audio_board/Kconfig.projbuild \
	/home/dirk/esp/esp-adf/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/dirk/esp/esp-adf/components/esp-adf-libs/Kconfig.projbuild \
	/home/dirk/esp/esp-adf/components/esp-sr/Kconfig.projbuild \
	/home/dirk/esp/esp-adf/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/dirk/Projects/LyraT/udp_server/main/Kconfig.projbuild \
	/home/dirk/esp/esp-adf/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/dirk/esp/esp-adf/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
