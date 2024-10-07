#ifndef __TASK_USB_H__
#define __TASK_USB_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define USB_TASK_STACK_SIZE (4096)
#define USB_TASK_PRIORITY (5)
#define USB_REC_QUEUE_LENGTH  (5)
#define USB_REC_QUEUE_SIZE  (100)

int createUsbTask();

#endif