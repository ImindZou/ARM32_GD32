#include "lvgl.h"

#if 0
/***************************************************
*   //���ո�������ж���
*   //���ڸ����󴴽��Ŀؼ����·�ע�����ξͳ��˷Ǹ���
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
*   //���ڶ������ÿؼ�λ��
*
***************************************************/
void HmiDemo(void)
{
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1,100,100);
    //�ؼ����룬��ƫ��ֵ
    //lv_obj_set_align(btn1,LV_ALIGN_TOP_MID);  //��
    //lv_obj_set_align(btn1,LV_ALIGN_CENTER);   //��
    //lv_obj_set_align(btn1,LV_ALIGN_BOTTOM_MID); //��
    //�ؼ����룬��ƫ��ֵ
    lv_obj_align(btn1,LV_ALIGN_TOP_MID,0 ,20);
    lv_obj_set_style_bg_color(btn1,lv_color_make(0xff,0,0),0);

    //lv_obj_t *btn2 = lv_btn_create(btn1);
    //lv_obj_set_align(btn2,LV_ALIGN_TOP_MID);

/***************************************************
*   //��������������ж���
*   //���ڸ����󴴽��Ŀؼ����·�ע�����ξͳ��˷Ǹ���
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

    lv_obj_remove_style_all(btn1);  //ɾ����ʽ
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
*   //ͨ�����������ÿؼ�����ʽ
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
#define STATE_ON    1
#define STATE_OFF   0
#define LED_NUM     3
static uint8_t g_ledState[LED_NUM] = {STATE_OFF,STATE_OFF,STATE_OFF};
static void Led1ClickedEventCb(lv_event_t *event)
{
    static lv_style_t style;
    lv_style_init(&style);

    lv_obj_t *ledCtrBtn = lv_event_get_target(event);

    if (g_ledState[0] == STATE_OFF)
    {
        lv_style_set_bg_color(&style,lv_color_make(0xff, 0, 0));
        g_ledState[0] = STATE_ON;
    }
    else
    {
        lv_style_set_bg_color(&style,lv_color_make(76, 151, 249));
        g_ledState[0] = STATE_OFF;
    }
    lv_obj_add_style(ledCtrBtn, &style, 0);
}

static void Led2ClickedEventCb(lv_event_t *event)
{
    static lv_style_t style;
    lv_style_init(&style);

    lv_obj_t *ledCtrBtn = lv_event_get_target(event);

    if (g_ledState[1] == STATE_OFF)
    {
        lv_style_set_bg_color(&style,lv_color_make(0xff, 0, 0));
        g_ledState[1] = STATE_ON;
    }
    else
    {
        lv_style_set_bg_color(&style,lv_color_make(76, 151, 249));
        g_ledState[1] = STATE_OFF;
    }
    lv_obj_add_style(ledCtrBtn, &style, 0);
}

static void Led3ClickedEventCb(lv_event_t *event)
{
    static lv_style_t style;
    lv_style_init(&style);

    lv_obj_t *ledCtrBtn = lv_event_get_target(event);

    if (g_ledState[2] == STATE_OFF)
    {
        lv_style_set_bg_color(&style,lv_color_make(0xff, 0, 0));
        g_ledState[2] = STATE_ON;
    }
    else
    {
        lv_style_set_bg_color(&style,lv_color_make(76, 151, 249));
        g_ledState[2] = STATE_OFF;
    }
    lv_obj_add_style(ledCtrBtn, &style, 0);
}

void HmiDemo(void)
{
    lv_obj_set_style_bg_color(lv_scr_act(),lv_color_make(0, 0, 0), 0);

    static lv_style_t style;
    lv_style_init(&style);



    //����������
    lv_style_set_bg_color(&style,lv_color_make(76, 151, 249));
    lv_style_set_bg_opa(&style,LV_OPA_50);
    lv_style_set_radius(&style, 8);
    lv_style_set_width(&style,lv_pct(80));
    lv_style_set_height(&style,lv_pct(20));
    lv_style_set_border_width(&style,0);
    //����1����
    lv_obj_t *ledCtrlBtn1  = lv_btn_create(lv_scr_act());
    lv_obj_set_align(ledCtrlBtn1,LV_ALIGN_TOP_MID);
    lv_obj_align(ledCtrlBtn1, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_style(ledCtrlBtn1,&style,0);
    lv_obj_add_event_cb(ledCtrlBtn1, Led1ClickedEventCb, LV_EVENT_CLICKED, NULL);

    //����2����
    lv_obj_t *ledCtrlBtn2  = lv_btn_create(lv_scr_act());
    lv_obj_set_align(ledCtrlBtn2,LV_ALIGN_CENTER);
    lv_obj_add_style(ledCtrlBtn2,&style,0);
    lv_obj_add_event_cb(ledCtrlBtn2, Led2ClickedEventCb, LV_EVENT_CLICKED, NULL);

    //����3����
    lv_obj_t *ledCtrlBtn3  = lv_btn_create(lv_scr_act());
    lv_obj_set_align(ledCtrlBtn3,LV_ALIGN_BOTTOM_MID);
    lv_obj_align(ledCtrlBtn3, LV_ALIGN_BOTTOM_MID,0, -10);
    lv_obj_add_style(ledCtrlBtn3,&style,0);
    lv_obj_add_event_cb(ledCtrlBtn3, Led3ClickedEventCb, LV_EVENT_CLICKED, NULL);
}




#endif








