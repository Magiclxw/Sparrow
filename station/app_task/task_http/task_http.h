#ifndef __TASK_HTTP_H__
#define __TASK_HTTP_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"


#define HTTP_TASK_STACK_SIZE    (4096)
#define HTTP_TASK_PRIORITY       (4)

int createHttpTask();

#endif