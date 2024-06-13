idf_component_register(SRCS "task_display.c"
                            "../../drivers/drv_lcd/drv_lcd.c"
                        INCLUDE_DIRS "."
                                     ".""../../managed_components"
                                     "../../drivers/drv_lcd"
                        REQUIRES   esp_event driver nvs_flash lvgl__lvgl esp_lcd esp_timer espressif__esp_lcd_ili9341)