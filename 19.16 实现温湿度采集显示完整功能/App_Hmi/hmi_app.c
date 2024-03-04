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
#include "humi_temphumi.h"
#include "lv_demo_widgets.h"

#if 0
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
#endif

#define STATE_ON    1
#define STATE_OFF   0
#define LED_NUM     3
static uint8_t g_ledState[LED_NUM] = {STATE_OFF,STATE_OFF,STATE_OFF};	//200 201 202 	theory / principle
static void LedClickedEventCb(lv_event_t *event)						//201 - 200 = 1 ,gain the index
{
    lv_obj_t *ledCtrBtn = lv_event_get_target(event);
    uint8_t *ledState = (uint8_t *)lv_event_get_user_data(event);
	uint8_t ledIndex = ledState - &g_ledState[0];
	
    if (*ledState == STATE_OFF)
    {
        lv_obj_set_style_bg_color(ledCtrBtn, lv_color_make(0xff, 0, 0),0);
        *ledState = STATE_ON;
    }
    else
    {
         lv_obj_set_style_bg_color(ledCtrBtn, lv_color_make(76, 151, 249),0);
        *ledState = STATE_OFF;
    }
	ToggleLed(ledIndex);
}

void HmiLedCtrlInit(void)
{
    lv_obj_set_style_bg_color(lv_scr_act(),lv_color_make(0, 0, 0), 0);

    static lv_style_t style;
    lv_style_init(&style);

    //按键共享风格
    lv_style_set_bg_color(&style,lv_color_make(76, 151, 249));
    lv_style_set_bg_opa(&style,LV_OPA_50);
    lv_style_set_radius(&style, 8);
    lv_style_set_width(&style,lv_pct(80));
    lv_style_set_height(&style,lv_pct(20));
    lv_style_set_border_width(&style,0);

    lv_obj_t *ledCtrlBtn;
    for (uint8_t i = 0; i < LED_NUM; i++)
    {
        ledCtrlBtn  = lv_btn_create(lv_scr_act());
		lv_obj_remove_style_all(ledCtrlBtn);
        lv_obj_set_align(ledCtrlBtn,LV_ALIGN_TOP_MID);
        lv_obj_align(ledCtrlBtn, LV_ALIGN_TOP_MID, 0, 40 + i * 140);
        lv_obj_add_style(ledCtrlBtn,&style,0);
        lv_obj_add_event_cb(ledCtrlBtn, LedClickedEventCb, LV_EVENT_CLICKED, &g_ledState[i]);
		TurnOffLed(i);
	}
}

void HmiInit(void)
{
	lv_init();					//lvgl系统初始化
	lv_port_disp_init();		//lvgl显示接口出啊实话，放在lv_init()后面
	lv_port_indev_init();		//lvgl输入接口初始化，放在lv_init()的后面
	//lv_example_flex_1();
	//lv_demo_widgets();
	//HmiLedCtrlInit();
	HmiTempHumiInit();
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
