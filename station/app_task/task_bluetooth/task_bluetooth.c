#include "task_bluetooth.h"
#include "esp_log.h"
#include "drv_hid.h"
#include "drv_led.h"
#include "drv_nvs.h"
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

    initBLE();
    //创建队列
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
    uint8_t dataLen = data[3];
    switch (cmd)
    {
        //鼠标移动
        case CMD_MOUSE_MOVE:
        {
            hid_mouse_move(data[4],data[5]);
            break;
        }
        //鼠标点击
        case CMD_MOUSE_CLICK:
        {
            //hid_mouse_click(data[4],data[5]);
            ESP_LOGI(TAG, " CLICKED\r\n");
            break;
        }
        //键盘输入
        case CMD_KEYBOARD_INPUT:
        {
            hid_input_char(data[4]);
            ESP_LOGI(TAG, " key %c\r\n",data[4]);
            break;
        }
        //键盘功能键
        case CMD_KEYBOARD_FUNC:
        {
            hid_input_func(data[4],data[5]);
            ESP_LOGI(TAG, " key %c\r\n",data[4]);
            break;
        }
        //发送HID数据
        case CMD_HID_DATA_SEND:
        {
            //ESP_LOGI(TAG, " hid \r\n");
            hid_data_send(data[4], dataLen);
            setLed(1,0,1);
            vTaskDelay(pdMS_TO_TICKS(1000));
            setLed(1,1,1);
            break;
        }
        //配置服务器地址
        case CMD_CFG_SET_SERVER:
        {
            char server_address[100] = {0};

            for(uint16_t i = 0; i < dataLen; i++)
            {
                server_address[i] = data[4 + i];
            }

            ESP_LOGI(TAG,"server_address = %s\n", server_address);

            // esp_err_t ret = nvsOpen(USER_NAMESPACE_0, NVS_READWRITE);

            // if(ret == ESP_OK)
            // {
            //     ret = nvsSetStr(SERVER_ADDRESS, server_address);

            //     ret = nvsCommit();

            //     nvsClose();
            // }

            break;
        }
        case CMD_CFG_SET_USERNAME:
        {
            char server_username[100] = {0};
            for(uint8_t i = 0; i < dataLen; i++)
            {
                server_username[i] = data[4 + i];
            }

            ESP_LOGI(TAG,"server_username = %s\n", server_username);

            break;
        }
        case CMD_CFG_SET_PASSWORD:
        {
            char server_password[100] = {0};
            for(uint8_t i = 0; i < dataLen; i++)
            {
                server_password[i] = data[4 + i];
            }

            ESP_LOGI(TAG,"server_password = %s\n", server_password);
            break;
        }
    }
}



