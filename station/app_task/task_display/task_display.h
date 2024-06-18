#ifndef __TASK_DISPLAY_H__
#define __TASK_DISPLAY_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lvgl.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "esp_lcd_panel_vendor.h"
#include "drv_lcd.h"
#include "esp_timer.h"
#include "esp_lcd_ili9341.h"
#include "../src/ui/ui_startPage.h"

#define LCD_TASK_STACK_SIZE (4096)
#define LCD_TASK_PRIORITY   (10)


uint8_t displayTaskCreate();

#endif