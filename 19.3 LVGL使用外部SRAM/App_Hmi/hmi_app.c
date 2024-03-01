#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rtc_drv.h"
#include "sensor_drv.h"
#include "led_drv.h"
#include "key_drv.h"
#include "touch_drv.h"
#include "lcd_drv.h"
#include "store_app.h"


/* LVGL */
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"

#include "lv_demo_widgets.h"

void lv_example_flex_1(void)
{
    /*Create a container with ROW flex direction*/
    lv_obj_t * cont_row = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont_row, 300, 75);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);

    /*Create a container with COLUMN flex direction*/
    lv_obj_t * cont_col = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont_col, 200, 150);
    lv_obj_align_to(cont_col, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);

    uint32_t i;
    for(i = 0; i < 10; i++) {
        lv_obj_t * obj;
        lv_obj_t * label;

        /*Add items to the row*/
        obj = lv_btn_create(cont_row);
        lv_obj_set_size(obj, 100, LV_PCT(100));

        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "Item: %"LV_PRIu32, i);
        lv_obj_center(label);

        /*Add items to the column*/
        obj = lv_btn_create(cont_col);
        lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);

        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "Item: %"LV_PRIu32, i);
        lv_obj_center(label);
    }
}

void HmiInit(void)
{
	lv_init();					//lvgl系统初始化
	lv_port_disp_init();		//lvgl显示接口出啊实话，放在lv_init()后面
	lv_port_indev_init();		//lvgl输入接口初始化，放在lv_init()的后面
	//lv_example_flex_1();
	lv_demo_widgets();
}

/**
***********************************************************
* @brief 人机交互任务处理函数
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
	lv_task_handler();
}
