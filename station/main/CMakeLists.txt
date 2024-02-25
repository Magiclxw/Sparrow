idf_component_register(SRCS "main.c"
                            "../drivers/wifi/wifi_station.c"
                            "../drivers/drv_mqtt/driver_mqtt.c"
                            "../app_task/task_servo/task_servo.c"
                    INCLUDE_DIRS "."
                                "../drivers/drv_mqtt"
                                "../drivers/wifi"
                                "../app_task")
