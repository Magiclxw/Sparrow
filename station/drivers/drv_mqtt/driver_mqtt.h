#ifndef __DRIVER_MQTT_H
#define __DRIVER_MQTT_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "mqtt_client.h"
#include "../../app_task/task_servo/task_servo.h"

extern esp_mqtt_client_handle_t client;

extern SemaphoreHandle_t preStartupSemaphore;

void mqtt_app_start(void);

#endif