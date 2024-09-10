idf_component_register(SRCS "task_http.c"
                            "../../drivers/drv_http/drv_http.c"
                        INCLUDE_DIRS "."
                                     "../../drivers/drv_http"
                        REQUIRES esp_event driver nvs_flash json esp_http_client)