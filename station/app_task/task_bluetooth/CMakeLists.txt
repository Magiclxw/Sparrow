idf_component_register(SRCS "task_bluetooth.c"
                            "../../drivers/drv_ble/drv_ble.c"
                        INCLUDE_DIRS "."
                                     "../../drivers/drv_ble"
                                     "../../drivers/drv_usb"
                        REQUIRES   esp_event driver bt espressif__esp_tinyusb)