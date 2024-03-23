#ifndef __TASK_BATTERY_H
#define __TASK_BATTERY_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_sleep.h"

#define BATTERY_TASK_STACK_SIZE (8124)
#define BATTERY_TASK_PRIORITY       (6)

#define DEFAULT_RUNTIME (60*1000)   //默认运行时间(60s)
#define DEFAULT_DEEP_SLEEP_SEC (1*60) //默认睡眠时间(1h)

typedef enum BATTERY_STATE
{
    BATTERY_STATE_UNKNOWN = 0,
    BATTERY_STATE_USB,
    BATTERY_STATE_BATTERY,
    BATTERY_STATE_LOW_VOL,
    BATTERY_STATE_ERROR,
}BATTERY_STATE_e;

int Battery_Task_Create();

#endif