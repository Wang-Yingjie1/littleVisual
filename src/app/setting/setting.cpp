/*
 * @Author: WYJ
 * @Date: 2022-03-29 18:11:43
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 15:22:21
 * @FilePath: \littleVisual\src\app\setting\setting.cpp
 * @Description: 
 */
#include "setting.h"
#include "lvgl.h"
#include "Arduino.h"
#include "config.h"
#include "app/apps.h"

LV_FONT_DECLARE(lv_font_montserrat_22);
extern lv_indev_t * indev_encoder;
static lv_group_t *setting_group;

static lv_obj_t * scr_setting;
static lv_obj_t * setting_btn[SETTING_NUM];
static lv_obj_t * setting_label[SETTING_NUM];
static lv_obj_t * cont;

static lv_style_t setting_cont_style;
static lv_style_t setting_btn_style;

const char * settings[SETTING_NUM] = 
{
    "WiFi",
    "Weather",
    "BackLight",
    "Beep",
    "Disp Rotation",
    "Album"
};

static void setting_init(void);
static void setting_style_init(void);
static void settings_cb(lv_event_t * e);
static void setting_clean(void);
static void setting_style_reset(void);

static void setting_clean(void)
{
    lv_obj_clean(scr_setting);
    setting_style_reset();
    lv_group_del(setting_group);
}

static void setting_style_reset(void)
{
    lv_style_reset(&setting_cont_style);
    lv_style_reset(&setting_btn_style);
}

static void settings_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);    // 获取触发事件
    lv_obj_t *obj = lv_event_get_target(e);         // 获取触发obj

    if(code == LV_EVENT_LONG_PRESSED)
    {
        setting_clean();
        APP_selector();
    }
    else if(code == LV_EVENT_CLICKED)
    {
        if(obj == setting_btn[0])
        {
            setting_clean();
            setting_wifi();
        }
        else if(obj == setting_btn[1])
        {
            setting_clean();
            setting_wea();
        }
        else if(obj == setting_btn[2])
        {
            setting_clean();
            setting_bl();
        }
        else if(obj == setting_btn[3])
        {
            setting_clean();
            setting_beep();
        }
        else if(obj == setting_btn[4])
        {
            setting_clean();
            setting_disp();
        }
        else if(obj == setting_btn[5])
        {
            setting_clean();
            setting_album();
        }
    }

}
static void setting_style_init(void)
{

    lv_style_init(&setting_cont_style);
    lv_style_set_bg_color(&setting_cont_style, lv_color_white());

    lv_style_init(&setting_btn_style);
    lv_style_set_bg_color(&setting_btn_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_color(&setting_btn_style, lv_color_black());
    lv_style_set_text_opa(&setting_btn_style, LV_OPA_100);
    lv_style_set_text_font(&setting_btn_style, &lv_font_montserrat_22);
}

static void setting_init(void)
{
    setting_style_init();

        /* 一个容器 */
    cont = lv_obj_create(scr_setting);
    lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);      // 实现滚动效果
    lv_obj_add_style(cont, &setting_cont_style, LV_PART_MAIN);

    int i = 0;
    for(i = 0; i < SETTING_NUM; i++)
    {
        setting_btn[i] = lv_btn_create(cont);
        lv_obj_set_size(setting_btn[i], SETTING_BTN_WIDTH, SETTING_BTN_HEIGHT);
        lv_obj_add_style(setting_btn[i], &setting_btn_style, 0);
        lv_obj_center(setting_btn[i]);

        setting_label[i] = lv_label_create(setting_btn[i]);
        lv_obj_center(setting_label[i]);
        lv_label_set_text(setting_label[i], settings[i]);

        lv_group_add_obj(setting_group, setting_btn[i]);
        lv_obj_add_event_cb(setting_btn[i], settings_cb, LV_EVENT_ALL, NULL);
    }  
}
void setting(void)
{
    scr_setting = lv_scr_act();
    lv_obj_clean(scr_setting);
    
    /* 添加组，encoder必须将部件加入组，并与indev绑定 */
    setting_group = lv_group_create();
    lv_indev_set_group(indev_encoder, setting_group);  // 重要，必须添加

    setting_init();
}


// static void settings_cb(lv_event_t *e);
// static void WiFi_setting(void);
// // static void Adress_setting(lv_obj_t *obj);
// // static void BlackLight_setting(lv_obj_t *obj);
// // static void Beep_setting(lv_obj_t *obj);
// // static void Sleep_setting(lv_obj_t * obj);

// static void settings_cb(lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);    // 获取触发事件
//     lv_obj_t *obj = lv_event_get_target(e);         // 获取触发obj
    
//     if( code == LV_EVENT_LONG_PRESSED)
//     {
//         lv_obj_clean(scr_setting);
//         APP_selector();
//     }
//     else if( code == LV_EVENT_CLICKED)
//     {
//         if(obj == setting_btn[0])
//         {
//             lv_obj_clean(scr_setting);
//             WiFi_setting();
//         }
//     }
// }

// static void setting_style_init(void)
// {
//     lv_style_init(&setting_cont_style);
//     lv_style_set_bg_color(&setting_cont_style, lv_color_black());

//     lv_style_init(&setting_btn_style);
//     lv_style_set_bg_color(&setting_btn_style, lv_palette_main(LV_PALETTE_GREY));
//     lv_style_set_text_color(&setting_btn_style, lv_color_black());
//     lv_style_set_text_opa(&setting_btn_style, LV_OPA_100);
//     lv_style_set_text_font(&setting_btn_style, &lv_font_montserrat_22);

// }
// static void setting_init(void)
// {
//     setting_style_init();

//     lv_obj_clean(scr_setting);

//     cont = lv_obj_create(scr_setting);

//     lv_group_remove_all_objs(setting_group);
    
//     lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);
//     lv_obj_center(cont);
//     lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);

//     int i = 0;
//     for(i = 0; i < SETTING_NUM; i++)
//     {
//         setting_btn[i] = lv_btn_create(cont);
//         lv_obj_set_size(setting_btn[i], SETTING_BTN_WIDTH, SETTING_BTN_HEIGHT);
//         lv_obj_add_style(setting_btn[i], &setting_btn_style, 0);
//         lv_obj_center(setting_btn[i]);
//         setting_label[i] = lv_label_create(setting_btn[i]);
//         lv_obj_center(setting_label[i]);
//         lv_label_set_text(setting_label[i], settings[i]);

//         lv_group_add_obj(setting_group, setting_btn[i]);
//         lv_obj_add_event_cb(setting_btn[i], settings_cb, LV_EVENT_ALL, NULL);
//     }
// }

// void setting(void)
// {
//     setting_group = lv_group_create();
//     // setting_group2 = lv_group_create();
//     lv_indev_set_group(indev_encoder, setting_group);
    
//     scr_setting = lv_scr_act();
//     // scr2_setting = lv_obj_create(NULL);
//     lv_obj_clean(scr_setting);

//     setting_init();
// }


// static void setting_back_cb(lv_event_t * e)
// {
//     lv_obj_clean(scr_setting);
//     setting_init();
// }

// static void WiFi_setting(void)
// {
//     lv_obj_clean(scr_setting);

//     cont = lv_obj_create(scr_setting);

//     lv_group_remove_all_objs(setting_group);

//     lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);

//     setting_btn[0] = lv_btn_create(cont);
//     lv_obj_set_size(setting_btn[0], SETTING_BTN_WIDTH, SETTING_BTN_HEIGHT);
//     lv_obj_add_style(setting_btn[0], &setting_btn_style, 0);
//     lv_obj_center(setting_btn[0]);

//     lv_obj_center(setting_label[0]);
//     lv_label_set_text(setting_label[0], settings[0]);
//     lv_obj_add_event_cb(setting_btn[0], setting_back_cb, LV_EVENT_CLICKED, NULL);

//     lv_group_add_obj(setting_group, setting_btn[0]);
// }

// static void Sleep_setting(lv_obj_t * obj)
// {
//     scr2_setting = lv_obj_create(NULL);
//     lv_obj_t *cont = lv_obj_create(scr2_setting);
//     lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);

//     lv_obj_t * sleep_btn = lv_btn_create(cont);
//     lv_obj_set_size(sleep_btn, lv_obj_get_width(obj), lv_obj_get_height(obj));
//     lv_obj_set_y(sleep_btn, lv_obj_get_y(obj));
//     lv_obj_add_style(sleep_btn, &setting_btn_style, 0);
//     lv_obj_add_event_cb(sleep_btn, setting_back_cb, LV_EVENT_ALL, NULL);

//     lv_obj_t *label = lv_label_create(sleep_btn);
//     lv_obj_center(label);
//     lv_label_set_text(label, "Sleep");

//     lv_scr_load(scr2_setting);

//     //set_y_anim(sleep_btn, 0);

// }

// static void Beep_setting(lv_obj_t *obj)
// {
//     scr2_setting = lv_obj_create(NULL);
//     lv_obj_t *cont = lv_obj_create(scr2_setting);
//     lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);

//     lv_obj_t * beep_btn = lv_btn_create(cont);
//     lv_obj_set_size(beep_btn, lv_obj_get_width(obj), lv_obj_get_height(obj));
//     lv_obj_set_y(beep_btn, lv_obj_get_y(obj));
//     lv_obj_add_style(beep_btn, &setting_btn_style, 0);
//     lv_obj_add_event_cb(beep_btn, setting_back_cb, LV_EVENT_ALL, NULL);

//     lv_obj_t *label = lv_label_create(beep_btn);
//     lv_obj_center(label);
//     lv_label_set_text(label, "Beep");

//     lv_scr_load(scr2_setting);

//     //set_y_anim(beep_btn, 0);
// }

// static void BlackLight_setting(lv_obj_t *obj)
// {
//     scr2_setting = lv_obj_create(NULL);
//     lv_obj_t *cont = lv_obj_create(scr2_setting);
//     lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);

//     lv_obj_t * bl_btn = lv_btn_create(cont);
//     lv_obj_set_size(bl_btn, lv_obj_get_width(obj), lv_obj_get_height(obj));
//     lv_obj_set_y(bl_btn, lv_obj_get_y(obj));
//     lv_obj_add_style(bl_btn, &setting_btn_style, 0);
//     lv_obj_add_event_cb(bl_btn, setting_back_cb, LV_EVENT_ALL, NULL);

//     lv_obj_t *label = lv_label_create(bl_btn);
//     lv_obj_center(label);
//     lv_label_set_text(label, "BlackLight");

//     lv_scr_load(scr2_setting);

//     //set_y_anim(bl_btn, 0);
// }

// static void Adress_setting(lv_obj_t *obj)
// {
//     scr2_setting = lv_obj_create(NULL);
//     lv_obj_t *cont = lv_obj_create(scr2_setting);
//     lv_obj_set_size(cont, CONFIG_SCREEN_HOR_RES, CONFIG_SCREEN_VER_RES);

//     lv_obj_t * addr_btn = lv_btn_create(cont);
//     lv_obj_set_size(addr_btn, lv_obj_get_width(obj), lv_obj_get_height(obj));
//     lv_obj_set_y(addr_btn, lv_obj_get_y(obj));
//     lv_obj_add_style(addr_btn, &setting_btn_style, 0);
//     lv_obj_add_event_cb(addr_btn, setting_back_cb, LV_EVENT_ALL, NULL);

//     lv_obj_t *label = lv_label_create(addr_btn);
//     lv_obj_center(label);
//     lv_label_set_text(label, "Address");

//     lv_scr_load(scr2_setting);

//     //set_y_anim(addr_btn, 0);
// }
