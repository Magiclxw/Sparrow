#include "startup.h"
#include "task_led.h"
#include "wifi_station.h"
#include "driver_mqtt.h"
#include "drv_nvs.h"
#include "task_bluetooth.h"
#include "task_servo.h"
#include "task_rtc.h"
#include "task_battery.h"

SemaphoreHandle_t preStartupSemaphore;

void preStartup()
{
    preStartupSemaphore = xSemaphoreCreateBinary();

    initNvs();
    //clearWifiData();
    //wifi初始化
    initialise_wifi();
    //mqtt初始化，建立mqtt连接
    mqtt_app_start();

    xSemaphoreTake(preStartupSemaphore,portMAX_DELAY);
}

void midStartup()
{
    LED_Task_Create();
}

void postStartup()
{
    Bluetooth_Task_Create();

    //启动舵机线程
    Servo_Control_TASK_Create();
    //创建电源线程
    Battery_Task_Create();

    //启动RTC线程
    ////Rtc_Task_Create();
}