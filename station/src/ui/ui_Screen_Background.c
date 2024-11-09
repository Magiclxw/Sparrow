#include "ui.h"
#include "drv_nvs/drv_nvs.h"

static uint32_t bgIndex = 0;
static lv_obj_t * ui_bg_icon = NULL;

static void ui_event_background(lv_event_t * e)
{
    static uint8_t index = 0;
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    reloadMainTimer();
    
    if(event_code == LV_EVENT_SHORT_CLICKED) 
    {
        if (bgIndex < BACKGROUND_MAX_NUM - 1)
        {
            bgIndex++;

        }
        else
        {
            bgIndex = 0;
        }
        lv_img_set_src(ui_bg_icon, backgroundImage[bgIndex]);
        lv_obj_set_pos(ui_bg_icon, 0, 0);

    }
    if(event_code == LV_EVENT_LONG_PRESSED) 
    {
        nvsSaveBackgroundIndex(bgIndex);
        ui_Screen_Main_LoadBackground();
        lv_scr_load(ui_Screen2);
    }
}

void ui_Screen_Background_init()
{
    ui_backgroundPage = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_backgroundPage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    nvsLoadBackgroundIndex(&bgIndex);
    // lv_obj_set_style_bg_img_src(ui_backgroundPage, backgroundImage[bgIndex], LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_bg_icon = lv_img_create(ui_backgroundPage);
    lv_img_set_src(ui_bg_icon, backgroundImage[bgIndex]);
    lv_obj_set_pos(ui_bg_icon, 0, 0);

    lv_obj_add_event_cb(ui_backgroundPage, ui_event_background, LV_EVENT_ALL, NULL);


}