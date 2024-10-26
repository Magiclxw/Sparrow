#include "drv_adc.h"


adc_oneshot_unit_handle_t adc1_handle = NULL;
adc_cali_handle_t adc1_cali_chan3_handle = NULL;

const static char *TAG = "adc";

static int Battery_IO_Init();
static bool example_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);

int Battery_Adc_Init(void)
{
    Battery_IO_Init();  


    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, BATTERY_ADC_CHANNEL, &config));

    bool do_calibration1_chan0 = example_adc_calibration_init(ADC_UNIT_1, BATTERY_ADC_CHANNEL, ADC_ATTEN_DB_11, &adc1_cali_chan3_handle);

    
    return 1;
}

static bool example_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .chan = channel,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif


    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

static void example_adc_calibration_deinit(adc_cali_handle_t handle)
{

    ESP_LOGI(TAG, "deregister %s calibration scheme", "Curve Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(handle));

}

static int Battery_IO_Init()
{
    // gpio_reset_pin(ADC_EN_PIN);
    // gpio_reset_pin(CHARGE_PIN);
    // gpio_reset_pin(STANDBY_PIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(ADC_EN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CHARGE_PIN,GPIO_MODE_INPUT);
    gpio_set_direction(STANDBY_PIN,GPIO_MODE_INPUT);

    gpio_set_level(ADC_EN_PIN, 1);
    gpio_set_pull_mode(CHARGE_PIN,GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(STANDBY_PIN,GPIO_PULLUP_ONLY);

    return 1;
} 

int GetChargeState(void)
{
    return gpio_get_level(CHARGE_PIN);
}

int GetStandbyState(void)
{
    return gpio_get_level(STANDBY_PIN);
}
