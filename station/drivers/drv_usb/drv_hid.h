#ifndef __DRV_HID_H
#define __DRV_HID_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "esp_log.h"

void hid_test(void);
void hid_input_char(char c);
void hid_mouse_click(hid_mouse_button_bm_t button);
void hid_mouse_move(uint8_t x,uint8_t y);

#endif