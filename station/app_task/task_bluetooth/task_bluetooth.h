#ifndef __TASK_BLUETOOTH_H
#define __TASK_BLUETOOTH_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "drv_ble.h"

#define BLUETOOTH_TASK_STACK_SIZE   (8192)
#define BLUETOOTH_TASK_PRIORITY     (9)

#define BLUETOOTH_REC_QUEUE_LENGTH  (10)
#define BLUETOOTH_REC_QUEUE_SIZE    (sizeof(BleData_t))

int Bluetooth_Task_Create();

#endif