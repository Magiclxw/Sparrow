#include "drv_jsonHandler.h"
#include "drv_nvs.h"
#include "drv_servo.h"
#include "drv_power.h"
#include "drv_mqtt.h"

static AppRetainedSettingsStruct s_appRetainedSettings = {0};
static AppDisretainedSettingsStruct s_appDisretainedSettings = {0};
static DeviceRetainedStateStruct s_deviceRetainedState = {0};
static DeviceRetainedStatisticsStruct s_deviceRetainedStateStatistics = {0};
static DeviceDisretainedStatisticsStruct s_deviceDisretainedStatistics = {0};
static char s_notification[256];

void drvJsonHandlerInit()
{

}

/**
 * @brief 解析app端发送的保留设置数据
 * @param[in] data : 原始JSON数据
 * @return : 执行结果
 */
esp_err_t setAppRetainedSettings(char *data)
{
    cJSON *jsonData = NULL;

    cJSON *jsonWakeupInterval = NULL;
    cJSON *jsonPowerCtrl = NULL;
    cJSON *jsonPCPassword = NULL;
    cJSON *jsonPCPasswordCtrl = NULL;
    cJSON *jsonPCPasswordWait = NULL;
    cJSON *jsonLed = NULL;
    cJSON *jsonToken = NULL;

    static uint32_t idleAngle = 0;
    static uint32_t posAngle = 0;
    static uint32_t negAngle = 0;

    jsonData = cJSON_Parse(data);

    if(jsonData == NULL)
    {
        return ESP_FAIL;
    }

    jsonWakeupInterval = cJSON_GetObjectItem(jsonData, JSON_KEY_WAKE_UP_INTERVAL);

    if(jsonWakeupInterval == NULL)
    {
        return ESP_FAIL;
    }

    jsonPowerCtrl = cJSON_GetObjectItem(jsonData, JSON_KEY_POWER_ON_OFF);

    if(jsonPowerCtrl == NULL)
    {
        return ESP_FAIL;
    }

    jsonPCPassword = cJSON_GetObjectItem(jsonData, JSON_KEY_PC_PASSWORD);

    if(jsonPCPassword == NULL)
    {
        return ESP_FAIL;
    }

    jsonPCPasswordCtrl = cJSON_GetObjectItem(jsonData, JSON_KEY_PC_PASSWORD_CTRL);

    if(jsonPCPasswordCtrl == NULL)
    {
        return ESP_FAIL;
    }

    jsonPCPasswordWait = cJSON_GetObjectItem(jsonData, JSON_KEY_PC_PASSWORD_WAIT);

    if(jsonPCPasswordWait == NULL)
    {
        return ESP_FAIL;
    }

    jsonLed = cJSON_GetObjectItem(jsonData, JSON_KEY_LED_CTRL);

    if(jsonLed == NULL)
    {
        return ESP_FAIL;
    }

    jsonToken = cJSON_GetObjectItem(jsonData, JSON_KEY_TOOLS_TOKEN);

    if(jsonToken == NULL)
    {
        return ESP_FAIL;
    }

    if (s_appRetainedSettings.wakeupInterval != jsonWakeupInterval->valueint)
    {
        s_appRetainedSettings.wakeupInterval = jsonWakeupInterval->valueint;
        nvsSaveWakeupInterval(s_appRetainedSettings.wakeupInterval);
    }
    
    if (strcmp(s_appRetainedSettings.password,jsonPCPassword->valuestring) != 0)
    {
        strcpy(s_appRetainedSettings.password, jsonPCPassword->valuestring);
        nvsSavePcPassword(s_appRetainedSettings.password);
    }
    if (s_appRetainedSettings.passwordCtrl != jsonPCPasswordCtrl->valueint)
    {
        s_appRetainedSettings.passwordCtrl = jsonPCPasswordCtrl->valueint;
        nvsSavePcPasswordCtrl(s_appRetainedSettings.passwordCtrl);
    }
    if (s_appRetainedSettings.passwordWait != jsonPCPasswordWait->valueint)
    {
        s_appRetainedSettings.passwordWait = jsonPCPasswordWait->valueint;
        nvsSavePcPasswordWait(s_appRetainedSettings.passwordWait);
    }
    if (s_appRetainedSettings.ledCtrl != jsonLed->valueint)
    {
        s_appRetainedSettings.ledCtrl = jsonLed->valueint;
        nvsSaveLedCtrl(s_appRetainedSettings.passwordWait);
    }
    if (s_appRetainedSettings.toolsToken != jsonToken->valueint)
    {
        s_appRetainedSettings.toolsToken = jsonToken->valueint;
        nvsSaveToolsTokenCtrl(s_appRetainedSettings.toolsToken);
    }
    // 最后解析开关机数据，防止清空开关信号时清空其他数据
    if (s_appRetainedSettings.powerCtrl != jsonPowerCtrl->valueint)
    {
        s_appRetainedSettings.powerCtrl = jsonPowerCtrl->valueint;
        nvsSavePowerCtrl(s_appRetainedSettings.powerCtrl);
        // 不需开关机
        if (s_appRetainedSettings.powerCtrl == POWER_NO_CTRL) return ESP_OK;

        // 控制电脑开关机
        drvPowerOnOff();
        // 舵机开机
        if (s_appRetainedSettings.powerCtrl == POWER_ON)
        {
            nvsLoadPosAngle(&posAngle);
            drvServoSetAngle(posAngle);
        }
        // 舵机关机
        else if (s_appRetainedSettings.powerCtrl == POWER_OFF)
        {
            nvsLoadNegAngle(&negAngle);
            drvServoSetAngle(negAngle);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
        // 恢复空闲角度
        nvsLoadIdleAngle(&idleAngle);
        drvServoSetAngle(idleAngle);
        // 清空开关信号
        s_appRetainedSettings.powerCtrl = POWER_NO_CTRL;
        drvMqttSendAppRetainedSettings();
    }
    return ESP_OK;
}

AppRetainedSettingsStruct getAppRetainedSettings()
{
    
    return s_appRetainedSettings;
}

esp_err_t setAppDisretainedSettings(char *data)
{
    cJSON *jsonData = NULL;

    cJSON *jsonTurnAngle = NULL;
    cJSON *jsonSaveAngle = NULL;

    jsonData = cJSON_Parse(data);

    if(jsonData == NULL)
    {
        return ESP_FAIL;
    }

    jsonTurnAngle = cJSON_GetObjectItem(jsonData, JSON_KEY_TURN_ANGLE);

    if(jsonTurnAngle == NULL)
    {
        return ESP_FAIL;
    }

    jsonSaveAngle = cJSON_GetObjectItem(jsonData, JSON_KEY_SAVE_ANGLE);

    if(jsonSaveAngle == NULL)
    {
        return ESP_FAIL;
    }

    s_appDisretainedSettings.turnAngle = jsonTurnAngle->valueint - 90;
    s_appDisretainedSettings.saveAngle = jsonSaveAngle->valueint;

    cJSON_Delete(jsonData);

    drvServoSetAngle(s_appDisretainedSettings.turnAngle);

    // 空闲时角度
    if (s_appDisretainedSettings.saveAngle == 1)
    {
        nvsSaveIdleAngle(s_appDisretainedSettings.turnAngle);
    }
    // 正向时角度
    else if (s_appDisretainedSettings.saveAngle == 2)
    {
        nvsSavePosAngle(s_appDisretainedSettings.turnAngle);
    }
    // 逆向时角度
    else if (s_appDisretainedSettings.saveAngle == 3)
    {
        nvsSaveNegAngle(s_appDisretainedSettings.turnAngle);
    }

    return ESP_OK;
}

AppDisretainedSettingsStruct getAppDisretainedSettings()
{
    return s_appDisretainedSettings;
}

esp_err_t setDeviceRetainedState(DeviceRetainedStateStruct state)
{
    s_deviceRetainedState.powerState = state.powerState;
    s_deviceRetainedState.lastPowerOnTime = state.lastPowerOnTime;
    s_deviceRetainedState.lastPowerOffTime = state.lastPowerOffTime;
    s_deviceRetainedState.nextPowerOnTime = state.nextPowerOnTime;
    s_deviceRetainedState.nextPowerOffTime = state.nextPowerOffTime;
    s_deviceRetainedState.lastWakeUpTime = state.lastWakeUpTime;
    s_deviceRetainedState.lastSleepTime = state.lastSleepTime;
    s_deviceRetainedState.nextWakeUpTime = state.nextWakeUpTime;
    s_deviceRetainedState.nextSleepTime = state.nextSleepTime;
    s_deviceRetainedState.deviceState = state.deviceState;
    return ESP_OK;
}

esp_err_t setAppNotification(char *data)
{
    cJSON *jsonData = NULL;
    cJSON *jsonNotification = NULL;

    jsonData = cJSON_Parse(data);

    if(jsonData == NULL)
    {
        return ESP_FAIL;
    }

    jsonNotification = cJSON_GetObjectItem(jsonData, JSON_KEY_NOTIFICATION);

    if(jsonNotification == NULL)
    {
        return ESP_FAIL;
    }

    strcpy(s_notification, jsonNotification->valuestring);

    cJSON_Delete(jsonData);

    return ESP_OK;
}

char *getAppNotification()
{
    return s_notification;
}

esp_err_t jsonSetPowerState(PowerStateEnum powerState)
{
    s_deviceRetainedState.powerState = powerState;
    return ESP_OK;
}

esp_err_t jsonSetLastPowerOnTime(uint32_t time)
{
    s_deviceRetainedState.lastPowerOnTime = time;
    return ESP_OK;
}

esp_err_t jsonSetLastPowerOffTime(uint32_t time)
{
    s_deviceRetainedState.lastPowerOffTime = time;
    return ESP_OK;
}

esp_err_t jsonSetNextPowerOnTime(uint32_t time)
{
    s_deviceRetainedState.nextPowerOnTime = time;
    return ESP_OK;
}

esp_err_t jsonSetNextPowerOffTime(uint32_t time)
{
    s_deviceRetainedState.nextPowerOffTime = time;
    return ESP_OK;
}

esp_err_t jsonSetLastWakeUpTime(uint32_t time)
{
    s_deviceRetainedState.lastWakeUpTime = time;
    return ESP_OK;
}

esp_err_t jsonSetLastSleepTime(uint32_t time)
{
    s_deviceRetainedState.lastSleepTime = time;
    return ESP_OK;
}

esp_err_t jsonSetNextSleepTime(uint32_t time)
{
    s_deviceRetainedState.nextSleepTime = time;
    return ESP_OK;
}

esp_err_t jsonSetDeviceState(uint32_t state)
{
    s_deviceRetainedState.deviceState = state;
    return ESP_OK;
}

esp_err_t getDeviceRetainedState(char *stateData)
{
    cJSON *jsonData = NULL;
    char *data;

    data = pvPortMalloc(1024);

    jsonData = cJSON_CreateObject();

    cJSON_AddNumberToObject(jsonData, JSON_KEY_POWER, s_deviceRetainedState.powerState);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_LAST_POWER_ON_TIME, s_deviceRetainedState.lastPowerOnTime);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_LAST_POWER_OFF_TIME, s_deviceRetainedState.lastPowerOffTime);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_NEXT_POWER_ON_TIME, s_deviceRetainedState.nextPowerOnTime);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_NEXT_POWER_OFF_TIME, s_deviceRetainedState.nextPowerOffTime);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_LAST_WAK_UP_TIME, s_deviceRetainedState.lastWakeUpTime);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_LAST_SLEEP_TIME, s_deviceRetainedState.lastSleepTime);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_NEXT_WAKE_UP_TIME, s_deviceRetainedState.nextWakeUpTime);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_NEXT_SLEEP_TIME, s_deviceRetainedState.nextSleepTime);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_DEVICE_STATE, s_deviceRetainedState.deviceState);

    data = cJSON_Print(jsonData);

    strcpy(stateData,data);

    vPortFree(data);

    return ESP_OK;
}

esp_err_t jsonGenerateAppRetainedSettings(char *stateData)
{
    cJSON *jsonData = NULL;
    char *data;

    data = pvPortMalloc(1024);

    jsonData = cJSON_CreateObject();

    cJSON_AddNumberToObject(jsonData, JSON_KEY_WAKE_UP_INTERVAL, s_appRetainedSettings.wakeupInterval);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_POWER_ON_OFF, s_appRetainedSettings.powerCtrl);
    cJSON_AddStringToObject(jsonData, JSON_KEY_PC_PASSWORD, s_appRetainedSettings.password);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_PC_PASSWORD_CTRL, s_appRetainedSettings.passwordCtrl);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_PC_PASSWORD_WAIT, s_appRetainedSettings.passwordWait);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_LED_CTRL, s_appRetainedSettings.ledCtrl);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_TOOLS_TOKEN, s_appRetainedSettings.toolsToken);

    data = cJSON_Print(jsonData);

    strcpy(stateData,data);

    vPortFree(data);

    return ESP_OK;
}

esp_err_t setDeviceRetainedStatistics(DeviceRetainedStatisticsStruct statistics)
{
    s_deviceRetainedStateStatistics.powerOnCount = statistics.powerOnCount;
    s_deviceRetainedStateStatistics.powerOffCount = statistics.powerOffCount;
    s_deviceRetainedStateStatistics.wakeUpCount = statistics.wakeUpCount;

    return ESP_OK;
}

esp_err_t getDeviceRetainedStatistics(char *statisticsData)
{
    cJSON *jsonData = NULL;
    char *data;

    jsonData = cJSON_CreateObject();

    cJSON_AddNumberToObject(jsonData, JSON_KEY_POWER_ON_COUNT, s_deviceRetainedStateStatistics.powerOnCount);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_POWER_OFF_COUNT, s_deviceRetainedStateStatistics.powerOffCount);
    cJSON_AddNumberToObject(jsonData, JSON_KEY_WAKE_UP_COUNT, s_deviceRetainedStateStatistics.wakeUpCount);

    data = cJSON_Print(jsonData);

    strcpy(statisticsData,data);

    return ESP_OK;
}

esp_err_t setDeviceDisretainedStatistics(DeviceDisretainedStatisticsStruct statistics)
{
    s_deviceDisretainedStatistics.voltage = statistics.voltage;

    return ESP_OK;
}

esp_err_t getDeviceDisretainedStateStatistics(char *statisticsData)
{
    cJSON *jsonData = NULL;
    char *data;

    jsonData = cJSON_CreateObject();

    cJSON_AddNumberToObject(jsonData, JSON_KEY_VOLTAGE, s_deviceDisretainedStatistics.voltage);

    data = cJSON_Print(jsonData);

    strcpy(statisticsData,data);

    return ESP_OK;
}