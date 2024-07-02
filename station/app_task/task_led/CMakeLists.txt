idf_component_register(SRCS "task_led.c"
                        INCLUDE_DIRS "."
                                    "../../drivers/drv_led"
                                    "../../drivers/drv_usb"
                        REQUIRES driver hal espressif__esp_tinyusb)