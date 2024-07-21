idf_component_register(SRCS "drv_usb.c"
                        INCLUDE_DIRS ".""../managed_components"
                        REQUIRES driver espressif__esp_tinyusb espressif_tinyusb)