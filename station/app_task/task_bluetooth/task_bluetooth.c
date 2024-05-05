#include "task_bluetooth.h"
#include "esp_log.h"
#include "../../sys_config.h"

TaskHandle_t Bluetooth_Task__Handle = NULL;


const static char *TAG = "task_bluetooth";

static void Bluetooth_Task();

int Bluetooth_Task_Create()
{
    xTaskCreate((TaskFunction_t)Bluetooth_Task,
                (const char*)"Bluetooth_Task",
                (uint32_t )BLUETOOTH_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)BLUETOOTH_TASK_PRIORITY,
                &Bluetooth_Task__Handle);
    return OPERATE_SUCCESS;
}

void Bluetooth_Task()
{
    BleData_t bleData;

    Bluetooth_Queue_Handle = xQueueCreate(BLUETOOTH_REC_QUEUE_LENGTH,BLUETOOTH_REC_QUEUE_SIZE);

    while (1)
    {
        if(Bluetooth_Queue_Handle != NULL)
        {
            xQueueReceive(Bluetooth_Queue_Handle,&bleData,portMAX_DELAY);
            ESP_LOGI(TAG, "ble_task rec : %d",bleData.length);
        }

    }
    
}

