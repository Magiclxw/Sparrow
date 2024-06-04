idf_component_register(SRCS "drv_nvs.c"
                        INCLUDE_DIRS ".""../managed_components"
                        REQUIRES driver nvs_flash)