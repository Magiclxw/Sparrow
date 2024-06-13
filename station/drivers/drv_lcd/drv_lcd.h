#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "esp_err.h"
#include <string.h>
#include "lvgl.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_types.h"
#include "esp_lcd_panel_ops.h"

#define LCD_HOST  SPI2_HOST

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (20 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_SCLK           2
#define EXAMPLE_PIN_NUM_MOSI           3
#define EXAMPLE_PIN_NUM_MISO           -1
#define EXAMPLE_PIN_NUM_LCD_DC         1
#define EXAMPLE_PIN_NUM_LCD_RST        45
#define EXAMPLE_PIN_NUM_LCD_CS         46
#define EXAMPLE_PIN_NUM_BK_LIGHT       38

#define EXAMPLE_LCD_H_RES              240
#define EXAMPLE_LCD_V_RES              135

#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8

#define EXAMPLE_LVGL_TICK_PERIOD_MS    2


bool example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx);
void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
void example_lvgl_port_update_callback(lv_disp_drv_t *drv);
void example_increase_lvgl_tick(void *arg);

#endif