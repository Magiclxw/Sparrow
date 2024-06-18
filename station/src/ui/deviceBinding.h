#ifndef __DEVICE_BINDING_H__
#define __DEVICE_BINDING_H__

#include "lvgl.h"
#include "lv_port_indev.h"

extern lv_indev_t * indev_button;

void dBAddObjectToButtonGroup(lv_obj_t *obj);
void dBRemoveAllAbjsFromButtonGroup();
void dBObjToButtonInit();

#endif