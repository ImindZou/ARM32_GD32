#include "lvgl.h"

void HmiDemo(void)
{
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1,100,100);
    lv_obj_set_pos(btn1,20,40);

    lv_obj_t *btn2 = lv_btn_create(btn1);
    //lv_obj_set_size(btn2,50,50);
    //lv_obj_set_pos(btn2,200,200);
}

