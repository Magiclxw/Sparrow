#include "ui.h"
#include "drv_led/drv_led.h"

static lv_obj_t * imgTomatoClock;
static lv_obj_t * tomatoArc;
static lv_obj_t * tomatoLabelMinute;
static lv_obj_t * tomatoLabelSecond;
static lv_timer_t * tomatoClockTimer;
static lv_obj_t * tomatoLabelText1;

static uint16_t s_clockCounter = 0; // 番茄时钟计时器
static uint8_t s_clockState = 0;    //番茄时钟启动标志
static uint8_t s_workState = 0; // 番茄时钟工作状态

const char * TOMATO_LABEL_TEXT1 = "长按开始专注模式";
const char * TOMATO_LABEL_WAIT_REST = "短按开始休息";

static void s_tomatoClockStartTimer(uint8_t state)
{
    if (state == 0)
    {
        s_clockCounter = 60*25; // 25分钟
    }
    else
    {
        s_clockCounter = 60*5; // 5分钟
    }
    lv_timer_set_repeat_count(tomatoClockTimer, -1);
    stopMainTimer();
}

static void anim_x_cb1(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
    lv_obj_align_to(tomatoArc, imgTomatoClock,LV_ALIGN_CENTER, 200, 0);
    lv_obj_align_to(tomatoLabelMinute, tomatoArc, LV_ALIGN_CENTER, -10, -15);
    lv_obj_align_to(tomatoLabelSecond, tomatoArc, LV_ALIGN_CENTER, 20, 15);
}

static void anim_x_cb2(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

static void s_tomatoClockTimerCb(lv_timer_t * timer)
{
    if (s_clockCounter > 0)
    {
        s_clockCounter--;
        uint8_t minutes = s_clockCounter / 60;
        uint8_t seconds = s_clockCounter % 60;
        
        lv_label_set_text_fmt(tomatoLabelMinute, "%02d", minutes);
        lv_label_set_text_fmt(tomatoLabelSecond, "%02d", seconds);
        if (s_workState == 0)
        {
            lv_arc_set_value(tomatoArc, s_clockCounter/15);
        }
        else
        {
            lv_arc_set_value(tomatoArc, s_clockCounter/3);
        }
    }
    else
    {
        if (s_workState == 0)
        {
            s_workState = 1;
            s_clockCounter = 60*5; // 5分钟
            setLed(LED_GREEN);
            // 计时结束，更新为番茄休息状态
            lv_obj_set_style_bg_color(ui_tomatoClock, lv_color_hex3(0xfff), 0);
            lv_obj_set_style_arc_color(tomatoArc, lv_color_hex3(0x0f0), LV_PART_INDICATOR);
            // lv_label_set_text_static(tomatoLabelText1, TOMATO_LABEL_WAIT_REST);
        }
        else
        {
            s_workState = 0;
            s_clockCounter = 60*25; // 25分钟
            setLed(LED_RED);
            // 计时结束，更新为番茄工作状态
            lv_obj_set_style_bg_color(ui_tomatoClock, lv_color_hex3(0xffe), 0);
            lv_obj_set_style_arc_color(tomatoArc, lv_color_hex3(0xf00), LV_PART_INDICATOR);
            // lv_label_set_text_static(tomatoLabelText1, TOMATO_LABEL_TEXT1);
        }
    }
}
lv_anim_t animShow;
lv_anim_t animHide;

void ui_event_tomatoClock(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_LONG_PRESSED) 
    {
        if (s_clockState == 0)
        {
            // lv_anim_init(&animShow);
            // lv_anim_set_var(&animShow, tomatoArc);
            // lv_anim_set_values(&animShow, 180,  0);
            // lv_animimg_set_duration(&animShow, 500);
            // lv_anim_set_exec_cb(&animShow, anim_x_cb1);
            // lv_anim_set_path_cb(&animShow, lv_anim_path_overshoot);
            // lv_anim_start(&animShow);

            lv_anim_init(&animHide);
            lv_anim_set_var(&animHide, imgTomatoClock);
            lv_anim_set_values(&animHide, 0, -200);
            lv_animimg_set_duration(&animHide, 500);
            lv_anim_set_exec_cb(&animHide, anim_x_cb1);
            lv_anim_set_path_cb(&animHide, lv_anim_path_ease_in);
            lv_anim_start(&animHide);

            s_clockState = 1;
            tomatoClockTimer = lv_timer_create(s_tomatoClockTimerCb, 1000, NULL);
            s_tomatoClockStartTimer(0);
            setLed(LED_RED);
        }
        else
        {
            lv_timer_reset(tomatoClockTimer);
            lv_timer_pause(tomatoClockTimer);
            lv_scr_load(ui_Screen_Main);
        }
        // 停止主定时器
        stopMainTimer();
        return ;
    }
    if(event_code == LV_EVENT_CLICKED) 
    {
        // if (s_clockState == 1)
        // {
        //     if (s_workState == 0)
        //     {
        //         s_tomatoClockStartTimer(1);
        //     }
        //     else
        //     {
        //         s_tomatoClockStartTimer(0);
        //     }
        // }
    }
}

void ui_Screen_TomatoClock_init(void)
{
    s_clockState = 0;
    s_clockCounter = 60*25; // 25分钟

    ui_tomatoClock = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_tomatoClock, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_tomatoClock, lv_color_hex3(0xffe), 0);

    // tomatoLabelText1 = lv_label_create(ui_tomatoClock);
    // lv_obj_set_width(tomatoLabelText1, 110);
    // lv_obj_set_height(tomatoLabelText1, 28);
    // lv_obj_set_pos(tomatoLabelText1, 0, 0);
    // lv_label_set_text_static(tomatoLabelText1, TOMATO_LABEL_TEXT1);
    // lv_label_set_long_mode(tomatoLabelText1, LV_LABEL_LONG_SCROLL);
    // lv_obj_set_style_text_font(tomatoLabelText1, &lv_font_source_han_sans_normal_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    imgTomatoClock = lv_img_create(ui_tomatoClock);
    lv_img_set_src(imgTomatoClock, &ui_img_tomato_clock);
    lv_obj_set_width(imgTomatoClock, 60);
    lv_obj_set_height(imgTomatoClock, 60);
    // lv_obj_set_x(imgTomatoClock, 0);
    // lv_obj_set_y(imgTomatoClock, 65);
    lv_obj_set_align(imgTomatoClock, LV_ALIGN_CENTER);
    lv_obj_add_flag(imgTomatoClock, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(imgTomatoClock, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    tomatoArc = lv_arc_create(ui_tomatoClock);
    lv_arc_set_rotation(tomatoArc, 270);
    lv_arc_set_bg_angles(tomatoArc, 0, 360);
    lv_obj_set_size(tomatoArc, 120, 120);
    // lv_obj_set_align(tomatoArc, LV_ALIGN_CENTER);
    // lv_obj_set_x(tomatoArc, 180);
    // lv_obj_set_y(tomatoArc, 0);
    lv_obj_align_to(tomatoArc, imgTomatoClock, LV_ALIGN_CENTER, 200, 0);
    lv_obj_remove_style(tomatoArc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(tomatoArc, lv_color_hex3(0xf00), LV_PART_INDICATOR);
    lv_arc_set_range(tomatoArc, 0 , 100);
    lv_arc_set_value(tomatoArc, 100);

    tomatoLabelMinute = lv_label_create(ui_tomatoClock);
    lv_obj_set_width(tomatoLabelMinute, LV_SIZE_CONTENT);
    lv_obj_set_height(tomatoLabelMinute, LV_SIZE_CONTENT);
    lv_obj_align_to(tomatoLabelMinute, tomatoArc, LV_ALIGN_CENTER, -25, -30);
    lv_obj_set_style_text_color(tomatoLabelMinute, lv_color_hex(0x247600), 0);
    lv_label_set_text(tomatoLabelMinute, "25");
    lv_obj_set_style_text_font(tomatoLabelMinute, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    tomatoLabelSecond = lv_label_create(ui_tomatoClock);
    lv_obj_set_width(tomatoLabelSecond, LV_SIZE_CONTENT);
    lv_obj_set_height(tomatoLabelSecond, LV_SIZE_CONTENT);
    lv_obj_align_to(tomatoLabelSecond, tomatoArc, LV_ALIGN_CENTER, 20, 15);
    lv_obj_set_style_text_color(tomatoLabelSecond, lv_color_hex(0x247600), 0);
    lv_label_set_text(tomatoLabelSecond, "00");
    lv_obj_set_style_text_font(tomatoLabelSecond, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_timer_set_repeat_count(tomatoClockTimer, -1);
    
    lv_obj_add_event_cb(ui_tomatoClock, ui_event_tomatoClock, LV_EVENT_ALL, NULL);
}