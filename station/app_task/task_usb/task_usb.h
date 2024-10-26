#ifndef __TASK_USB_H__
#define __TASK_USB_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define USB_TASK_STACK_SIZE (8192)
#define USB_TASK_PRIORITY (5)
#define USB_REC_QUEUE_LENGTH  (5)
#define USB_REC_QUEUE_SIZE  (64)

int createUsbTask();

#endif