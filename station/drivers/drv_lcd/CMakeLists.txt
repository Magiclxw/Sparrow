idf_component_register(SRCS "drv_lcd.c"
                        INCLUDE_DIRS ".""../../managed_components"
                        REQUIRES driver hal lvgl__lvgl esp_lcd)