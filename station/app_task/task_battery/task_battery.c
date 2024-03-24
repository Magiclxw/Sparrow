#include "task_battery.h"
#include "../../drivers/drv_adc/drv_adc.h"
#include "driver_mqtt.h"
#include "../../sys_config.h"

TaskHandle_t Battery_Task__Handle = NULL;

const static char *TAG = "battery";

static void Battery_Task();

int Battery_Task_Create()
{
    xTaskCreate((TaskFunction_t)Battery_Task,
                (const char*)"Battery_Task",
                (uint32_t )BATTERY_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)BATTERY_TASK_PRIORITY,
                &Battery_Task__Handle);
    return OPERATE_SUCCESS;
}

static void Battery_Task()
{
    int adc_raw = 0;
    int battery_voltage = 0;    //电压(mV)
    BATTERY_STATE_e battery_state = BATTERY_STATE_UNKNOWN;
    char str_voltage[2];
    Battery_Adc_Init();
    while(1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, BATTERY_ADC_CHANNEL, &adc_raw));
        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, BATTERY_ADC_CHANNEL, adc_raw);
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_chan3_handle, adc_raw, &battery_voltage));
        //battery_voltage = adc_raw*3300.0f/4096;
        ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, BATTERY_ADC_CHANNEL, battery_voltage);

        itoa(battery_voltage,str_voltage,10);

        

        //判断电源电压
        if(battery_voltage <= 1400)  //电压过低(2800mV)
        {
            if(client != NULL)
            {
                battery_state = BATTERY_STATE_LOW_VOL;

                itoa(battery_state,str_voltage,10);

                esp_mqtt_client_publish(client, "/status/power", str_voltage, 0, 0, 0);
            }
            
        }
        //else if(battery_voltage <= 2100)  //电池供电状态(4200mV)
        //改为通过判断TP4056引脚判断状态
        else if((GetChargeState() == 1) && (GetStandbyState() == 1))
        {
            if(client!= NULL)
            {
                battery_state = BATTERY_STATE_BATTERY;

                itoa(battery_state,str_voltage,10);

                esp_mqtt_client_publish(client, "/status/power", str_voltage, 0, 0, 0);
            }
            
            vTaskDelay(pdMS_TO_TICKS(DEFAULT_RUNTIME));
            printf("sleep");
            esp_deep_sleep(1000000LL * DEFAULT_DEEP_SLEEP_SEC);
        }
        else if(battery_voltage <= 2600 && battery_voltage >= 2400) //usb供电状态(5V)
        {
            if(client!= NULL)
            {
                battery_state = BATTERY_STATE_USB;

                itoa(battery_state,str_voltage,10);

                esp_mqtt_client_publish(client, "/status/power", str_voltage, 0, 0, 0);
            }
            vTaskDelay(1000);
        }
        else
        {
            //异常状态
            if(client!= NULL)
            {
                battery_state = BATTERY_STATE_ERROR;

                itoa(battery_state,str_voltage,10);

                esp_mqtt_client_publish(client, "/status/power", str_voltage, 0, 0, 0);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}