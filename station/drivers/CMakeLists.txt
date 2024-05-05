idf_component_register(SRCS "drv_dac/drv_adc.c"
                            "drv_usb/drv_hid.c"
                            "drv_ble/drv_ble.c"
                        INCLUDE_DIRS "drv_dac"
                        "drv_ble"
                        "managed_components"
)