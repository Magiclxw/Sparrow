#include "drv_usb.h"
#include "driver/gpio.h"
#include "drv_ble.h"
#include "drv_led.h"
#include "drv_nvs.h"
#include "task_bluetooth.h"

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_INOUT_DESC_LEN)

#define APP_BUTTON (GPIO_NUM_0) // Use BOOT signal by default
static const char *TAG = "example";

static uint8_t *s_usbDataBackground = NULL;
static uint16_t s_imageSize = 0;
static uint16_t s_imageCount = 0;

uint8_t const conv_table[128][2] =  { HID_ASCII_TO_KEYCODE };
/**
 * @brief HID report descriptor
 *
 * In this example we implement Keyboard + Mouse HID device,
 * so we must define both report descriptors
 */
const uint8_t hid_report_descriptor0[] = {
    //键盘
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD) ),
    //鼠标
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE) ),
    //标准hid设备
    TUD_HID_REPORT_DESC_GENERIC_INOUT(63,HID_REPORT_ID(3)),
};


/**
 * @brief String descriptor
 */
const char* hid_string_descriptor[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "TinyUSB",             // 1: Manufacturer
    "TinyUSB Device",      // 2: Product
    "123456",              // 3: Serials, should use chip ID
    "Example HID interface",  // 4: HID
    "cdc", // 5:
};

/**
 * @brief Configuration descriptor
 *
 * This is a simple configuration descriptor that defines 1 configuration and 1 HID interface
 */
static const uint8_t hid_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 3, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_CDC_DESCRIPTOR(0, 5, 0x81, 8, 0x01, 0x82, 64),
    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_INOUT_DESCRIPTOR(2,4,HID_ITF_PROTOCOL_NONE,sizeof(hid_report_descriptor0), 0X04, 0x84, CFG_TUD_HID_EP_BUFSIZE, 10),
};

static uint8_t hidGenerateChecksum(uint8_t data[], uint16_t len);
static void hidReceiveProtocol(uint8_t data[], uint8_t len);
static esp_err_t compareCheckSum(uint8_t data[] ,uint16_t length);
static esp_err_t dataCheck(uint8_t data[]);
static void usbDataHandler(uint8_t data[]);

/********* TinyUSB HID callbacks ***************/

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    //We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    return hid_report_descriptor0;
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    //第一个字节是report id， 从第二个字节开始发送
    hidReceiveProtocol(&buffer[1],bufsize);

    // if(report_id == 0)
    // {
    //     setLed(0,1,1);
    //     vTaskDelay(pdMS_TO_TICKS(500));
    //     setLed(1,0,1);
    // }
    
}

void tud_cdc_rx_cb(uint8_t itf)
{
    uint8_t readBuffer[1024] = {0};
    //uint8_t rxData = 0;

    //readBuffer = tud_cdc_read_char();

    tud_cdc_read(readBuffer, 1024);
    //tud_cdc_read_flush();
    //xQueueSend(bleTransQueueHandle,readBuffer,0);
    usbDataHandler(readBuffer);
    // setLed(0,1,1);
    // vTaskDelay(pdMS_TO_TICKS(500));
    // setLed(1,0,1);
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
        }
    }
}

void cdcSendProtocol(uint8_t cmd, uint8_t data[], uint16_t len)
{
    static uint8_t uploadData[500] = {0};
    uint8_t checksum = 0;

    if (len > 500) return; 

    uploadData[0] = USB_PROTOCOL_START_H;
    uploadData[1] = USB_PROTOCOL_START_L;
    uploadData[2] = cmd;
    uploadData[3] = len>>8;
    uploadData[4] = (uint8_t)len;
    for(uint16_t i = 0; i < len; i++)
    {
        uploadData[i+5] = data[i];
    }

    checksum = hidGenerateChecksum(uploadData, len+5);

    uploadData[len + 5] = checksum;
    uploadData[len + 5 + 1] = USB_PROTOCOL_STOP_H;
    uploadData[len + 5 + 2] = USB_PROTOCOL_STOP_L;

    //tud_cdc_write_clear();
    tud_cdc_write(uploadData, len+8);
    tud_cdc_write_flush();
    //tud_cdc_write_clear();
}



/********* Application ***************/

typedef enum {
    MOUSE_DIR_RIGHT,
    MOUSE_DIR_DOWN,
    MOUSE_DIR_LEFT,
    MOUSE_DIR_UP,
    MOUSE_DIR_MAX,
} mouse_dir_t;

/* 输入一个ascii字符 */
void hid_input_char(char c)
{
    uint8_t keycode[6] = {0};
    uint8_t modifier = 0;

    if ( conv_table[(uint8_t)c][0] ) modifier = KEYBOARD_MODIFIER_LEFTSHIFT;

    keycode[0] = conv_table[(uint8_t)c][1];

    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, modifier, keycode);

    vTaskDelay(pdMS_TO_TICKS(50));

    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);
}

void hid_input_func(uint8_t modifier,uint8_t keyCode)
{
    uint8_t keycode[6] = {0};

    keycode[0] = keyCode;

    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, modifier, keycode);

    vTaskDelay(pdMS_TO_TICKS(50));

    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);
}

/* 鼠标按键 */
void hid_mouse_click(hid_mouse_button_bm_t button, MouseClickState_e state)
{
    if(state == MouseClickState_Pressed)
    {
        tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, button, 0, 0, 0, 0);
    }
    else if(state == MouseClickState_Released)
    {
        tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0, 0, 0, 0, 0);
    }
    else
    {
        tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, button, 0, 0, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
        tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0, 0, 0, 0, 0);
    }
}

static uint8_t hidGenerateChecksum(uint8_t data[], uint16_t len)
{
    uint8_t checksum = 0;
    for(uint16_t i = 0; i < len; i++)
    {
        checksum += data[i];
    }
    return checksum;
}

void hidSendProtocol(uint8_t cmd, uint8_t data[], uint8_t len)
{
    uint8_t reportData[63] = {0};
    uint8_t checksum = 0;

    reportData[0] = USB_PROTOCOL_START_H;
    reportData[1] = USB_PROTOCOL_START_L;
    reportData[2] = cmd;
    reportData[3] = len;
    for(uint8_t i = 0; i < len; i++)
    {
        reportData[i+4] = data[i];
    }

    checksum = hidGenerateChecksum(reportData, len+4);

    reportData[59] = checksum;
    reportData[60] = USB_PROTOCOL_START_H;
    reportData[61] = USB_PROTOCOL_START_L;

    if (tud_hid_ready())
    {
        tud_hid_report(3,reportData,63);
    }
}

void hidReceiveProtocol(uint8_t data[], uint8_t len)
{
    if (data[0] == 0xC0 && data[1] == 0x0C)
    {
        if (data[2] == 0x01)
        {
            //bleSendProtocol(CMD_HID_SEND_TEXT_START, &data[3], len);
        }
        else if (data[2] == 0x02)
        {
            //bleSendProtocol(CMD_HID_SEND_TEXT, &data[3], len);
        }

    }
}


/* 鼠标向相对位置移动 */
void hid_mouse_move(uint8_t x,uint8_t y)
{
    tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0x00, x, y, 0, 0);
}

// //保存背景图片
// esp_err_t usbSaveBackground(uint16_t frameIndex, uint16_t data[])
// {

// }

void intiUsb(void)
{
    // Initialize button that will trigger HID reports
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(APP_BUTTON),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));

    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");

}

