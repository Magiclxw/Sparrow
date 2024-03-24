idf_component_register(SRCS "task_battery.c"
                            "../../drivers/drv_adc/drv_adc.c"
                            "../../drivers/drv_mqtt/driver_mqtt.c"
                        INCLUDE_DIRS "."
                                     "../../drivers/drv_adc"
                                     "../../drivers/drv_mqtt"
                        REQUIRES esp_hw_support esp_adc esp_event mqtt driver nvs_flash)