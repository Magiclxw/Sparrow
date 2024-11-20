#include "task_bluetooth.h"
#include "esp_log.h"
#include "drv_usb.h"
#include "drv_led.h"
#include "drv_nvs.h"
#include "../../sys_config.h"
#include "drv_mqtt.h"

TaskHandle_t Bluetooth_Task__Handle = NULL;
TaskHandle_t bleTaskHandle = NULL;

static uint8_t hidData[63] = {0};
static uint8_t transData[1024] = {0};

const static char *TAG = "task_bluetooth";

static void bluetoothRecTask();
static void bluetoothRecDataHandler(uint8_t *data);
static void bluetoothTransTask();
static void bluetoothTransDataHandler(uint8_t *data);
static esp_err_t compareCheckSum(uint8_t data[] ,uint16_t length);
static esp_err_t dataCheck(uint8_t data[]);

int createBleRecTask()
{
    xTaskCreate((TaskFunction_t)bluetoothRecTask,
                (const char*)"bluetoothRecTask",
                (uint32_t )BLUETOOTH_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)BLUETOOTH_TASK_PRIORITY,
                &Bluetooth_Task__Handle);
    return OPERATE_SUCCESS;
}

int createBleTransTask()
{
    xTaskCreate((TaskFunction_t)bluetoothTransTask,
                (const char*)"bluetoothTransTask",
                (uint32_t )BLUETOOTH_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)BLUETOOTH_TASK_PRIORITY,
                &bleTaskHandle);
    return OPERATE_SUCCESS;
}

void bluetoothRecTask()
{
    BleData_t bleData;
    uint8_t recData[1024] = {0};

    //创建队列
    Bluetooth_Queue_Handle = xQueueCreate(BLUETOOTH_REC_QUEUE_LENGTH,BLUETOOTH_REC_QUEUE_SIZE);

    while (1)
    {
        
        if(Bluetooth_Queue_Handle != NULL)
        {
            xQueueReceive(Bluetooth_Queue_Handle,recData,portMAX_DELAY);
            //ESP_LOGI(TAG, "ble_task rec : %d",bleData.length);
            //打印数据
            //esp_log_buffer_hex(TAG, bleData.data,bleData.length);
            bluetoothRecDataHandler(recData);
        }

    }
    
}

static void bluetoothTransTask()
{
    BleTransDataStruct transData;

    uint8_t recdata[1024];

    bleTransQueueHandle = xQueueCreate(BLUETOOTH_TRANS_QUEUE_LENGTH,BLUETOOTH_TRANS_QUEUE_SIZE);

    uint8_t bleData[5] = {0,1,2,3,4};

    while (1)
    {
        if(bleTransQueueHandle != NULL)
        {
            xQueueReceive(bleTransQueueHandle,recdata,portMAX_DELAY);

            bleSendProtocol(recdata);
        }
    }
}

static esp_err_t compareCheckSum(uint8_t data[] ,uint16_t length)
{
    uint8_t checksum = 0;

    for (uint16_t i = 0; i < length; i++)
    {
        checksum += data[i];
    }

    ESP_LOGI(TAG, " rec checksum = %d, calc checksum = %d\r\n", checksum, data[length]);

    if (checksum == data[length])
    {
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}

static esp_err_t dataCheck(uint8_t data[])
{
    if (data[0] == BLE_PROTOCOL_START_H && data[1] == BLE_PROTOCOL_START_L)
    {
        uint16_t dataLen = data[3] << 8 | data[4];

        if (data[dataLen + BLE_PROTOCOL_START_LEN + BEL_PROTOCOL_CMD_LEN + BLE_PROTOCOL_DATALEN_LEN + BLE_PROTOCOL_CHECKSUM_LEN + 1] == BLE_PROTOCOL_STOP_H 
        && data[dataLen + BLE_PROTOCOL_START_LEN + BEL_PROTOCOL_CMD_LEN + BLE_PROTOCOL_DATALEN_LEN + BLE_PROTOCOL_CHECKSUM_LEN + 2] == BLE_PROTOCOL_STOP_L )
        {
            if (compareCheckSum(data, dataLen + 5) == ESP_OK)
            {
                // ESP_LOGI(TAG, " compare ok\r\n");
                return ESP_OK;
            }
            else
            {
                // ESP_LOGI(TAG, " compare fail\r\n");
                return ESP_FAIL;
            }
        }
    }
    return ESP_FAIL;
}

/*
* @brief 处理接收到的数据
* @param[in] data : [0]     [1]     [2]     [3]     [4]         [5]...
*                  Start_H Start_L  CMD  DataLen_H DataLen_L    Data
*/
static void bluetoothRecDataHandler(uint8_t *data)
{
    uint8_t cmd = data[2];
    uint16_t dataLen = data[3] << 8 | data[4];

    esp_err_t result = dataCheck(data);

    if(result == ESP_FAIL) return;

    switch (cmd)
    {
        //鼠标移动
        case CMD_MOUSE_MOVE:
        {
            hid_mouse_move(data[5],data[6]);
            break;
        }
        //鼠标点击
        case CMD_MOUSE_CLICK:
        {
            hid_mouse_click(data[5],data[6]);
            // ESP_LOGI(TAG, " CLICKED\r\n");
            break;
        }
        //键盘输入
        case CMD_KEYBOARD_INPUT:
        {
            hid_input_char(data[4]);
            // ESP_LOGI(TAG, " key %c\r\n",data[4]);
            break;
        }
        //键盘功能键
        case CMD_KEYBOARD_FUNC:
        {
            hid_input_func(data[5],data[6]);
            // ESP_LOGI(TAG, " key %c\r\n",data[5]);
            break;
        }

        //发送HID数据
        // case CMD_HID_DATA_SEND:
        // {
        //     //ESP_LOGI(TAG, " hid \r\n");
            
        //     for(uint8_t i = 0; i < dataLen; i++)
        //     {
        //         hidData[i] = data[4 + i];
        //     }
        //     hid_data_send(hidData, dataLen);
        //     // ESP_LOGI(TAG, " hid %s\r\n",data);
        //     // ESP_LOGI(TAG, " len %d\r\n",dataLen);
        //     // setLed(1,0,1);
        //     // vTaskDelay(pdMS_TO_TICKS(1000));
        //     // setLed(1,1,1);
        //     break;
        // }
        //配置服务器地址
        case CMD_CFG_SET_SERVER:
        {
            char server_address[100] = {0};

            for(uint16_t i = 0; i < dataLen; i++)
            {
                server_address[i] = data[5 + i];
            }

            ESP_LOGI(TAG,"server_address = %s\n", server_address);

            mqttSetBrokerAddr(server_address);

            break;
        }
        case CMD_CFG_SET_USERNAME:
        {
            char server_username[100] = {0};
            for(uint8_t i = 0; i < dataLen; i++)
            {
                server_username[i] = data[5 + i];
            }

            ESP_LOGI(TAG,"server_username = %s\n", server_username);

            mqttSetBrokerUsername(server_username);
            break;
        }
        case CMD_CFG_SET_PASSWORD:
        {
            char server_password[100] = {0};
            for(uint8_t i = 0; i < dataLen; i++)
            {
                server_password[i] = data[5 + i];
            }

            ESP_LOGI(TAG,"server_password = %s\n", server_password);

            mqttSetBrokerPassword(server_password);
            break;
        }
        case CMD_HID_SEND_TEXT_START:
        {
            for(uint8_t i = 0; i < dataLen; i++)
            {
                transData[i] = data[5 + i];
            }
            cdcSendProtocol(USB_PROTOCOL_CMD_TEXT_START, transData, dataLen);
            //hidSendProtocol(HID_PROTOCOL_CMD_TEXT_START ,hidData, dataLen);
            //hid_data_send(hidData, dataLen);
            //ESP_LOGI(TAG,"hid text = %s\n", &data[5]);
            break;
        }
        case CMD_HID_SEND_TEXT:
        {
            for(uint8_t i = 0; i < dataLen; i++)
            {
                transData[i] = data[5 + i];
            }

            cdcSendProtocol(USB_PROTOCOL_CMD_TEXT, transData, dataLen);
            break;
        }
        case CMD_CDC_SEND_FILE_START:
        {
            for(uint8_t i = 0; i < dataLen; i++)
            {
                transData[i] = data[5 + i];
            }
            cdcSendProtocol(USB_PROTOCOL_CMD_FILE_START, transData, dataLen);
            break;
        }
        case CMD_CDC_SEND_FILE:
        {
            for(uint16_t i = 0; i < dataLen; i++)
            {
                transData[i] = data[5 + i];
            }
            cdcSendProtocol(USB_PROTOCOL_CMD_FILE, transData, dataLen);
            break;
        }
        case CMD_BLE_SET_WEATHER_KEY:
        {
            char biliBiliData[100] = {0};

            for(uint16_t i = 0; i < dataLen; i++)
            {
                biliBiliData[i] = data[5 + i];
            }
            nvsSetBilibiliUrl(biliBiliData);
            break;
        }
        case CMD_BLE_SET_BILIBILI_VMID:
        {
            char weatherData[100] = {0};

            for(uint16_t i = 0; i < dataLen; i++)
            {
                weatherData[i] = data[5 + i];
            }

            nvsSetBilibiliUrl(weatherData);
            break;
        }
    }
}
