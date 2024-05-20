idf_component_register(SRCS "task_led.c"
                        INCLUDE_DIRS "."
                                    "../../drivers/drv_led"
                        REQUIRES driver hal)