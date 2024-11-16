#include "task_mqtt.h"
#include "drv_jsonHandler.h"
#include "ui.h"

TaskHandle_t taskMqttHandle = NULL;


static void mqttTask(void *pvParameters);

int createMqttTask()
{
    xTaskCreate((TaskFunction_t)mqttTask,
                (const char*)"mqttTask",
                (uint32_t )MQTT_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)MQTT_TASK_PRIORITY,
                &taskMqttHandle);
    return ESP_OK;
}

static void mqttTask(void *pvParameters)
{
    MqttDataStruct mqttData = {0};

    mqttQueueHandle = xQueueCreate(MQTT_REC_QUEUE_LENGTH, sizeof(mqttData));

    initMqtt();
    // 等待连接完成
    xSemaphoreTake(preStartupSemaphore,portMAX_DELAY);

    // 设置设备状态
    jsonSetDeviceState(1);
    // 发送设备状态
    drvMqttSendRetainedState();

    while(1)
    {
        xQueueReceive(mqttQueueHandle, &mqttData, portMAX_DELAY);

        printf("mqttData: %s, len:%d\n", mqttData.data, mqttData.dataLength);

        if (strcmp(MQTT_TOPIC_APP_RETAINED_SETTINGS, mqttData.topic) == 0)
        {
            printf("app retained settings = %s\n",mqttData.data);
            setAppRetainedSettings(mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_APP_DISRETAINED_SETTINGS,mqttData.topic) == 0)
        {
            printf("app disretained settings = %s\n",mqttData.data);
            setAppDisretainedSettings(mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_RETAINED_SETTINGS, mqttData.topic) == 0)
        {
            printf("device retained settings = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS, mqttData.topic) == 0)
        {
            printf("device disretained settings = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_RETAINED_STATE, mqttData.topic) == 0)
        {
            printf("device retained state = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_DISRETAINED_STATE, mqttData.topic) == 0)
        {
            printf("device disretained state = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_RETAINED_STATISTICS, mqttData.topic) == 0)
        {
            printf("device retained statistics = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS, mqttData.topic) == 0)
        {
            printf("device disretained statistics = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_NOTIFICATION, mqttData.topic) == 0)
        {
            printf("notification = %s\r\n",mqttData.data);
            setAppNotification(mqttData.data);  
            
            ui_Screen_Main_SetNotification(getAppNotification());
        }

        memset(&mqttData, 0 , sizeof(mqttData));

        vTaskDelay(pdMS_TO_TICKS(100));
        // if(memcmp(MQTT_TOPIC_APP_RETAINED_SETTINGS,event->topic,event->topic_len) == 0)
        // {
        //     printf("app retained settings = %s\n",event->data);
        //     setAppRetainedSettings(event->data);
        // }
        // else if (memcmp(MQTT_TOPIC_DEVICE_NOTIFICATION, event->topic,event->topic_len) == 0)
        // {
        //     printf("notification = %s\r\n",data);
        //     ui_Screen_Main_SetNotification(data);
        // }
    }
}