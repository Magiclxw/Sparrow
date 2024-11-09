#ifndef _DRV_ADC_H
#define _DRV_ADC_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "esp_err.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"

#define BATTERY_ADC_CHANNEL ADC_CHANNEL_3


extern adc_oneshot_unit_handle_t adc1_handle;
extern adc_cali_handle_t adc1_cali_chan3_handle;

int drvAdcInitAdc(void);

#endif