#ifndef __DRV_JSON_HANDLER_H__
#define __DRV_JSON_HANDLER_H__

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "esp_err.h"
#include <string.h>
#include "cJSON.h"

#define JSON_KEY_WAKE_UP_INTERVAL           "wakeup_interval"
#define JSON_KEY_POWER_ON_OFF               "power_on_off"
#define JSON_KEY_PC_PASSWORD                "pc_password"
#define JSON_KEY_PC_PASSWORD_CTRL           "pc_password_ctrl"
#define JSON_KEY_PC_PASSWORD_WAIT           "pc_password_wait"
#define JSON_KEY_LED_CTRL                   "led_ctrl"

#define JSON_KEY_TURN_ANGLE                 "turn_angle"
#define JSON_KEY_SAVE_ANGLE                 "save_angle"

#define JSON_KEY_POWER                      "power"
#define JSON_KEY_LAST_POWER_ON_TIME         "last_power_on_time"
#define JSON_KEY_LAST_POWER_OFF_TIME        "last_power_off_time"
#define JSON_KEY_NEXT_POWER_ON_TIME         "next_power_on_time"
#define JSON_KEY_NEXT_POWER_OFF_TIME        "next_power_off_time"
#define JSON_KEY_LAST_WAK_UP_TIME           "last_wakeup_time"
#define JSON_KEY_LAST_SLEEP_TIME            "last_sleep_time"
#define JSON_KEY_NEXT_WAKE_UP_TIME          "next_wake_up_time"
#define JSON_KEY_NEXT_SLEEP_TIME            "next_sleep_time"

#define JSON_KEY_POWER_ON_COUNT             "power_on_count"
#define JSON_KEY_POWER_OFF_COUNT            "power_off_count"
#define JSON_KEY_WAKE_UP_COUNT              "wake_up_count"

#define JSON_KEY_VOLTAGE                    "voltage"

typedef enum powerCtrlEnum
{
    POWER_NO_CTRL,
    POWER_ON,
    POWER_OFF
}PowerCtrlEnum;

typedef enum powerStateEnum
{
    POWER_STATE_UNKNOWN,
    POWER_STATE_USB,
    POWER_STATE_BATTERY,
    POWER_STATE_LOW_VOLTAGE,
    POWER_STATE_ERROR,
}PowerStateEnum;

typedef struct appRetainedSettingsStruct
{
    uint16_t wakeupInterval;    //唤醒间隔
    PowerCtrlEnum powerCtrl;   //开关控制
    char password[100];         //电脑开机密码
    uint8_t passwordCtrl;      //电脑开机密码控制
    uint16_t passwordWait;     //电脑开机密码等待时间
    uint8_t ledCtrl;            //led控制
}AppRetainedSettingsStruct;

typedef struct appDisretainedSettingsStruct
{
    char turnAngle;     //舵机转动角度
    uint8_t saveAngle;  //保存舵机角度
}AppDisretainedSettingsStruct;

typedef struct deviceRetainedSettingsStruct
{

}DeviceRetainedSettingsStruct;

typedef struct deviceDisretainedSettingsStruct
{

}DeviceDisretainedSettingsStruct;

typedef struct deviceRetainedStateStruct
{
    PowerStateEnum powerState; //电源状态
    uint32_t lastPowerOnTime;
    uint32_t lastPowerOffTime;
    uint32_t nextPowerOnTime;
    uint32_t nextPowerOffTime;
    uint32_t lastWakeUpTime;
    uint32_t lastSleepTime;
    uint32_t nextWakeUpTime;
    uint32_t nextSleepTime;

}DeviceRetainedStateStruct;

typedef struct deviceDisretainedStateStruct
{

}DeviceDisretainedStateStruct;

typedef struct deviceRetainedStatisticsStruct
{
    uint32_t powerOnCount; //开机次数
    uint32_t powerOffCount; //关机次数
    uint32_t wakeUpCount;   //唤醒次数
}DeviceRetainedStatisticsStruct;

typedef struct deviceDisretainedStatisticsStruct
{
    uint16_t voltage;   //当前电压
}DeviceDisretainedStatisticsStruct;

esp_err_t setAppRetainedSettings(char *data);
AppRetainedSettingsStruct getAppRetainedSettings();
esp_err_t setAppDisretainedSettings(char *data);
AppDisretainedSettingsStruct getAppDisretainedSettings();
esp_err_t setDeviceRetainedState(DeviceRetainedStateStruct state);
esp_err_t getDeviceRetainedState(char *data);
esp_err_t setDeviceRetainedStatistics(DeviceRetainedStatisticsStruct statistics);
esp_err_t getDeviceRetainedStatistics(char *data);
esp_err_t setDeviceDisretainedStatistics(DeviceDisretainedStatisticsStruct statistics);
esp_err_t getDeviceDisretainedStateStatistics(char *data);

#endif