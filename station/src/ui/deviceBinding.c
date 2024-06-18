#include "deviceBinding.h"


static lv_group_t *s_group;

void dBAddObjectToButtonGroup(lv_obj_t *obj)
{
    lv_group_add_obj(s_group, obj);
}

void dBRemoveAllAbjsFromButtonGroup()
{
    lv_group_remove_all_objs(s_group);
}

void dBObjToButtonInit()
{
    lv_port_indev_init();

    s_group = lv_group_create();
    //lv_group_set_default(s_group);
    //将group与输入设备关联
    lv_indev_set_group(indev_button, s_group);
}


