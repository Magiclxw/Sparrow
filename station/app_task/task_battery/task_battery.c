#include "task_battery.h"
#include "../../drivers/drv_adc/drv_adc.h"
#include "drv_mqtt.h"
#include "../../sys_config.h"
#include "drv_power.h"
#include "drv_nvs.h"

TaskHandle_t Battery_Task__Handle = NULL;

const static char *TAG = "battery";

static void batteryTask();

int createBatteryTask()
{
    xTaskCreate((TaskFunction_t)batteryTask,
                (const char*)"batteryTask",
                (uint32_t )BATTERY_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)BATTERY_TASK_PRIORITY,
                &Battery_Task__Handle);
    return OPERATE_SUCCESS;
}

static void batteryTask()
{
    int adc_raw = 0;
    int battery_voltage = 0;    //电压(mV)
    uint64_t wakeupInterval = 0;    // 唤醒间隔
    BATTERY_STATE_e battery_state = BATTERY_STATE_UNKNOWN;
    BATTERY_STATE_e lastBatteryState = BATTERY_STATE_MAX;
    char str_voltage[4];
    char strNextWakeupTime[100];
    drvAdcInitAdc();
    drvPowerInitBatterySignal();
    while(1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, BATTERY_ADC_CHANNEL, &adc_raw));
        // ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, BATTERY_ADC_CHANNEL, adc_raw);
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_chan3_handle, adc_raw, &battery_voltage));
        //battery_voltage = adc_raw*3300.0f/4096;
        ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, BATTERY_ADC_CHANNEL, battery_voltage);

        itoa(battery_voltage,str_voltage,10);

        if(client != NULL)
        {
            //判断电源电压
            if(battery_voltage <= 1400 && lastBatteryState != battery_state)  //电压过低(2800mV)
            {
                battery_state = BATTERY_STATE_LOW_VOL;
                lastBatteryState = battery_state;

                itoa(battery_state,str_voltage,10);

                while (esp_mqtt_client_publish(client, MQTT_TOPIC_DEVICE_RETAINED_STATE, str_voltage, 0, 0, 0) < 0)
                {
                    vTaskDelay(pdMS_TO_TICKS(1000));
                }
            }
            //else if(battery_voltage <= 2100)  //电池供电状态(4200mV)
            //改为通过判断TP4056引脚判断状态
            // 电池供电时，连接上服务器更新状态后进入睡眠模式
            else if((drvPowergetChargeState() == 1) && (drvPowergetStandbyState() == 1) && lastBatteryState != battery_state)
            {
                battery_state = BATTERY_STATE_BATTERY;
                lastBatteryState = battery_state;

                itoa(battery_state,str_voltage,10);
                // 最多尝试10次
                for (int i = 0; i < 10; i++)
                {
                    if (esp_mqtt_client_publish(client, MQTT_TOPIC_DEVICE_RETAINED_STATE, str_voltage, 0, 0, 0) >= 0)
                    {
                        break;
                    }
                }
                // 待机60s
                vTaskDelay(pdMS_TO_TICKS(DEFAULT_RUNTIME));

                uint64_t nextWakeupTime = drvPowerGetNextWakeupTime();

                itoa(nextWakeupTime, strNextWakeupTime, 10);

                esp_mqtt_client_publish(client, MQTT_TOPIC_DEVICE_RETAINED_STATE, strNextWakeupTime, 0, 0, 1);
                printf("sleep");
                if (nvsLoadWakeupInterval(&wakeupInterval) == ESP_OK)
                {
                    esp_deep_sleep(1000000LL * wakeupInterval);
                }
                else
                {
                    // 默认睡眠时间 s
                    esp_deep_sleep(1000000LL * DEFAULT_DEEP_SLEEP_SEC);
                }
                
                
            }
            else if(((battery_voltage <= 2600 && battery_voltage >= 2400)
                        // 唤醒引脚状态
                    || drvPowerGetWakeupLevel()) && lastBatteryState != battery_state) //usb供电状态(5V)
            {
                battery_state = BATTERY_STATE_USB;
                lastBatteryState = battery_state;

                itoa(battery_state,str_voltage,10);
                
                while (esp_mqtt_client_publish(client, MQTT_TOPIC_DEVICE_RETAINED_STATE, str_voltage, 0, 0, 0) < 0)
                {
                    vTaskDelay(pdMS_TO_TICKS(1000));
                }
            }
            else if (lastBatteryState != battery_state)
            {
                //异常状态
                battery_state = BATTERY_STATE_ERROR;
                lastBatteryState = battery_state;

                itoa(battery_state,str_voltage,10);

                while (esp_mqtt_client_publish(client, MQTT_TOPIC_DEVICE_RETAINED_STATE, str_voltage, 0, 0, 0) < 0)
                {
                    vTaskDelay(pdMS_TO_TICKS(1000));
                }
            }
        }

        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}