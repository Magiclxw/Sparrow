idf_component_register(SRCS "drv_http.c"
                        INCLUDE_DIRS "."
                        REQUIRES json esp-tls esp_wifi esp_http_client esp_event nvs_flash json)
