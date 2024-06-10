idf_component_register(SRCS "driver_mqtt.c"
                        INCLUDE_DIRS "."
                                    "../../app_task/task_servo"
                                    "../../main"
                        REQUIRES json esp-tls esp_wifi esp_event mqtt nvs_flash task_servo)
