/*
 * @Author: WYJ
 * @Date: 2022-04-01 22:32:53
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-01 23:52:24
 * @FilePath: \littleVisual\src\app\calendar\calendar.cpp
 * @Description: 
 */

#include "calendar.h"
#include "lvgl.h"
#include "app/apps.h"
#include "driver/network.h"
#include "config.h"
#include "ESP32Time.h"

extern lv_indev_t * indev_encoder;
extern ConfigStruct cfgstruct;
LV_FONT_DECLARE(lv_font_montserrat_22);
LV_FONT_DECLARE(lv_font_montserrat_32);

static lv_obj_t * scr_calendar = NULL;
static lv_group_t * calendar_group;

static lv_obj_t * title;
static lv_obj_t * titleLabel;
static lv_obj_t * calendar_obj;

static lv_style_t title_style;
static lv_style_t title_label_style;
// static lv_style_t calendar_style;

static long long NetTimestamp = 1577808000000;
static int year = 0, month = 0, day = 0;

static void calendar_init(void);
static void calendar_style_init(void);
static void title_btn_cb(lv_event_t * e);
static void calendar_clean(void);
static void calendar_style_reset(void);
static void get_date(String url);

void calendar(void)
{
    calendar_group = lv_group_create();
    lv_indev_set_group(indev_encoder, calendar_group);
    lv_group_set_default(calendar_group);

    scr_calendar = lv_scr_act();
    lv_obj_clean(scr_calendar);

    calendar_init();
}

static void calendar_init(void)
{
    calendar_style_init();

    title = lv_btn_create(scr_calendar);
    lv_obj_add_style(title, &title_style, LV_PART_MAIN);
    lv_obj_set_size(title, 220, 50);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);
    titleLabel = lv_label_create(title);
    lv_obj_align(titleLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(titleLabel, "Calendar");
    lv_obj_add_style(titleLabel, &title_label_style, LV_PART_MAIN);
    lv_obj_add_event_cb(title, title_btn_cb, LV_EVENT_ALL, NULL);

    calendar_obj = lv_calendar_create(scr_calendar);
    lv_obj_set_size(calendar_obj, 230, 180);
    lv_obj_align(calendar_obj, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_calendar_set_today_date(calendar_obj, 2021, 02, 23);
    lv_calendar_set_showed_date(calendar_obj, 2021, 02);
    lv_calendar_header_dropdown_create(calendar_obj);

    //lv_group_add_obj(calendar_group, title);
    //lv_group_add_obj(calendar_group, calendar_obj);
}

static void calendar_style_init(void)
{
    lv_style_init(&title_style);
    lv_style_set_bg_color(&title_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_color(&title_style, lv_color_black());
    lv_style_set_opa(&title_style, LV_OPA_100);

    lv_style_init(&title_label_style);
    lv_style_set_text_opa(&title_label_style, LV_OPA_100);
    lv_style_set_text_font(&title_label_style, &lv_font_montserrat_32);
}

static void title_btn_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED)
    {
        lv_obj_t * label = lv_label_create(scr_calendar);
        lv_label_set_text(label, "Getting date from Internet...");
        lv_obj_center(label);

        get_date("http://api.m.taobao.com/rest/api3.do?api=mtop.common.gettimestamp");
        lv_calendar_set_today_date(calendar_obj, year, month, day);
        lv_calendar_set_showed_date(calendar_obj, year, month);

        lv_obj_del(label);
    }
    else if(code == LV_EVENT_LONG_PRESSED)
    {
        calendar_clean();
        APP_selector();
    }
}

static void calendar_clean(void)
{
    lv_obj_clean(scr_calendar);
    calendar_style_reset();
    lv_group_del(calendar_group);
    
}
static void calendar_style_reset(void)
{
    lv_style_reset(&title_style);
    lv_style_reset(&title_label_style);
}

static void get_date(String url)
{
    search_wifi();
    start_conn_wifi(cfgstruct.Wifi_name.c_str(), (char*)cfgstruct.Wifi_pwd.c_str());
    WiFi.status();
    delay(2000);
    
    if( WiFi.status() == WL_CONNECTED )
    {
        String time = "";
        HTTPClient http;
        http.setTimeout(1000);
        Serial.println("HTTP Begin");
        http.begin(url);

        int httpCode = http.GET();
        Serial.println("HTTP Get OK");
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                Serial.println(payload);
                int time_index = (payload.indexOf("data")) + 12;
                time = payload.substring(time_index, payload.length() - 3);
                // 以网络时间戳为准
                NetTimestamp = atoll(time.c_str()) + + TIMEZERO_OFFSIZE;    // TIMEZERO_OFFSIZE: +8时区，8*3600*1000
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    else
    {
        Serial.println("No wifi connected");
    }

    ESP32Time g_rtc;
    g_rtc.setTime(NetTimestamp/1000);
    year = g_rtc.getYear();
    month = g_rtc.getMonth() + 1;
    day = g_rtc.getDay();
}