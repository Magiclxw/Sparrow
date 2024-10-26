#ifndef _TASK_SERVO_CONTROL_H
#define _TASK_SERVO_CONTROL_H
#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "driver/mcpwm_prelude.h"


// Please consult the datasheet of your servo before changing the following parameters
#define SERVO_MIN_PULSEWIDTH_US 500//500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500//2500  // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE        -90//-90   // Minimum angle
#define SERVO_MAX_DEGREE        90    // Maximum angle

#define SERVO_PULSE_GPIO             10        // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

#define SERVO_CONTROL_STACK_SIZE    (4096)
#define SERVO_CONTROL_PRIORITY      (10)

typedef struct ServoAngleState
{
    uint8_t ServoAngleState_Save;
    int ServoAngleState_Value;
}ServoAngleState_t;

extern QueueHandle_t Angle_State_Handle;

int createServoTask(void);
void servo_calibrate(void);

#endif