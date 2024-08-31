#include "ui.h"

#define SETTINGS_ITEM_NUM (10)

enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
};
typedef uint8_t lv_menu_builder_variant_t;
lv_obj_t * ui_settingMenu;
lv_obj_t * ui_settingList;
lv_obj_t * ui_settingText;
lv_obj_t * ui_settingBtn;
lv_obj_t * ui_settingLabel[10];
lv_obj_t * ui_settingItem[10];
// lv_obj_t * ui_settingBtn[10];

lv_obj_t * mbox1;

static uint8_t s_currentIndex = 0;

const char* s_settingItemsTxt = "一二三\n2\n3\n4\n5\n6\n7\n8\n9\n10";
const char* s_settingItemsBuffer[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
static bool s_settingSwitch[SETTINGS_ITEM_NUM] = {true,false,true,true,true,true,true,true,true,false};

static lv_style_t style;


static void change_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_event_code_t event_code = lv_event_get_code(e);
    
    static uint8_t index = 0;

    if(event_code == LV_EVENT_LONG_PRESSED) 
    {
        uint8_t id = lv_roller_get_selected(ui_settingText);

        if (s_settingSwitch[id])
        {
            lv_led_set_color(ui_settingBtn, lv_palette_main(LV_PALETTE_RED));
            s_settingSwitch[id] = false;
        }
        else
        {
            lv_led_set_color(ui_settingBtn, lv_palette_main(LV_PALETTE_GREEN));
            s_settingSwitch[id] = true;
        }
    }
    else if (event_code == LV_EVENT_CLICKED)
    {
        if (index < lv_roller_get_option_cnt(ui_settingText)-1)
        {
            index++;
        }
        else
        {
            index = 0;
        }
        lv_roller_set_selected(ui_settingText, index, LV_ANIM_ON);

        if (s_settingSwitch[index])
        {
            lv_led_set_color(ui_settingBtn, lv_palette_main(LV_PALETTE_GREEN));
        }
        else
        {
            lv_led_set_color(ui_settingBtn, lv_palette_main(LV_PALETTE_RED));
        }
    }
}


void ui_Settings_screen_init(void)
{
    lv_style_init(&style);
    lv_style_set_border_width(&style, 0);
    lv_style_set_radius(&style, 0);

    ui_Settings = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(ui_Settings, LV_SCROLLBAR_MODE_OFF);

    ui_settingText = lv_roller_create(ui_Settings);
    ui_settingBtn = lv_led_create(ui_Settings);

    if (s_settingSwitch[0])
    {
        lv_led_set_color(ui_settingBtn, lv_palette_main(LV_PALETTE_GREEN));
    }
    else
    {
        lv_led_set_color(ui_settingBtn, lv_palette_main(LV_PALETTE_RED));
    }

    lv_roller_set_options(ui_settingText, s_settingItemsTxt, LV_ROLLER_MODE_INFINITE);
    lv_obj_add_style(ui_settingText, &style, 0);

    lv_obj_set_size(ui_settingText, 200, 135);
    lv_obj_set_size(ui_settingBtn, 20 , 20);

    lv_obj_set_align(ui_settingText, LV_ALIGN_LEFT_MID);
    lv_obj_set_align(ui_settingBtn, LV_ALIGN_RIGHT_MID);

    lv_obj_set_pos(ui_settingBtn, -10, 0);

    lv_obj_set_style_text_font(ui_settingText, &lv_font_simsun_16_cjk, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_roller_set_visible_row_count(ui_settingText, 4);

    lv_obj_add_event_cb(ui_settingText, change_event_cb, LV_EVENT_ALL, NULL);

    lv_roller_set_selected(ui_settingText, 0, LV_ANIM_OFF);
}
