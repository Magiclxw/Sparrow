#include "ui.h"

static lv_obj_t * imgTomatoClock;

void ui_tomatoClock_screen_init(void)
{
    ui_tomatoClock = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_tomatoClock, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    imgTomatoClock = lv_img_create(ui_tomatoClock);
    lv_img_set_src(imgTomatoClock, &ui_img_tomato_clock);
    lv_obj_set_width(imgTomatoClock, 60);
    lv_obj_set_height(imgTomatoClock, 60);
    lv_obj_set_x(imgTomatoClock, 0);
    lv_obj_set_y(imgTomatoClock, -40);
    lv_obj_set_align(imgTomatoClock, LV_ALIGN_CENTER);
    lv_obj_add_flag(imgTomatoClock, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(imgTomatoClock, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

}