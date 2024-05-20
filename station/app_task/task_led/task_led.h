#ifndef _TASK_LED_H
#define _TASK_LED_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_TASK_STACK_SIZE (4096)
#define LED_TASK_PRIORITY (3)

int LED_Task_Create();

#endif