#include "task_mqtt.h"
#include "drv_jsonHandler.h"
#include "ui.h"

TaskHandle_t taskMqttHandle = NULL;


static void mqttTask(void *pvParameters);

int createMqttTask()
{
    xTaskCreate((TaskFunction_t)mqttTask,
                (const char*)"httpTask",
                (uint32_t )MQTT_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)MQTT_TASK_PRIORITY,
                &taskMqttHandle);
    return ESP_OK;
}

static void mqttTask(void *pvParameters)
{
    char mqttData[MQTT_REC_QUEUE_SIZE] = {0};
    mqttQueueHandle = xQueueCreate(MQTT_REC_QUEUE_LENGTH, MQTT_REC_QUEUE_SIZE);

    initMqtt();

    while(1)
    {
        xQueueReceive(mqttQueueHandle, &mqttData, portMAX_DELAY);
        printf("mqttData: %s, len:%d\n", mqttData, strlen(mqttData));

        if (strcmp(MQTT_TOPIC_APP_RETAINED_SETTINGS, mqttData) == 0)
        {
            printf("app retained settings = %s\n",mqttData);
            setAppRetainedSettings(mqttData);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_NOTIFICATION, mqttData) == 0)
        {
            printf("notification = %s\r\n",mqttData);
            ui_screen2SetNotification(mqttData);
        }
        // if(memcmp(MQTT_TOPIC_APP_RETAINED_SETTINGS,event->topic,event->topic_len) == 0)
        // {
        //     printf("app retained settings = %s\n",event->data);
        //     setAppRetainedSettings(event->data);
        // }
        // else if (memcmp(MQTT_TOPIC_DEVICE_NOTIFICATION, event->topic,event->topic_len) == 0)
        // {
        //     printf("notification = %s\r\n",data);
        //     ui_screen2SetNotification(data);
        // }
    }
}