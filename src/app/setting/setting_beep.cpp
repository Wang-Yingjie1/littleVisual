/*
 * @Author: WYJ
 * @Date: 2022-03-31 22:17:28
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-01 21:46:34
 * @FilePath: \littleVisual\src\app\setting\setting_beep.cpp
 * @Description: 
 */


#include "setting_beep.h"
#include "lvgl.h"
#include "config.h"
#include "Arduino.h"
#include "driver/buzz.h"
#include "setting.h"

LV_FONT_DECLARE(lv_font_montserrat_32);
extern lv_indev_t * indev_encoder;
extern ConfigStruct cfgstruct;

static lv_group_t * setting_beep_group;

static bool ifMsgbox = true;       // 是否创建Msgbox

static lv_obj_t * scr_setting_beep;
// static lv_obj_t * cont;
static lv_obj_t * title;
static lv_obj_t * title_label;
static lv_obj_t * switch_beep;
static lv_obj_t * save_btn;
static lv_obj_t * save_btn_label;
static lv_obj_t * back_btn;
static lv_obj_t * back_btn_label;
static lv_obj_t * MsgLabel;

static lv_style_t title_style;
//static lv_style_t sw_style;
static lv_style_t btn_style;
static lv_style_t label_style;

static void setting_beep_init(void);
static void setting_beep_style_init(void);
static void switch_event_cb(lv_event_t * e);
static void setting_beep_cb(lv_event_t * e);
static void setting_beep_clean(void);
static void setting_beep_style_reset(void);

void setting_beep(void)
{
    setting_beep_group = lv_group_create();
    lv_indev_set_group(indev_encoder, setting_beep_group);

    scr_setting_beep = lv_scr_act();
    lv_obj_clean(scr_setting_beep);

    setting_beep_init();

}

static void setting_beep_init(void)
{
    setting_beep_style_init();

    title = lv_btn_create(scr_setting_beep);
    lv_obj_set_size(title, 200, 40);
    lv_obj_add_style(title, &title_style, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    title_label = lv_label_create(title);
    lv_obj_center(title_label);
    lv_label_set_text(title_label, "Beep");

    switch_beep = lv_switch_create(scr_setting_beep);
    lv_obj_add_state(switch_beep, cfgstruct.Beep? LV_STATE_CHECKED:LV_STATE_DEFAULT);
    lv_obj_add_event_cb(switch_beep, switch_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_center(switch_beep);

    save_btn = lv_btn_create(scr_setting_beep);
    lv_obj_set_size(save_btn, 100, 50);
    lv_obj_add_style(save_btn, &btn_style, 0);
    save_btn_label = lv_label_create(save_btn);
    lv_obj_center(save_btn_label);
    lv_label_set_text(save_btn_label, "Save");
    lv_obj_add_event_cb(save_btn, setting_beep_cb, LV_EVENT_ALL, NULL);
    lv_obj_align(save_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
   
    back_btn = lv_btn_create(scr_setting_beep);
    lv_obj_set_size(back_btn, 100, 50);
    lv_obj_add_style(back_btn, &btn_style, 0);
    back_btn_label = lv_label_create(back_btn);
    lv_obj_center(back_btn_label);
    lv_label_set_text(back_btn_label, "Back");
    lv_obj_add_event_cb(back_btn, setting_beep_cb, LV_EVENT_ALL, NULL);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);

    lv_group_add_obj(setting_beep_group, switch_beep);
    lv_group_add_obj(setting_beep_group, save_btn);
    lv_group_add_obj(setting_beep_group, back_btn);
}

static void setting_beep_style_init(void)
{
    lv_style_init(&title_style);
    lv_style_set_bg_color(&title_style, lv_palette_lighten(LV_PALETTE_GREY, 1));
    lv_style_set_text_font(&title_style, &lv_font_montserrat_32);
    lv_style_set_text_color(&title_style, lv_color_black());

    lv_style_init(&btn_style);
    lv_style_set_bg_color(&btn_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_color(&btn_style, lv_color_black());
    lv_style_set_text_opa(&btn_style, LV_OPA_100);
    lv_style_set_text_font(&btn_style, &lv_font_montserrat_22);

    lv_style_init(&label_style);
    lv_style_set_text_font(&label_style, &lv_font_montserrat_22);
    lv_style_set_text_color(&label_style, lv_color_black());

}

static void switch_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if( code == LV_EVENT_VALUE_CHANGED)
    {
        Buzz_Tone(700, 500, 200);
    }
}

static void setting_beep_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED)
    {
        if( obj == back_btn)
        {
            ifMsgbox = !ifMsgbox;
            Buzz_Enable(cfgstruct.Beep);
            setting_beep_clean();
            setting();
        }
        if(obj == save_btn)
        {
            cfgstruct.Beep = (switch_beep->state == LV_STATE_DEFAULT) ? false : true;
            Config_save();
            if ( ifMsgbox )
            {
                MsgLabel = lv_label_create(scr_setting_beep);
                lv_obj_add_style(MsgLabel, &label_style, 0);
                lv_label_set_text(MsgLabel, "Save OK!!");
                lv_obj_align_to(MsgLabel, title, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
                ifMsgbox = !ifMsgbox;
            }
        }
    }
}

static void setting_beep_clean(void)
{
    lv_obj_clean(scr_setting_beep);
    setting_beep_style_reset();
    lv_group_del(setting_beep_group);
}

static void setting_beep_style_reset()
{
    lv_style_reset(&title_style);
    lv_style_reset(&btn_style);
    lv_style_reset(&label_style);  
}