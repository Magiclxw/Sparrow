#ifndef __DRV_HID_H
#define __DRV_HID_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "esp_log.h"

typedef enum MouseClickState
{
    MouseClickState_Pressed = 0,
    MouseClickState_Released,
    MouseClickState_Clicked,
}MouseClickState_e;

void hid_test(void);
void hid_input_char(char c);
void hid_input_func(uint8_t modifier,uint8_t keyCode);
void hid_mouse_click(hid_mouse_button_bm_t button, MouseClickState_e state);
void hid_mouse_move(uint8_t x,uint8_t y);

#endif