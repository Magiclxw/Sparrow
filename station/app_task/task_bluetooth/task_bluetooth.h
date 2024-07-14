#ifndef __TASK_BLUETOOTH_H
#define __TASK_BLUETOOTH_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "drv_ble.h"

#define BLUETOOTH_TASK_STACK_SIZE   (8192)
#define BLUETOOTH_TASK_PRIORITY     (14)

#define BLUETOOTH_REC_QUEUE_LENGTH  (2)
#define BLUETOOTH_REC_QUEUE_SIZE    (1024)

#define BLUETOOTH_TRANS_QUEUE_LENGTH  (2)
#define BLUETOOTH_TRANS_QUEUE_SIZE    (1024)

#define CMD_MOUSE_MOVE              (0x01)
#define CMD_MOUSE_CLICK             (0x02)
#define CMD_KEYBOARD_INPUT          (0x03)
#define CMD_KEYBOARD_FUNC           (0x04)
#define CMD_HID_DATA_SEND           (0x05)
#define CMD_CFG_SET_SERVER          (0x06)
#define CMD_CFG_SET_USERNAME        (0x07) 
#define CMD_CFG_SET_PASSWORD        (0x08)
#define CMD_HID_SEND_TEXT_START     (0x09)
#define CMD_HID_SEND_TEXT           (0x0A)
#define CMD_CDC_SEND_FILE_START     (0x0B)
#define CMD_CDC_SEND_FILE           (0x0C)


int Bluetooth_Task_Create();
int bleTransTaskCreate();

#endif