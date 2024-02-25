idf_component_register(SRCS "driver_mqtt.c"
                        INCLUDE_DIRS "."
                        REQUIRES esp-tls esp_wifi esp_event mqtt)
