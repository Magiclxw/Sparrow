idf_component_register(SRCS "main.c"
                            "../drivers/wifi/wifi_station.c"
                            "../drivers/drv_mqtt/driver_mqtt.c"
                            "../drivers/drv_adc/drv_adc.c"
                            "../app_task/task_servo/task_servo.c"
                            "../app_task/task_rtc/task_rtc.c"
                            "../app_task/task_battery/task_battery.c"
                            
                    INCLUDE_DIRS "."
                                "../drivers/drv_mqtt"
                                "../drivers/wifi"
                                "../app_task")
