/*
 * @Author: WYJ
 * @Date: 2022-03-31 21:27:35
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-31 22:16:25
 * @FilePath: \littleVisual\src\app\setting\setting_wea.cpp
 * @Description: 
 */

#include "setting_wea.h"
#include "lvgl.h"
#include "Arduino.h"
#include "config.h"
#include "setting.h"

LV_FONT_DECLARE(lv_font_montserrat_22);
LV_FONT_DECLARE(lv_font_montserrat_32);
extern lv_indev_t * indev_encoder;
extern ConfigStruct cfgstruct;

static lv_group_t * setting_wea_group;

static lv_obj_t * scr_setting_wea;
static lv_obj_t * wea_keyboard;
static lv_obj_t * cont;
static lv_obj_t * title;
static lv_obj_t * title_label;
static lv_obj_t * addr_label;
static lv_obj_t * addr_textarea;
static lv_obj_t * appid_label;
static lv_obj_t * appid_textarea;
static lv_obj_t * appsecrect_label;
static lv_obj_t * appsecret_textarea;
static lv_obj_t * save_btn;
static lv_obj_t * save_btn_label;
static lv_obj_t * back_btn;
static lv_obj_t * back_btn_label;
static lv_obj_t * msg_btn; 
static lv_obj_t * msg_label;

static lv_style_t cont_style;
static lv_style_t title_style;
static lv_style_t btn_style;
static lv_style_t label_style;
static lv_style_t textarea_style;

static bool ifMsgbox = true;       // 是否创建Msgbox

static void setting_wea_init(void);
static void setting_wea_style_init(void);
static void setting_wea_clean(void);
static void setting_wea_style_reset(void);
static void textarea_event_cb(lv_event_t *e);
static void setting_wea_cb(lv_event_t *e);

void setting_wea(void)
{
    setting_wea_group = lv_group_create();
    //lv_group_set_default(setting_wifi_group);
    lv_indev_set_group(indev_encoder, setting_wea_group); 

    scr_setting_wea = lv_scr_act();
    lv_obj_clean(scr_setting_wea);

    setting_wea_init();
}

static void setting_wea_init(void)
{
    setting_wea_style_init();

    wea_keyboard = lv_keyboard_create(lv_scr_act());
    lv_obj_set_size(wea_keyboard, 220, WEA_KEYBOARD_VER_RES);
    lv_obj_add_flag(wea_keyboard, LV_OBJ_FLAG_HIDDEN);

    cont = lv_obj_create(scr_setting_wea);
    lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);      // 实现滚动效果
    lv_obj_add_style(cont, &cont_style, LV_PART_MAIN);

    title = lv_btn_create(cont);
    lv_obj_set_size(title, 200, 40);
    lv_obj_add_style(title, &title_style, 0);
    title_label = lv_label_create(title);
    lv_obj_center(title_label);
    lv_label_set_text(title_label, "Weather");

    addr_label = lv_label_create(cont);
    lv_label_set_text(addr_label, "City:(Hangzhou)");
    lv_obj_set_size(addr_label, WEA_LABEL_WIDTH, WEA_LABEL_HEIGHT);
    lv_obj_add_style(addr_label, &label_style, 0);

    addr_textarea = lv_textarea_create(cont);
    lv_textarea_set_text(addr_textarea, cfgstruct.Address.c_str());
    lv_textarea_set_password_mode(addr_textarea, false);
    lv_textarea_set_one_line(addr_textarea, true);
    lv_obj_set_size(addr_textarea, 200, WIFI_TEXTAREA_HEIGHT);
    lv_obj_add_event_cb(addr_textarea, textarea_event_cb, LV_EVENT_ALL, wea_keyboard);

    appid_label = lv_label_create(cont);
    lv_label_set_text(appid_label, "AppID:");
    lv_obj_set_size(appid_label, WIFI_LABEL_WIDTH, WIFI_LABEL_HEIGHT);
    lv_obj_add_style(appid_label, &label_style, 0);

    appid_textarea = lv_textarea_create(cont);
    lv_textarea_set_text(appid_textarea, cfgstruct.appid.c_str());
    lv_textarea_set_password_mode(appid_textarea, false);
    lv_textarea_set_one_line(appid_textarea, true);
    lv_obj_set_size(appid_textarea, 200, WIFI_TEXTAREA_HEIGHT);
    lv_obj_add_event_cb(appid_textarea, textarea_event_cb, LV_EVENT_ALL, wea_keyboard);

    appsecrect_label = lv_label_create(cont);
    lv_label_set_text(appsecrect_label, "AppSecrect:");
    lv_obj_set_size(appsecrect_label, WIFI_LABEL_WIDTH, WIFI_LABEL_HEIGHT);
    lv_obj_add_style(appsecrect_label, &label_style, 0);

    appsecret_textarea = lv_textarea_create(cont);
    lv_textarea_set_text(appsecret_textarea, cfgstruct.appsecret.c_str());
    lv_textarea_set_password_mode(appsecret_textarea, false);
    lv_textarea_set_one_line(appsecret_textarea, true);
    lv_obj_set_size(appsecret_textarea, 200, WIFI_TEXTAREA_HEIGHT);
    lv_obj_add_event_cb(appsecret_textarea, textarea_event_cb, LV_EVENT_ALL, wea_keyboard);

    save_btn = lv_btn_create(cont);
    lv_obj_set_size(save_btn, 100, 50);
    lv_obj_add_style(save_btn, &btn_style, 0);
    save_btn_label = lv_label_create(save_btn);
    lv_obj_center(save_btn_label);
    lv_label_set_text(save_btn_label, "Save");
    lv_obj_add_event_cb(save_btn, setting_wea_cb, LV_EVENT_ALL, NULL);
    
    back_btn = lv_btn_create(cont);
    lv_obj_set_size(back_btn, 100, 50);
    lv_obj_add_style(back_btn, &btn_style, 0);
    back_btn_label = lv_label_create(back_btn);
    lv_obj_center(back_btn_label);
    lv_label_set_text(back_btn_label, "Back");
    lv_obj_add_event_cb(back_btn, setting_wea_cb, LV_EVENT_ALL, NULL);

    lv_group_add_obj(setting_wea_group, title);
    lv_group_add_obj(setting_wea_group, addr_textarea);
    lv_group_add_obj(setting_wea_group, appid_textarea);
    lv_group_add_obj(setting_wea_group, appsecret_textarea);
    lv_group_add_obj(setting_wea_group, save_btn);
    lv_group_add_obj(setting_wea_group, back_btn);
}

static void setting_wea_style_init(void)
{
    lv_style_init(&cont_style);
    lv_style_set_bg_color(&cont_style, lv_color_white());

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

    lv_style_init(&textarea_style);
    lv_style_set_border_color(&textarea_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_font(&textarea_style, &lv_font_montserrat_22);
}

static void setting_wea_clean(void)
{
    lv_obj_clean(scr_setting_wea);
    setting_wea_style_reset();
    lv_group_del(setting_wea_group);
}

static void setting_wea_style_reset(void)
{
    lv_style_reset(&cont_style);
    lv_style_reset(&title_style);
    lv_style_reset(&btn_style);
    lv_style_reset(&label_style);
    lv_style_reset(&textarea_style);
}

static void textarea_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED ) {
        lv_group_add_obj(setting_wea_group, kb);
        lv_obj_set_size(cont, 240, LV_VER_RES/2);
        lv_group_focus_obj(ta);
        lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
        
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_group_focus_obj(kb);
        lv_group_set_editing((lv_group_t *)lv_obj_get_group(kb), kb);
        lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    }
    if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height(cont, LV_VER_RES);
        lv_group_remove_obj(kb);
        lv_group_focus_obj(ta);
    }
}

static void setting_wea_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);    // 获取触发事件
    lv_obj_t *obj = lv_event_get_target(e); 
    if( code == LV_EVENT_CLICKED)
    {
        if(obj == back_btn)
        {
            ifMsgbox = !ifMsgbox;
            setting_wea_clean();
            setting();
        }
        if(obj == save_btn)
        {
            String addr = lv_textarea_get_text(addr_textarea);
            String appid = lv_textarea_get_text(appid_textarea);
            String appsecret = lv_textarea_get_text(appsecret_textarea);

            // 需要同时有效
            if(addr != "" && appid != "" && appsecret != "")
            {
                cfgstruct.Address = addr;
                cfgstruct.appid = appid;
                cfgstruct.appsecret = appsecret;
                Config_save();
                if( ifMsgbox )
                {
                    msg_btn = lv_btn_create(cont);
                    lv_obj_set_size(msg_btn, 200, 40);
                    lv_obj_add_style(msg_btn, &btn_style, 0);
                    msg_label = lv_label_create(msg_btn);
                    lv_obj_center(msg_label);
                    lv_group_add_obj(setting_wea_group, msg_btn);
                    ifMsgbox = !ifMsgbox;
                }
                lv_label_set_text(msg_label, "Save OK!!!");
                lv_group_focus_obj(msg_btn);  
            }
            else{
                if( ifMsgbox )
                {
                    msg_btn = lv_btn_create(cont);
                    lv_obj_set_size(msg_btn, 200, 40);
                    lv_obj_add_style(msg_btn, &btn_style, 0);
                    msg_label = lv_label_create(msg_btn);
                    lv_obj_center(msg_label);
                    lv_group_add_obj(setting_wea_group, msg_btn);
                    ifMsgbox = !ifMsgbox;
                }
                lv_label_set_text(msg_label, "SAVE ERROR");
                lv_group_focus_obj(msg_btn);
            }
        }
    }
}