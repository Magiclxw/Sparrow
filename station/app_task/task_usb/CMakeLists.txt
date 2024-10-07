idf_component_register(SRCS "task_usb.c"
                            "../../drivers/drv_usb/drv_usb.c"
                        INCLUDE_DIRS "."
                        REQUIRES esp_event driver nvs_flash espressif__esp_tinyusb espressif_tinyusb)