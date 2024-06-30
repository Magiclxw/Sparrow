#ifndef __DRV_BLE_H
#define __DRV_BLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include "sdkconfig.h"

extern QueueHandle_t Bluetooth_Queue_Handle;

#define BLE_PROTOCOL_START_H    (0xA5)
#define BLE_PROTOCOL_START_L    (0x5A)
#define BLE_PROTOCOL_STOP_H     (0xF0)
#define BLE_PROTOCOL_STOP_L     (0x0F)

typedef struct BleData
{
    uint8_t length;
    uint8_t data[100];
}BleData_t;

typedef struct bleTransDataStruct
{
    uint8_t start[2];
    uint8_t cmd;
    uint8_t length;
    uint8_t data[100];
    uint8_t checksum;
    uint8_t stop[2];
}BleTransDataStruct;

void initBLE();
void bleSendProtocol(uint8_t cmd, uint8_t data[], uint8_t length);

#endif