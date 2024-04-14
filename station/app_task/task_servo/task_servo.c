#include "task_servo.h"
#include "../../sys_config.h"


TaskHandle_t Task_Servo_Handle = NULL;
QueueHandle_t Angle_State_Handle = NULL;    //角度控制队列句柄

mcpwm_cmpr_handle_t comparator = NULL;

static void vTaskServoProcessing();

static const char *TAG = "example";

static inline uint32_t example_angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

int Servo_Control_TASK_Create(void)
{
    xTaskCreate((TaskFunction_t)vTaskServoProcessing,
                (const char*)"Servo Processing",
                (uint32_t )SERVO_CONTROL_STACK_SIZE,
                (void *	)NULL,
                (UBaseType_t)SERVO_CONTROL_PRIORITY,
                &Task_Servo_Handle);
    return OPERATE_SUCCESS;
}

static void vTaskServoProcessing()
{
    ESP_LOGI(TAG, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = SERVO_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

    // set the initial compare value, so that the servo will spin to the center position
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                    MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));

    ServoAngleState_t angle_state;

    Angle_State_Handle = xQueueCreate(1,sizeof(ServoAngleState_t));

    servo_calibrate();

    int angle = 0;
    int step = 1;
    while(1)
    {
        xQueueReceive(Angle_State_Handle,&angle_state,portMAX_DELAY);

        ESP_LOGI(TAG, "Angle of rotation: %d", angle_state.ServoAngleState_Value);

        if(angle_state.ServoAngleState_Value < -90 || angle_state.ServoAngleState_Value > 90)
        {
            continue;
        }

        //数据不需保存
        if(angle_state.ServoAngleState_Save == 0)
        {
            printf("servo control %d\r\n",angle_state.ServoAngleState_Value);
            mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle_state.ServoAngleState_Value));
        }
        else
        {
            nvs_handle_t my_handle;
            esp_err_t ret = nvs_open("storage", NVS_READWRITE, &my_handle);
            if (ret != ESP_OK) {
                printf("Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
            } else {
                printf("save angle : %d\r\n",angle_state.ServoAngleState_Value);

                ret = nvs_set_i16(my_handle,"angle",angle_state.ServoAngleState_Value);

                if(ret == ESP_OK)
                {

                }
            }
        }

        // ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle)));
        // //Add delay, since it takes time for servo to rotate, usually 200ms/60degree rotation under 5V power supply
        // vTaskDelay(pdMS_TO_TICKS(500));
        // if ((angle + step) > 90 || (angle + step) < -90) {
        //     step *= -1;
        // }
        // angle += step;
        // ESP_LOGI(TAG, "Angle of rotation: %d", angle);
    }
}

/* 舵机校准 */
void servo_calibrate(void)
{
    //-90°
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(-90)));

    vTaskDelay(pdMS_TO_TICKS(1000));
    //90°
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(90)));

    vTaskDelay(pdMS_TO_TICKS(1000));
    //0°
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));
}

