idf_component_register(SRCS "task_mqtt.c"
                            "../../drivers/drv_http/drv_mqtt.c"
                        INCLUDE_DIRS "."
                                     "../../drivers/drv_mqtt"
                        REQUIRES esp_event driver nvs_flash json)