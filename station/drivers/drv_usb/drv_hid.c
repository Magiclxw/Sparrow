#include "drv_hid.h"
#include "driver/gpio.h"

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_INOUT_DESC_LEN)

#define APP_BUTTON (GPIO_NUM_0) // Use BOOT signal by default
static const char *TAG = "example";
uint8_t const conv_table[128][2] =  { HID_ASCII_TO_KEYCODE };
/**
 * @brief HID report descriptor
 *
 * In this example we implement Keyboard + Mouse HID device,
 * so we must define both report descriptors
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD) ),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE) ),
    TUD_HID_REPORT_DESC_GENERIC_INOUT(63,HID_REPORT_ID(3)),
    
};
uint8_t const desc_hid_report[] = {
 
    TUD_HID_REPORT_DESC_GENERIC_INOUT(63, HID_REPORT_ID(3))
 
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
};



/**
 * @brief Configuration descriptor
 *
 * This is a simple configuration descriptor that defines 1 configuration and 1 HID interface
 */
static const uint8_t hid_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    //TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),

    TUD_HID_INOUT_DESCRIPTOR(0,0,HID_ITF_PROTOCOL_NONE,sizeof(hid_report_descriptor), 0x01, 0x81, 64, 10),
};

/********* TinyUSB HID callbacks ***************/

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    return hid_report_descriptor;
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
}

/********* Application ***************/

typedef enum {
    MOUSE_DIR_RIGHT,
    MOUSE_DIR_DOWN,
    MOUSE_DIR_LEFT,
    MOUSE_DIR_UP,
    MOUSE_DIR_MAX,
} mouse_dir_t;


void app_send_hid_demo(void)
{
    // Keyboard output: Send key 'a/A' pressed and released
    //ESP_LOGI(TAG, "Sending Keyboard report");
    // uint8_t keycode[6] = {HID_KEY_CAPS_LOCK};
    // tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, keycode);
    // vTaskDelay(pdMS_TO_TICKS(50));
    // tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);

     hid_input_char('b');
    // vTaskDelay(pdMS_TO_TICKS(100));

    //hid_mouse_move(50,0);
    //tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, MOUSE_BUTTON_RIGHT, 0, 0, 0, 0);
    // Mouse output: Move mouse cursor in square trajectory
    //ESP_LOGI(TAG, "Sending Mouse report");
    // int8_t delta_x;
    // int8_t delta_y;
    // for (int i = 0; i < (DISTANCE_MAX / DELTA_SCALAR) * 4; i++) {
    //     // Get the next x and y delta in the draw square pattern
    //     mouse_draw_square_next_delta(&delta_x, &delta_y);
    //     tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0x00, delta_x, delta_y, 0, 0);
    //     vTaskDelay(pdMS_TO_TICKS(20));
    // }
}

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

void hid_data_send(uint8_t data[], uint8_t length)
{
    uint8_t reportData[63] = {0};

    if (tud_hid_ready())
    {
        //memcpy(reportData, data, length);
        tud_hid_report(3,reportData,63);
        
    }
}

/* 鼠标向相对位置移动 */
void hid_mouse_move(uint8_t x,uint8_t y)
{
    tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0x00, x, y, 0, 0);
}

void hid_test(void)
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

    // while (1) {
    //     if (tud_mounted()) {
    //         static bool send_hid_data = true;
    //         if (send_hid_data) {
    //             app_send_hid_demo();
    //         }
    //         send_hid_data = !gpio_get_level(APP_BUTTON);
    //     }
    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }
}