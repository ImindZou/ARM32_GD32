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
#if 1
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












