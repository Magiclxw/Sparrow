#ifndef __TASK_RTC_H
#define __TASK_RTC_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"

#define RTC_TASK_STACK_SIZE (2048)
#define RTC_TASK_PRIORITY   (5)


int createRtcTask();

#endif