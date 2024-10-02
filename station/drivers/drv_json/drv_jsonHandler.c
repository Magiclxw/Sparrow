#include "drv_jsonHandler.h"
#include "drv_nvs.h"

static AppRetainedSettingsStruct s_appRetainedSettings = {0};
static AppDisretainedSettingsStruct s_appDisretainedSettings = {0};
static DeviceRetainedStateStruct s_deviceRetainedState = {0};
static DeviceRetainedStatisticsStruct s_deviceRetainedStateStatistics = {0};
static DeviceDisretainedStatisticsStruct s_deviceDisretainedStatistics = {0};


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

    s_appRetainedSettings.wakeupInterval = jsonWakeupInterval->valueint;
    s_appRetainedSettings.powerCtrl = jsonPowerCtrl->valueint;
    strcpy(s_appRetainedSettings.password, jsonPCPassword->valuestring);
    s_appRetainedSettings.passwordCtrl = jsonPCPasswordCtrl->valueint;
    s_appRetainedSettings.passwordWait = jsonPCPasswordWait->valueint;
    s_appRetainedSettings.ledCtrl = jsonLed->valueint;
    s_appRetainedSettings.toolsToken = jsonToken->valueint;

    //保存配置
    nvsSaveValue(USER_NAMESPACE_0, NVS_READWRITE, JSON_KEY_TOOLS_TOKEN, s_appRetainedSettings.toolsToken);

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

    s_appDisretainedSettings.turnAngle = jsonTurnAngle->valueint;
    s_appDisretainedSettings.saveAngle = jsonSaveAngle->valueint;

    cJSON_Delete(jsonData);

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
    
    return ESP_OK;
}

esp_err_t getDeviceRetainedState(char *stateData)
{
    cJSON *jsonData = NULL;
    char *data;

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

    data = cJSON_Print(jsonData);

    strcpy(stateData,data);

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