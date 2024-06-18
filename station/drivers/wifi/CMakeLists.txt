idf_component_register(SRCS "wifi_station.c"
                        INCLUDE_DIRS "."
                        REQUIRES json esp-tls esp_wifi esp_event esp_netif nvs_flash lwip esp_netif)

                        