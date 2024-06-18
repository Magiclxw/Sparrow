#include "ui_startPage.h"
#include "../imgs/img_sparrow.c"

static lv_obj_t *page;
static lv_obj_t *icons;

static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

void ui_startPageInit(lv_disp_t *disp)
{
    lv_color_t color = LV_COLOR_MAKE(36, 163, 235);

    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    page = lv_obj_create(scr);
    lv_obj_set_size(page, 240, 135);
    lv_obj_center(page);
    lv_obj_refr_size(page);

    icons = lv_img_create(page);

    lv_img_set_src(icons, &icon_sparrow);

    lv_obj_set_style_border_color(icons, color, 0);
    lv_obj_align(icons, LV_ALIGN_CENTER, -128, 0);
    //lv_obj_set_pos(icons, 0, 0);
    //lv_obj_set_align(icons, LV_ALIGN_CENTER);
    //lv_obj_set_style_radius(icons, LV_RADIUS_CIRCLE, 0);

    //lv_obj_set_style_shadow_width(icons, 15, 0);
    //lv_obj_set_style_shadow_spread(icons, 3, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, icons);
    lv_anim_set_values(&a, -128, 0);
    lv_animimg_set_duration(&icons, 100);
    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
    lv_anim_start(&a);

}
