#ifndef __TASK_MQTT_H__
#define __TASK_MQTT_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include "drv_mqtt.h"

#define MQTT_TASK_STACK_SIZE    (4096)
#define MQTT_TASK_PRIORITY      (5)
#define MQTT_REC_QUEUE_LENGTH  (5)
#define MQTT_REC_QUEUE_SIZE   (1024)

int createMqttTask();

#endif