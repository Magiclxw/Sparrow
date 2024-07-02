#ifndef __DRV_HID_H
#define __DRV_HID_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "esp_log.h"

#define HID_PROTOCOL_START_H     (0xC0)
#define HID_PROTOCOL_START_L     (0x0C)
#define HID_PROTOCOL_STOP_H      (0xED)
#define HID_PROTOCOL_STOP_L      (0xDE)

#define HID_PROTOCOL_CMD_TEXT_START   (0x01)
#define HID_PROTOCOL_CMD_TEXT         (0x02)

#define DESC_STR_MAX 20
#define USBD_VID 0x2E8A /* Raspberry Pi */
#define USBD_PID 0x000A /* Raspberry Pi Pico SDK CDC */
#define USBD_STR_0 0x00
#define USBD_STR_MANUF 0x01
#define USBD_STR_PRODUCT 0x02
#define USBD_STR_SERIAL 0x03
#define USBD_STR_SERIAL_LEN 17
#define USBD_STR_CDC 0x04


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
void hid_data_send(uint8_t data[], uint8_t length);
void hid_data_upload(uint8_t data[], uint8_t length);
void hidSendProtocol(uint8_t cmd, uint8_t data[], uint8_t len);

#endif