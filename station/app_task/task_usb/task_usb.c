#include "task_usb.h"
#include "../../sys_config.h"
#include "drv_usb.h"
#include "task_bluetooth.h"
#include "drv_nvs.h"
#include "drv_http.h"
#include "ui.h"
#include "drv_led.h"
#include "wifi_station.h"
#include "drv_mqtt.h"
#include "systeminfo.h"
#include "task_display.h"

TaskHandle_t usbTaskHandle = NULL;

static uint8_t *s_usbDataBackground = NULL;
static uint16_t s_imageSize = 0;
static uint16_t s_imageCount = 0;

static void usbTask();
static void usbDataHandler(uint8_t data[]);
static esp_err_t dataCheck(uint8_t data[]);
static esp_err_t compareCheckSum(uint8_t data[] ,uint16_t length);

int createUsbTask()
{
    xTaskCreate((TaskFunction_t)usbTask,
                (const char*)"usbTask",
                (uint32_t )USB_TASK_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)USB_TASK_PRIORITY,
                &usbTaskHandle);
    return OPERATE_SUCCESS;
}

static void usbTask()
{
    uint8_t recData[100] = { 0 };

    Usb_Queue_Handle = xQueueCreate(USB_REC_QUEUE_LENGTH,USB_REC_QUEUE_SIZE);

    intiUsb();

    while(1)
    {
        if(Usb_Queue_Handle != NULL)
        {
            xQueueReceive(Usb_Queue_Handle,recData,portMAX_DELAY);

            usbDataHandler(recData);
        }
    }
}

static esp_err_t dataCheck(uint8_t data[])
{
    if (data[0] == USB_PROTOCOL_START_H && data[1] == USB_PROTOCOL_START_L)
    {
        uint16_t dataLen = data[3] << 8 | data[4];

        if (data[dataLen + BLE_PROTOCOL_START_LEN + BEL_PROTOCOL_CMD_LEN + BLE_PROTOCOL_DATALEN_LEN + BLE_PROTOCOL_CHECKSUM_LEN + 1] == USB_PROTOCOL_STOP_H 
        && data[dataLen + BLE_PROTOCOL_START_LEN + BEL_PROTOCOL_CMD_LEN + BLE_PROTOCOL_DATALEN_LEN + BLE_PROTOCOL_CHECKSUM_LEN + 2] == USB_PROTOCOL_STOP_L )
        {
            if (compareCheckSum(data, dataLen + 5) == ESP_OK)
            {
                return ESP_OK;
            }
            else
            {
                return ESP_FAIL;
            }
        }
    }
    return ESP_FAIL;
}

static void usbDataHandler(uint8_t data[])
{
    if (dataCheck(data) == ESP_OK)
    {
        uint8_t cmd = data[2];
        uint8_t* dataPtr = &data[5];
        uint16_t dataLen = data[3] << 8 | data[4];

        switch (cmd)
        {
            case USB_PROTOCOL_CMD_TEXT_START:
            case USB_PROTOCOL_CMD_TEXT:
            case USB_PROTOCOL_CMD_FILE_START:
            case USB_PROTOCOL_CMD_FILE:
            {
                xQueueSend(bleTransQueueHandle,data,0);
                break;
            }
            case USB_PROTOCOL_CMD_SET_BACKGROUND_START:
            {
                uint8_t ack = 0x01;
                uint16_t s_imageSize = data[5] << 8 | data[6];
                if (s_usbDataBackground != NULL)
                {
                    free(s_usbDataBackground);
                    s_usbDataBackground = NULL;
                }
                s_usbDataBackground = (uint8_t*) malloc(s_imageSize);
                cdcSendProtocol(USB_PROTOCOL_CMD_SET_BACKGROUND_START, &ack, 1);
                break;
            }
            case USB_PROTOCOL_CMD_SET_BACKGROUND:
            {
                if (s_usbDataBackground != NULL)
                {
                    //uint16_t frameIndex = data[5] << 8 | data[6];
                    uint8_t response[3] = {data[5], data[6], 0x01};
                    uint16_t frameLength = data[7] << 8 | data[8];
                    
                    memcpy(&s_usbDataBackground[s_imageCount], &data[9], frameLength);

                    s_imageCount += frameLength;
                    //数据接收完成
                    if (s_imageCount == s_imageSize)
                    {
                        nvsSaveBlobData(USER_NAMESPACE_0, NVS_READWRITE, NVS_KEY_BACKGROUND, s_usbDataBackground, s_imageCount);
                        s_imageCount = 0;
                        s_imageSize = 0;
                        free(s_usbDataBackground);
                        s_usbDataBackground = NULL;
                    }

                    cdcSendProtocol(USB_PROTOCOL_CMD_SET_BACKGROUND, response, 3);
                }
                break;
            }
            case USB_PROTOCOL_CMD_DISK_INFO:
            {
                uint8_t diskNum = data[5];
                ui_Screen_Main_DrawDiskInfoBar(diskNum, &data[6]);
                break;
            }
            case USB_PROTOCOL_CMD_SYSTEM_INFO:
            {
                // static uint8_t color = 0;
                // uint8_t memery = data[5];
                // uint8_t cpu = data[6];
                // uint64_t daownLoadSpeed;
                // uint64_t uploadSpeed;
                // memcpy(&daownLoadSpeed, &data[7], sizeof(daownLoadSpeed));
                // memcpy(&uploadSpeed, &data[15], sizeof(uploadSpeed));
                // if (color == 0)
                // {
                //     setLed(LED_BLUE);
                //     color = 1;
                // }
                // else
                // {
                //     setLed(LED_RED);
                //     color = 0;
                // }
                
                // ui_Screen_Main_SetMeter(cpu, memery);
                // ui_Screen_Main_SetNetSpeed(daownLoadSpeed, uploadSpeed);

                xQueueSend(lcdQueueHandle, &data[5], 0);

                break;
            }
            case USB_PROTOCOL_CMD_CLEAR_WIFI_INFO:
            {
                clearWifiData();
                setLed(LED_BLUE);
                break;
            }
            case USB_PROTOCOL_CMD_SET_WIFI_INFO:
            {
                uint8_t ssidLen = data[5];
                uint8_t pwdLen = data[5 + ssidLen + 1];
                char ssid[ssidLen];
                char pwd[pwdLen];
                memcpy(ssid, &data[6], ssidLen);
                memcpy(pwd, &data[6 + ssidLen + 1], pwdLen);

                esp_err_t ret = nvsOpen(USER_NAMESPACE_0, NVS_READWRITE);

                ret = nvsSetStr("ssid",(char*)ssid);

                if(ret == ESP_OK)
                {
                    ret = nvsSetStr("password",(char*)pwd);

                    ret = nvsCommit();

                    nvsClose();

                    if(ret == ESP_OK)
                    {
                        printf("wifi message store success!");
                        setLed(LED_RED);
                    }
                }
                break;
            }
            case USB_PROTOCOL_CMD_SET_MQTT_INFO:
            {
                uint8_t dataType = data[5];
                uint8_t dataLength = data[6];

                // printf("data len = %d\r\n",dataLength);

                char cmbData[dataLength];

                memcpy(cmbData, &data[7], dataLength);

                // printf("addr = %s, username = %s, password = %s\r\n", addr, username, password);
                //for (int i = 0; i < dataLength; i++)
                // {
                //     printf("%s\r\n", cmbData);
                // }
                
                if (dataType == MQTT_DATA_ADDR)
                {
                    mqttSetBrokerAddr(cmbData);
                }
                else if (dataType == MQTT_DATA_USERNAME)
                {
                    mqttSetBrokerUsername(cmbData);
                }
                else
                {
                    mqttSetBrokerPassword(cmbData);
                }
                
                setLed(LED_RED);
                break;
            }
            case USB_PROTOCOL_CMD_SET_WEATHER_URL:
            {
                uint8_t weatherUrlLen = data[5];

                char weatherData[weatherUrlLen];

                memcpy(weatherData, &data[6], weatherUrlLen);

                nvsSetWeatherUrl(weatherData);

                setLed(LED_GREEN);
                break;
            }
            case USB_PROTOCOL_CMD_SET_BILIBILI_URL:
            {
                uint8_t biliBiliUrlLen = data[5];

                char biliBiliData[biliBiliUrlLen];

                memcpy(biliBiliData, &data[6], biliBiliUrlLen);

                nvsSetBilibiliUrl(biliBiliData);

                setLed(LED_GREEN);
                break;
            }
            case USB_PROTOCOL_CMD_SET_SLEEP_TIME:
            {
                uint32_t sleepTime = 0;

                memcpy(&sleepTime, &data[5], sizeof(sleepTime));

                sysSetSleepTime(sleepTime);

                setLed(LED_GREEN);

                break;
            }
            // 设置蓝牙名称
            case USB_PROTOCOL_CMD_SET_BEL_NAME:
            {
                uint8_t bleNameLen = data[5];

                char bleNameData[bleNameLen];

                memcpy(bleNameData, &data[6], bleNameLen);

                nvsSaveBleName(bleNameData);
                break;
            }
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

    if (checksum == data[length])
    {
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}