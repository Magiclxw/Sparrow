#ifndef __DRV_USB_H
#define __DRV_USB_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "esp_log.h"

#define USB_PROTOCOL_START_H     (0xC0)
#define USB_PROTOCOL_START_L     (0x0C)
#define USB_PROTOCOL_STOP_H      (0xED)
#define USB_PROTOCOL_STOP_L      (0xDE)


#define USB_PROTOCOL_CMD_TEXT_START   (0x01)
#define USB_PROTOCOL_CMD_TEXT         (0x02)
#define USB_PROTOCOL_CMD_FILE_START   (0x03)
#define USB_PROTOCOL_CMD_FILE         (0x04)

typedef enum MouseClickState
{
    MouseClickState_Pressed = 0,
    MouseClickState_Released,
    MouseClickState_Clicked,
}MouseClickState_e;

void intiUsb(void);
void hid_input_char(char c);
void hid_input_func(uint8_t modifier,uint8_t keyCode);
void hid_mouse_click(hid_mouse_button_bm_t button, MouseClickState_e state);
void hid_mouse_move(uint8_t x,uint8_t y);
void hid_data_send(uint8_t data[], uint8_t length);
void hid_data_upload(uint8_t data[], uint8_t length);
void hidSendProtocol(uint8_t cmd, uint8_t data[], uint8_t len);
void cdcSendProtocol(uint8_t cmd, uint8_t data[], uint16_t len);

#endif