#include "task_bluetooth.h"
#include "esp_log.h"
#include "drv_hid.h"
#include "../../sys_config.h"

TaskHandle_t Bluetooth_Task__Handle = NULL;


const static char *TAG = "task_bluetooth";

static void Bluetooth_Task();
static void blueToothDataHandler(uint8_t *data);

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
            //打印数据
            esp_log_buffer_hex(TAG, bleData.data,bleData.length);
            blueToothDataHandler(bleData.data);
        }

    }
    
}

static void blueToothDataHandler(uint8_t *data)
{
    uint8_t cmd = data[2];
    switch (cmd)
    {
        //鼠标移动
        case CMD_MOUSE_MOVE:
        {
            hid_mouse_move(data[3],data[4]);
            break;
        }
        //鼠标点击
        case CMD_MOUSE_CLICK:
        {
            hid_mouse_click(data[3],data[4]);
            ESP_LOGI(TAG, " CLICKED\r\n");
            break;
        }
        //键盘输入
        case CMD_KEYBOARD_INPUT:
        {
            hid_input_char(data[3]);
            ESP_LOGI(TAG, " key %c\r\n",data[3]);
            break;
        }
        case CMD_KEYBOARD_FUNC:
        {
            hid_input_func(data[3]);
            ESP_LOGI(TAG, " key %c\r\n",data[3]);
            break;
        }
    }
}



