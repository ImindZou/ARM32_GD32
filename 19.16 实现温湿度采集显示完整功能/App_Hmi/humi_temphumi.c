#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sensor_drv.h"
/* LVGL */
#include "lvgl.h"

/***************************************************
*
*   //思源字库声明API
*
***************************************************/
LV_FONT_DECLARE(lv_font_semibold_26);
LV_FONT_DECLARE(lv_font_semibold_28);
LV_FONT_DECLARE(lv_font_semibold_40);

/***************************************************
*
*   //图片库声明API
*
***************************************************/
LV_IMG_DECLARE(g_tempimg);
LV_IMG_DECLARE(g_humiimg);


static lv_obj_t *g_tempValLabel;
static lv_obj_t *g_humiValLabel;

void CreateTitleArea(lv_obj_t *parent, const char *title)
{
    lv_obj_t *titleLabel = lv_label_create(parent);
    lv_obj_set_style_text_font(titleLabel, &lv_font_semibold_28, 0);
    lv_label_set_text(titleLabel, title);
    lv_obj_set_style_text_color(titleLabel, lv_color_make(0xff,0xff,0xff),0);
    lv_obj_align(titleLabel, LV_ALIGN_CENTER, 0, 0);
}

static void	RefreshTempHumiVal(lv_timer_t *timer)
{
	SensorData_t sensorData;
	GetSensorData(&sensorData);
	char buf[10];
	sprintf(buf, "%2d", (uint32_t)sensorData.temp);
	lv_label_set_text(g_tempValLabel, buf);
	
	sprintf(buf, "%2d", (uint32_t)sensorData.humi);
	lv_label_set_text(g_humiValLabel, buf);
}

void CreateMainArea(lv_obj_t *parent)
{
    lv_obj_t *imgObj;
    lv_obj_t *nameLabel;

    imgObj = lv_img_create(parent);
    lv_img_set_src (imgObj, &g_tempimg);
    lv_obj_align(imgObj, LV_ALIGN_TOP_LEFT, 0, 0);

    nameLabel = lv_label_create(parent);
    lv_obj_set_style_text_font(nameLabel, &lv_font_semibold_26, 0);
	lv_label_set_text(nameLabel, "温度(℃)");
    lv_obj_set_style_text_color(nameLabel, lv_color_make(0xff,0xff,0xff), 0);
    lv_obj_align_to(nameLabel, imgObj, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    g_tempValLabel = lv_label_create(parent);
    lv_obj_set_style_text_font(g_tempValLabel, &lv_font_montserrat_40,0);
    lv_label_set_text(g_tempValLabel, "0");
    lv_obj_set_style_text_color(g_tempValLabel, lv_color_make(0xff, 0xff, 0xff),0);
    lv_obj_align_to(g_tempValLabel, nameLabel, LV_ALIGN_OUT_RIGHT_MID, 50, 0);

    imgObj = lv_img_create(parent);
    lv_img_set_src(imgObj, &g_humiimg);
    lv_obj_align(imgObj, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    nameLabel = lv_label_create(parent);
    lv_obj_set_style_text_font(nameLabel, &lv_font_semibold_26, 0);
    lv_label_set_text(nameLabel, "湿度(%)");
    lv_obj_set_style_text_color(nameLabel, lv_color_make(0xff, 0xff, 0xff),0);
    lv_obj_align_to(nameLabel, imgObj, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    g_humiValLabel = lv_label_create(parent);
    lv_obj_set_style_text_font(g_humiValLabel, &lv_font_montserrat_40,0);
    lv_label_set_text(g_humiValLabel, "0");
    lv_obj_set_style_text_color(g_humiValLabel, lv_color_make(0xff, 0xff, 0xff),0);
    lv_obj_align_to(g_humiValLabel, nameLabel, LV_ALIGN_OUT_RIGHT_MID, 50, 0);

    imgObj = lv_img_create(parent);
    lv_img_set_src(imgObj, &g_humiimg);
    lv_obj_align(imgObj, LV_ALIGN_BOTTOM_LEFT, 0, 0);

	lv_timer_create(RefreshTempHumiVal, 1000, NULL);
	
}

#if 1

void HmiTempHumiInit(void)
{
    lv_obj_t *tempHumiScr = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(tempHumiScr);
    lv_obj_set_size(tempHumiScr, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(tempHumiScr, lv_color_make(0x00, 0x00, 0x00), 0);
    lv_obj_set_style_bg_opa(tempHumiScr, LV_OPA_100, 0);
    lv_obj_set_style_border_width(tempHumiScr, 0, 0);
    lv_obj_set_style_radius(tempHumiScr, 0, 0);

    lv_obj_t *titleCont = lv_obj_create(tempHumiScr);
    lv_obj_set_size(titleCont, lv_pct(100), lv_pct(10));
    lv_obj_set_style_bg_color(titleCont, lv_color_make(0x00, 0x00, 0x00), 0);
    lv_obj_set_style_border_width(titleCont, 0, 0);
    lv_obj_set_scroll_dir(titleCont, LV_DIR_NONE);

    CreateTitleArea(titleCont, "温度湿度");

    static lv_point_t linePoints[2];
    linePoints[0].x = 10;
    linePoints[0].y = LV_VER_RES / 10 + 10;
    linePoints[1].x = LV_HOR_RES - 10;
    linePoints[1].y = LV_VER_RES / 10 + 10;
    lv_obj_t *line = lv_line_create(tempHumiScr);
    lv_obj_set_style_line_width(line, 1, 0);
    lv_obj_set_style_line_color(line, lv_color_make(0xff, 0xff, 0xff), 0);
    lv_obj_set_style_line_opa(line, LV_OPA_20, 0);
    lv_line_set_points(line, linePoints, 2);

    lv_obj_t *mainCont = lv_obj_create(tempHumiScr);
    lv_obj_set_size(mainCont, lv_pct(100), lv_pct(40));
    lv_obj_set_style_bg_color(mainCont, lv_color_make(0x00, 0x00, 0x00),0);
    lv_obj_set_style_bg_opa(mainCont, LV_OPA_100, 0);
    lv_obj_set_style_border_width(mainCont, 0, 0);
    lv_obj_set_scroll_dir(mainCont, LV_DIR_NONE);
    lv_obj_center(mainCont);
    CreateMainArea(mainCont);

}




#endif

