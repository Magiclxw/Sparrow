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

extern esp_mqtt_client_handle_t client;
extern QueueHandle_t mqttQueueHandle;
extern SemaphoreHandle_t preStartupSemaphore;

#define MQTT_TOPIC_APP_RETAINED_SETTINGS            "/settings/app_config/retained"
#define MQTT_TOPIC_APP_DISRETAINED_SETTINGS         "/settings/app_config/disretained"
#define MQTT_TOPIC_DEVICE_RETAINED_SETTINGS         "/settings/device_config/retained"
#define MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS      "/settings/device_config/disretained"
#define MQTT_TOPIC_DEVICE_RETAINED_STATE            "/state/device_config/retained"
#define MQTT_TOPIC_DEVICE_DISRETAINED_STATE         "/state/device_config/disretained"
#define MQTT_TOPIC_DEVICE_RETAINED_STATISTICS       "/statistics/device_config/retained"
#define MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS    "/statistics/device_config/disretained"
#define MQTT_TOPIC_DEVICE_NOTIFICATION              "/notification"

typedef struct mqttDataStruct
{
    uint16_t topicLength;
    char topic[512];
    uint16_t dataLength;
    char data[1024];
}MqttDataStruct;

void initMqtt(void);
esp_err_t mqttSetBrokerAddr(const char *addr);
esp_err_t mqttSetBrokerUsername(const char *username);
esp_err_t mqttSetBrokerPassword(const char *password);

#endif