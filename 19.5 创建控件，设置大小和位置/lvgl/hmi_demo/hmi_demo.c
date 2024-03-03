#include "lvgl.h"

#if 0
/***************************************************
*   //参照父对象进行对齐
*   //基于父对象创建的控件，下方注释屏蔽就成了非父类
*
***************************************************/
void HmiDemo(void)
{
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1,100,100);
    lv_obj_set_pos(btn1,20,40);

    lv_obj_t *btn2 = lv_btn_create(btn1);
    //lv_obj_set_size(btn2,50,50);
    //lv_obj_set_pos(btn2,200,200);
}
#endif
#if 0
/***************************************************
*
*   //基于对齐设置控件位置
*
***************************************************/
void HmiDemo(void)
{
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1,100,100);
    //控件对齐，无偏移值
    //lv_obj_set_align(btn1,LV_ALIGN_TOP_MID);  //上
    //lv_obj_set_align(btn1,LV_ALIGN_CENTER);   //中
    //lv_obj_set_align(btn1,LV_ALIGN_BOTTOM_MID); //下
    //控件对齐，有偏移值
    lv_obj_align(btn1,LV_ALIGN_TOP_MID,0 ,20);
    lv_obj_set_style_bg_color(btn1,lv_color_make(0xff,0,0),0);

    //lv_obj_t *btn2 = lv_btn_create(btn1);
    //lv_obj_set_align(btn2,LV_ALIGN_TOP_MID);

/***************************************************
*   //参照其他对象进行对齐
*   //基于父对象创建的控件，下方注释屏蔽就成了非父类
*
***************************************************/
    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    //lv_obj_set_align(btn2,LV_ALIGN_BOTTOM_MID);
    //lv_obj_set_size(btn2,100,100);
    //lv_obj_align(btn2,LV_ALIGN_BOTTOM_MID,0,-20);
    lv_obj_set_style_bg_color(btn2,lv_color_make(0,0xff,0),0);
    lv_obj_align_to(btn2,btn1,LV_ALIGN_OUT_RIGHT_MID,10,0);

}
#endif
#if 0
void HmiDemo(void)
{
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1,100,50);
    lv_obj_set_align(btn1,LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(btn1,lv_color_make(0xff,0,0),0);
    lv_obj_set_style_bg_opa(btn1,LV_OPA_50,50);
    lv_obj_set_style_border_color(btn1,lv_color_make(0,0xff,0),0);
    lv_obj_set_style_border_width(btn1,1,0);
    lv_obj_set_style_pad_all(btn1,0,0);
    lv_obj_set_style_outline_color(btn1,lv_color_make(0,0,0xff),0);
    lv_obj_set_style_outline_width(btn1,15,0);
    lv_obj_set_style_radius(btn1,0,0);

    lv_obj_remove_style_all(btn1);  //删除样式
    //lv_obj_t *btn2 = lv_btn_create(btn1);
    //lv_obj_set_align(btn2,LV_ALIGN_OUT_TOP_LEFT);
}

#endif

#if 0

void HmiDemo()
{
    //lv_obj_t *slider = lv_slider_create(lv_scr_act());
    //lv_obj_set_align(slider,LV_ALIGN_CENTER);
    //lv_obj_set_style_bg_color(slider,lv_color_make(0xff,0,0),LV_PART_KNOB | LV_STATE_PRESSED);


    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1,100,50);
    lv_obj_set_align(btn1,LV_ALIGN_CENTER);

    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn2,40,20);
    lv_obj_set_align(btn2,LV_ALIGN_BOTTOM_MID);

/***************************************************
*
*   //通过共享方法设置控件的样式
*
***************************************************/
    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_bg_color(&style,lv_color_make(0xff,0,0));
    lv_style_set_bg_opa(&style,LV_OPA_50);

    lv_obj_add_style(btn1,&style,0);
    lv_obj_add_style(btn2,&style,0);
}

#endif

#if 1
/***************************************************
*
*   //19.10LED控制UI布局实现
*   //19.12 FLEX弹性布局的使用方法
*   //使用cont这种方法的时候cont可以视为容器
*
***************************************************/
#define STATE_ON    1
#define STATE_OFF   0
#define LED_NUM     3
static uint8_t g_ledState[LED_NUM] = {STATE_OFF,STATE_OFF,STATE_OFF};
static void LedClickedEventCb(lv_event_t *event)
{
    lv_obj_t *ledCtrBtn = lv_event_get_target(event);
    uint8_t *ledState = (uint8_t *)lv_event_get_user_data(event);
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
}


void HmiDemo(void)
{
    //lv_obj_set_style_bg_color(lv_scr_act(),lv_color_make(0, 0, 0), 0);

    lv_obj_t *mainCont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mainCont, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(mainCont, lv_color_make(0x00,0x00,0x00),0);
    lv_obj_set_style_bg_opa(mainCont, LV_OPA_100,0);
    lv_obj_set_style_border_width(mainCont, 0, 0);
    lv_obj_set_style_radius(mainCont, 0, 0);
    lv_obj_set_flex_flow(mainCont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(mainCont, LV_FLEX_ALIGN_SPACE_EVENLY, 0, LV_FLEX_ALIGN_CENTER);


    //按键共享风格
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style,lv_color_make(76, 151, 249));
    lv_style_set_bg_opa(&style,LV_OPA_50);
    lv_style_set_radius(&style, 8);
    lv_style_set_width(&style,lv_pct(80));
    lv_style_set_height(&style,lv_pct(20));
    lv_style_set_border_width(&style,0);

    #if 0
    //按键1设置
    lv_obj_t *ledCtrlBtn1  = lv_btn_create(lv_scr_act());
    lv_obj_set_align(ledCtrlBtn1,LV_ALIGN_TOP_MID);
    lv_obj_align(ledCtrlBtn1, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_add_style(ledCtrlBtn1,&style,0);
    lv_obj_add_event_cb(ledCtrlBtn1, LedClickedEventCb, LV_EVENT_CLICKED, &g_ledState[0]);

    //按键2设置
    lv_obj_t *ledCtrlBtn2  = lv_btn_create(lv_scr_act());
    lv_obj_set_align(ledCtrlBtn2,LV_ALIGN_CENTER);
    lv_obj_add_style(ledCtrlBtn2,&style,0);
    lv_obj_add_event_cb(ledCtrlBtn2, LedClickedEventCb, LV_EVENT_CLICKED, &g_ledState[1]);

    //按键3设置
    lv_obj_t *ledCtrlBtn3  = lv_btn_create(lv_scr_act());
    lv_obj_set_align(ledCtrlBtn3,LV_ALIGN_BOTTOM_MID);
    lv_obj_align(ledCtrlBtn3, LV_ALIGN_BOTTOM_MID,0, -60);
    lv_obj_add_style(ledCtrlBtn3,&style,0);
    lv_obj_add_event_cb(ledCtrlBtn3, LedClickedEventCb, LV_EVENT_CLICKED, &g_ledState[2]);
    #endif
    lv_obj_t *ledCtrlBtn;
    for (uint8_t i = 0; i < LED_NUM; i++)
    {
        ledCtrlBtn  = lv_btn_create(mainCont);
        lv_obj_set_align(ledCtrlBtn,LV_ALIGN_TOP_MID);
        //lv_obj_align(ledCtrlBtn, LV_ALIGN_TOP_MID, 0, 40 + i * 140);
        lv_obj_add_style(ledCtrlBtn,&style,0);
        lv_obj_add_event_cb(ledCtrlBtn, LedClickedEventCb, LV_EVENT_CLICKED, &g_ledState[i]);
    }
}

#endif

#if 0
/***************************************************
*
*   //19.12 FLEX弹性布局的使用方法
*   //使用cont这种方法的时候cont可以视为容器
*
***************************************************/
void HmiDemo()
{
    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 180, 470);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    lv_obj_t * btn1 = lv_btn_create(cont);
    lv_obj_set_size(btn1, 30,40);
    lv_obj_t *label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "01");

    lv_obj_t * btn2 = lv_btn_create(cont);
    lv_obj_set_size(btn2, 40,80);
    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "02");

    lv_obj_t * btn3 = lv_btn_create(cont);
    lv_obj_set_size(btn3,80,80);
    lv_obj_t *label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "03");

    lv_obj_t * btn4 = lv_btn_create(cont);
    lv_obj_set_size(btn4, 50,80);
    lv_obj_t *label4 = lv_label_create(btn4);
    lv_label_set_text(label4, "04");

    lv_obj_t * btn5 = lv_btn_create(cont);
    lv_obj_set_size(btn5, 80,80);
    lv_obj_t *label5 = lv_label_create(btn5);
    lv_label_set_text(label5, "05");


}



#endif






