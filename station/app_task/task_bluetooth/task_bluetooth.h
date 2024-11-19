#ifndef __TASK_BLUETOOTH_H
#define __TASK_BLUETOOTH_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "drv_ble.h"

#define BLUETOOTH_TASK_STACK_SIZE   (4096)
#define BLUETOOTH_TASK_PRIORITY     (14)

#define BLUETOOTH_REC_QUEUE_LENGTH  (5)
#define BLUETOOTH_REC_QUEUE_SIZE    (1024)

#define BLUETOOTH_TRANS_QUEUE_LENGTH  (5)
#define BLUETOOTH_TRANS_QUEUE_SIZE    (1024)

#define BLE_PROTOCOL_START_LEN      (2)
#define BLE_PROTOCOL_STOP_LEN       (2)
#define BEL_PROTOCOL_CMD_LEN        (1)
#define BLE_PROTOCOL_DATALEN_LEN    (1)
#define BLE_PROTOCOL_CHECKSUM_LEN   (1)

#define BLE_PROTOCOL_START_H        (0xA5)
#define BLE_PROTOCOL_START_L        (0x5A)
#define BLE_PROTOCOL_STOP_H         (0xF0) 
#define BLE_PROTOCOL_STOP_L         (0x0F)

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


int createBleRecTask();
int createBleTransTask();

#endif