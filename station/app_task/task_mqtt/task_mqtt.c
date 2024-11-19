#include "task_mqtt.h"
#include "drv_jsonHandler.h"
#include "ui.h"
#include "drv_usb.h"
#include "drv_power.h"
#include "drv_servo.h"
#include "drv_nvs.h"

TaskHandle_t taskMqttHandle = NULL;
TimerHandle_t timerHandle = NULL;

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

static void passwordDelayTimer()
{
    // 自动输入密码
    hid_input_passowrd(getAppRetainedSettings()->password);
}

static void mqttTask(void *pvParameters)
{
    MqttDataStruct mqttData = {0};
    static uint8_t firstPowerOnFlag = 0;
    static uint32_t lastAngle = 0;
    static uint32_t idleAngle = 0;
    static uint32_t posAngle = 0;
    static uint32_t negAngle = 0;

    mqttQueueHandle = xQueueCreate(MQTT_REC_QUEUE_LENGTH, sizeof(mqttData));
    timerHandle = xTimerCreate("PasswordDelayTimer", 6000, 0, 0, passwordDelayTimer);
    
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
            // printf("app retained settings = %s\n",mqttData.data);
            setAppRetainedSettings(mqttData.data);
            if (getAppRetainedSettings()->powerCtrl != firstPowerOnFlag)
            {
                firstPowerOnFlag = getAppRetainedSettings()->powerCtrl;
                // 不需开关机
                if (firstPowerOnFlag == POWER_NO_CTRL) continue;

                // 控制电脑开关机
                drvPowerOnOff();
                // 判断自动输入密码功能是否开启
                if (getAppRetainedSettings()->passwordCtrl == 1)
                {
                    // 开启自动输入密码定时器
                    xTimerChangePeriod(timerHandle,pdMS_TO_TICKS((getAppRetainedSettings()->passwordWait) * 1000), 0);
                    
                }
                // 舵机开机
                if (getAppRetainedSettings()->powerCtrl == POWER_ON)
                {
                    nvsLoadPosAngle(&posAngle);
                    drvServoSetAngle(posAngle);
                }
                // 舵机关机
                else if (getAppRetainedSettings()->powerCtrl == POWER_OFF)
                {
                    nvsLoadNegAngle(&negAngle);
                    drvServoSetAngle(negAngle);
                }
                vTaskDelay(pdMS_TO_TICKS(500));
                // 恢复空闲角度
                nvsLoadIdleAngle(&idleAngle);
                drvServoSetAngle(idleAngle);
                // 清空开关信号
                getAppRetainedSettings()->powerCtrl = POWER_NO_CTRL;
                drvMqttSendAppRetainedSettings();
            }
        }
        else if (strcmp(MQTT_TOPIC_APP_DISRETAINED_SETTINGS,mqttData.topic) == 0)
        {
            // printf("app disretained settings = %s\n",mqttData.data);
            setAppDisretainedSettings(mqttData.data);
            // 设置舵机调度
            if (lastAngle != getAppDisretainedSettings()->turnAngle && getAppDisretainedSettings()->saveAngle == 1)
            {
                lastAngle = getAppDisretainedSettings()->turnAngle;
                drvServoSetAngle(lastAngle);
            }
            
            // 空闲时角度
            if (getAppDisretainedSettings()->saveAngle == 2)
            {
                nvsSaveIdleAngle(getAppDisretainedSettings()->turnAngle);
            }
            // 正向时角度
            else if (getAppDisretainedSettings()->saveAngle == 3)
            {
                nvsSavePosAngle(getAppDisretainedSettings()->turnAngle);
            }
            // 反向时角度
            else if (getAppDisretainedSettings()->saveAngle == 4)
            {
                nvsSaveNegAngle(getAppDisretainedSettings()->turnAngle);
            }
            if (getAppDisretainedSettings()->tokenCtrl == 1 && getAppRetainedSettings()->toolsToken == 1)
            {
                // 开启屏幕
                // 自动输入密码
                hid_input_passowrd(getAppRetainedSettings()->password);
            }
            else if (getAppDisretainedSettings()->tokenCtrl == 2 && getAppRetainedSettings()->toolsToken == 1)
            {
                // 关闭屏幕 win + L
                hid_input_func(KEYBOARD_MODIFIER_LEFTGUI, HID_KEY_L);  
            }
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_RETAINED_SETTINGS, mqttData.topic) == 0)
        {
            // printf("device retained settings = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS, mqttData.topic) == 0)
        {
            // printf("device disretained settings = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_RETAINED_STATE, mqttData.topic) == 0)
        {
            // printf("device retained state = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_DISRETAINED_STATE, mqttData.topic) == 0)
        {
            // printf("device disretained state = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_RETAINED_STATISTICS, mqttData.topic) == 0)
        {
            // printf("device retained statistics = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS, mqttData.topic) == 0)
        {
            // printf("device disretained statistics = %s\n",mqttData.data);
        }
        else if (strcmp(MQTT_TOPIC_DEVICE_NOTIFICATION, mqttData.topic) == 0)
        {
            // printf("notification = %s\r\n",mqttData.data);
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