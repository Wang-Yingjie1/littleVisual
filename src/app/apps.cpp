/*
 * @Author: WYJ
 * @Date: 2022-03-24 11:40:08
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 17:10:08
 * @FilePath: \littleVisual\src\app\apps.cpp
 * @Description: 
 */
#include "apps.h"
#include "lvgl.h"
#include "lv_port/lv_port_indev.h"
#include "Arduino.h"

LV_FONT_DECLARE(lv_font_montserrat_22);  
extern lv_indev_t * indev_encoder;          // 旋转编码器，绑定组使用
static lv_group_t *apps_group;

static unsigned long createtime = 0;

static lv_obj_t * scr_apps;                  // app 选择器的屏幕
static lv_obj_t * apps[APP_NUM];            // app 对象指针
static lv_obj_t * apps_label[APP_NUM];       // app 的名字对象
static lv_obj_t * cont;

static lv_style_t scr_apps_style;           // 屏幕的样式
static lv_style_t cont_style;
static lv_style_t apps_label_style;         // app 名字的样式
static lv_style_t apps_style[APP_NUM];      

lv_color_t apps_color[APP_NUM] =            // APP图标的颜色
{
    LV_COLOR_BLUE,
    LV_COLOR_CYAN,
    LV_COLOR_ORANGE,
    LV_COLOR_YELLOW,
    LV_COLOR_RED,
    LV_COLOR_GREEN,
    LV_COLOR_SILVER,
    LV_COLOR_OLIVE
};

int apps_width[APP_NUM] = {                 // app图标宽度设置，一行两个，宽度和为200，高度设置在头文件           
    120, 80,
    80, 120,
    120, 80,
    90, 110
};

const char* apps_name[APP_NUM] = {          // app 名字
    "Weather",  "Album",
    "Tictac", "Calendar",
    "Bilibili", "Video",
    "Game", "Setting"
};


static void apps_style_reset(void);
static void apps_init(void);
static void apps_style_init(void);
static void apps_clean(void);
static void apps_selector_cb(lv_event_t * e);

/**
 * @description: app选择器，管理app
 * @param {*}
 * @return {*}
 */
void APP_selector(void)
{
#if MEM_MONITOR
    memory_print();
#endif
    createtime = millis();
    /* 创建一个无边框的屏幕样式 */
    scr_apps = lv_scr_act(); // 创建一个屏幕，作为app选择器的界面 
    lv_obj_clean(scr_apps);

    /* 添加组，encoder必须将部件加入组，并与indev绑定 */
    apps_group = lv_group_create();
    lv_indev_set_group(indev_encoder, apps_group);  // 重要，必须添加
    
    apps_init();
}

static void apps_init(void)
{
    apps_style_init();

    /* 一个容器 */
    cont = lv_obj_create(scr_apps);
    lv_obj_set_size(cont, CONT_HOR_RES, CONT_VER_RES);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);      // 实现滚动效果
    lv_obj_add_style(cont, &cont_style, 0);

    for(int i = 0; i < APP_NUM; i++)
    {
        /* 添加图标样式 */
        apps[i] = lv_btn_create(cont);
        lv_obj_set_size(apps[i], apps_width[i], APP_HEIGHT);
        lv_obj_add_style(apps[i], &apps_style[i], LV_PART_MAIN);
        /* 添加名字 */
        apps_label[i] = lv_label_create(apps[i]);
        lv_label_set_long_mode(apps_label[i], LV_LABEL_LONG_WRAP);
        lv_label_set_text(apps_label[i], apps_name[i]);
        lv_obj_center(apps_label[i]);
        lv_obj_add_style(apps_label[i], &apps_label_style, LV_PART_MAIN);

        lv_group_add_obj(apps_group, apps[i]);      // 加入组
        lv_obj_add_event_cb(apps[i], apps_selector_cb, LV_EVENT_ALL, NULL); // 添加回调函数
    }
}

static void apps_style_init(void)
{
    lv_style_init(&scr_apps_style);
    lv_style_set_bg_color(&scr_apps_style, lv_color_black());
    lv_obj_add_style(scr_apps, &scr_apps_style, LV_PART_MAIN);
    
    lv_style_init(&apps_label_style);
    lv_style_set_text_opa(&apps_label_style, LV_OPA_COVER);
    lv_style_set_text_color(&apps_label_style, lv_color_make(0,0,0));
    lv_style_set_text_font(&apps_label_style, &lv_font_montserrat_22);
    
    lv_style_init(&cont_style);
    lv_style_set_bg_color(&cont_style, lv_color_black());
    lv_style_set_border_color(&cont_style, lv_color_black());

    for (int i = 0; i < APP_NUM; i++)
    {
        lv_style_init(&apps_style[i]);
        lv_style_set_radius(&apps_style[i], 10);
        lv_style_set_border_width(&apps_style[i], 0);
        lv_style_set_bg_color(&apps_style[i], apps_color[i]);
    }
}

/**
 * @description: app选择器的回调函数，实现切换页面，进入app
 * @param {lv_event_t *} e 
 * @return {*}
 */
static void apps_selector_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);    // 获取触发事件
    lv_obj_t *obj = lv_event_get_target(e);         // 获取触发obj
    
    if( millis() - createtime > 1000)       // 防止app回到该界面的时候连着就关机了
    {
        if(code == LV_EVENT_CLICKED){
            Serial.println("CLICKED!!!!!!!!");
            if(obj == apps[0])
            {
                apps_clean();
                weather();
            }
            else if(obj == apps[1])
            {
                apps_clean();
                album();
            }
            else if(obj == apps[2])
            {
                apps_clean();
                tictac();
            }
            else if(obj == apps[3])
            {
                apps_clean();
                calendar();
            }
            // else if(obj == apps[6])
            // {
            //     apps_clean();
            //     game();
            // }
            // else if(obj == apps[5])
            // {
            //     apps_clean();
            //     video();
            // }
            else if(obj == apps[7])
            {
                apps_clean();
                setting();
            }
        }
        else if(code == LV_EVENT_LONG_PRESSED)
        {
            Serial.println("LONG PRESSED!!!!!!!!");
            Power_Shutdown();
        }
    }
}

// 为切换页面做准备
static void apps_clean(void)
{
    lv_obj_clean(scr_apps);
    apps_style_reset();
    lv_group_del(apps_group);
}

static void apps_style_reset(void)
{  
    lv_style_reset(&scr_apps_style);
    lv_style_reset(&cont_style);
    lv_style_reset(&apps_label_style);
    for(int i = 0; i < APP_NUM; i++)
    {
        lv_style_reset(&apps_style[i]);
    }
}



