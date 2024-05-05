idf_component_register(SRCS "task_bluetooth.c"
                            "../../drivers/drv_ble/drv_ble.c"
                        INCLUDE_DIRS "."
                                     "../../drivers/drv_ble"
                        REQUIRES   esp_event driver bt)