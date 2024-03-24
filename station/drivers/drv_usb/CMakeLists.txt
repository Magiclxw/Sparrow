idf_component_register(SRCS "drv_hid.c"
                        INCLUDE_DIRS ".""../managed_components"
                        REQUIRES driver espressif__esp_tinyusb espressif_tinyusb)