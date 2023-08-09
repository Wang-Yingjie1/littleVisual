/*
 * @Author: WYJ
 * @Date: 2022-03-30 10:39:44
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-01 22:02:35
 * @FilePath: \littleVisual\src\app\setting\setting_wifi.cpp
 * @Description: 
 */

#include "setting.h"
#include "setting_wifi.h"
#include "lvgl.h"
#include "Arduino.h"
#include "config.h"

LV_FONT_DECLARE(lv_font_simsun_16_cjk);
LV_FONT_DECLARE(lv_font_montserrat_22);
LV_FONT_DECLARE(lv_font_montserrat_32);
extern lv_indev_t * indev_encoder;
extern ConfigStruct cfgstruct;

static lv_group_t * setting_wifi_group;

static lv_obj_t * scr_setting_wifi;
static lv_obj_t * cont;
static lv_obj_t * title;
static lv_obj_t * title_label;
static lv_obj_t * wifi_name_label;
static lv_obj_t * password_label;
static lv_obj_t * wifi_keyboard = NULL;        // keyboard
static lv_obj_t * name_textarea;
static lv_obj_t * pwd_textarea;
static lv_obj_t * save_btn;
static lv_obj_t * save_btn_label;
static lv_obj_t * back_btn;
static lv_obj_t * back_btn_label;
static lv_obj_t * msg_btn; 
static lv_obj_t * msg_label;

static lv_style_t title_style;
static lv_style_t btn_style;
static lv_style_t setting_cont_style;
static lv_style_t label_style;
static lv_style_t textarea_style;

static bool ifMsgbox = true;       // 是否创建Msgbox

static void setting_wifi_cb(lv_event_t *e);
static void setting_wifi_style_init(void);
static void setting_wifi_init(void);
static void setting_wifi_clean(void);
static void setting_wifi_style_reset(void);
static void textarea_event_cb(lv_event_t *e);

static void setting_wifi_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);    // 获取触发事件
    lv_obj_t *obj = lv_event_get_target(e); 

    if( code == LV_EVENT_CLICKED)
    {
        if(obj == back_btn)
        {
            ifMsgbox = !ifMsgbox;
            setting_wifi_clean();
            setting();
        }
        if(obj == save_btn)
        {
            String ssid = lv_textarea_get_text(name_textarea);
            String pwd = lv_textarea_get_text(pwd_textarea);

            // 需要同时有效
            if(ssid != "" && pwd != "")
            {
                cfgstruct.Wifi_name = ssid;
                cfgstruct.Wifi_pwd = pwd;
                Config_save();
                if( ifMsgbox )
                {
                    msg_btn = lv_btn_create(cont);
                    lv_obj_set_size(msg_btn, 200, 40);
                    lv_obj_add_style(msg_btn, &btn_style, 0);
                    msg_label = lv_label_create(msg_btn);
                    lv_obj_center(msg_label);
                    lv_group_add_obj(setting_wifi_group,msg_btn);
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
                    lv_group_add_obj(setting_wifi_group,msg_btn);
                    ifMsgbox = !ifMsgbox;
                }
                lv_label_set_text(msg_label, "SAVE ERROR");
                lv_group_focus_obj(msg_btn);
            }
        }
    }
}

static void setting_wifi_clean(void)
{
    lv_obj_clean(scr_setting_wifi);
    setting_wifi_style_reset();
    lv_group_del(setting_wifi_group);
}

static void setting_wifi_style_reset(void)
{
    lv_style_reset(&title_style);
    lv_style_reset(&btn_style);
    lv_style_reset(&setting_cont_style);
    lv_style_reset(&label_style);
    lv_style_reset(&textarea_style);
}

static void setting_wifi_style_init(void)
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

    lv_style_init(&setting_cont_style);
    lv_style_set_bg_color(&setting_cont_style, lv_color_white());

    lv_style_init(&label_style);
    lv_style_set_text_font(&label_style, &lv_font_montserrat_22);
    lv_style_set_text_color(&label_style, lv_color_black());

    lv_style_init(&textarea_style);
    lv_style_set_border_color(&textarea_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_font(&textarea_style, &lv_font_montserrat_22);

    Serial.println("setting_wifi_style_init OK");
}

// static lv_obj_t * pre_focus = NULL;
// static void kb_cb(lv_event_t *e)
// {
//     lv_keyboard_def_event_cb(e);
//     lv_obj_t * obj = lv_event_get_target(e);
    
//     // if(strcmp(txt, LV_SYMBOL_OK) == 0)
//     // {
//     //     Serial.println("LV_SYMBOL_OK");
//     //     // lv_group_remove_all_objs(setting_wifi_group);
//     //     // lv_keyboard_set_textarea(wifi_keyboard, NULL);
//     //     // lv_obj_add_flag(wifi_keyboard, LV_OBJ_FLAG_HIDDEN);

//     //     // lv_obj_set_size(cont, 240, 240);
//     //     // lv_obj_center(cont);

//     //     // lv_group_add_obj(setting_wifi_group, title);
//     //     // lv_group_add_obj(setting_wifi_group, name_textarea);
//     //     // lv_group_add_obj(setting_wifi_group, pwd_textarea);    
//     //     // lv_group_add_obj(setting_wifi_group, save_btn);
//     //     // lv_group_add_obj(setting_wifi_group, back_btn);

//     //     // if(pre_focus != NULL)
//     //     // {
//     //     //     lv_group_focus_obj(pre_focus);
//     //     //     pre_focus = NULL;
//     //     // }
//     // } 
// }

// static void kb_event_cb(lv_event_t *e)
// {
//     lv_keyboard_def_event_cb(e);
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t * obj = lv_event_get_target(e);
//     if(code == LV_EVENT_CANCEL)
//     {
//         lv_group_focus_obj(lv_keyboard_get_textarea(obj));
//         lv_obj_del(obj);
//         wifi_keyboard = NULL;
        
//         lv_obj_set_size(cont, 240, 240);
//         lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
//     }
// }
static void textarea_event_cb(lv_event_t *e)
{   
    // lv_indev_t * indev = lv_indev_get_act();
    // if(indev == NULL) return;
    // lv_indev_type_t indev_type = lv_indev_get_type(indev);

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);

//    if(code == LV_EVENT_CLICKED && indev_type == LV_INDEV_TYPE_ENCODER) {
    if(code == LV_EVENT_CLICKED ) {
        lv_group_add_obj(setting_wifi_group, kb);
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

static void setting_wifi_init(void)
{
    setting_wifi_style_init();
    
    wifi_keyboard = lv_keyboard_create(lv_scr_act());
    lv_obj_set_size(wifi_keyboard, WIFI_KEYBOARD_HOR_RES, WIFI_KEYBOARD_VER_RES);
    //lv_obj_align(wifi_keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    //lv_group_add_obj(keyboard_group, wifi_keyboard);
    //lv_keyboard_set_textarea(wifi_keyboard, NULL);   
    //lv_obj_add_event_cb(wifi_keyboard, kb_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(wifi_keyboard, LV_OBJ_FLAG_HIDDEN);

    cont = lv_obj_create(scr_setting_wifi);
    lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);      // 实现滚动效果
    lv_obj_add_style(cont, &setting_cont_style, LV_PART_MAIN);

    title = lv_btn_create(cont);
    lv_obj_set_size(title, 200, 40);
    lv_obj_add_style(title, &title_style, 0);
    title_label = lv_label_create(title);
    lv_obj_center(title_label);
    lv_label_set_text(title_label, "WiFi");

    wifi_name_label = lv_label_create(cont);
    lv_label_set_text(wifi_name_label, "Name:");
    lv_obj_set_size(wifi_name_label, WIFI_LABEL_WIDTH, WIFI_LABEL_HEIGHT);
    lv_obj_add_style(wifi_name_label, &label_style, 0);

    name_textarea = lv_textarea_create(cont);
    lv_textarea_set_text(name_textarea, "");
    lv_textarea_set_password_mode(name_textarea, false);
    lv_textarea_set_one_line(name_textarea, true);
    lv_obj_set_size(name_textarea, 200, WIFI_TEXTAREA_HEIGHT);
    lv_obj_add_event_cb(name_textarea, textarea_event_cb, LV_EVENT_ALL, wifi_keyboard);

    password_label = lv_label_create(cont);
    lv_label_set_text(password_label, "Password:");
    lv_obj_set_size(password_label, WIFI_LABEL_WIDTH, WIFI_LABEL_HEIGHT);
    lv_obj_add_style(password_label, &label_style, 0);

    pwd_textarea = lv_textarea_create(cont);
    lv_textarea_set_text(pwd_textarea, "");
    lv_textarea_set_password_mode(pwd_textarea, true);
    lv_textarea_set_one_line(pwd_textarea, true);
    lv_obj_set_size(pwd_textarea, 200, WIFI_TEXTAREA_HEIGHT);
    lv_obj_add_event_cb(pwd_textarea, textarea_event_cb, LV_EVENT_ALL, wifi_keyboard);

    save_btn = lv_btn_create(cont);
    lv_obj_set_size(save_btn, 100, 50);
    lv_obj_add_style(save_btn, &btn_style, 0);
    save_btn_label = lv_label_create(save_btn);
    lv_obj_center(save_btn_label);
    lv_label_set_text(save_btn_label, "Save");
    lv_obj_add_event_cb(save_btn, setting_wifi_cb, LV_EVENT_ALL, NULL);
    
    back_btn = lv_btn_create(cont);
    lv_obj_set_size(back_btn, 100, 50);
    lv_obj_add_style(back_btn, &btn_style, 0);
    back_btn_label = lv_label_create(back_btn);
    lv_obj_center(back_btn_label);
    lv_label_set_text(back_btn_label, "Back");
    lv_obj_add_event_cb(back_btn, setting_wifi_cb, LV_EVENT_ALL, NULL);

    lv_group_add_obj(setting_wifi_group, title);
    lv_group_add_obj(setting_wifi_group, name_textarea);
    lv_group_add_obj(setting_wifi_group, pwd_textarea);    
    lv_group_add_obj(setting_wifi_group, save_btn);
    lv_group_add_obj(setting_wifi_group, back_btn);

    Serial.println("setting_wifi_init OK");
}

void setting_wifi(void)
{
    setting_wifi_group = lv_group_create();
    //lv_group_set_default(setting_wifi_group);
    lv_indev_set_group(indev_encoder, setting_wifi_group); 

    scr_setting_wifi = lv_scr_act();
    lv_obj_clean(scr_setting_wifi);

    setting_wifi_init();
}