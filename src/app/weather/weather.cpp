/*
 * @Author: WYJ
 * @Date: 2022-03-24 22:02:58
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 13:18:22
 * @FilePath: \littleVisual\src\app\weather\weather.cpp
 * @Description: 
 */

#include "lvgl.h"
#include "weather.h"
#include "config.h"
#include "app/apps.h"
#include "ArduinoJson.h"
#include "driver/network.h"
#include "driver/lv_timer.h"
#include "driver/encoder.h"
#include <map>


LV_FONT_DECLARE(ch_font20);
LV_FONT_DECLARE(lv_font_ibmplex_64);
LV_FONT_DECLARE(lv_font_ibmplex_115);

LV_IMG_DECLARE(weather_0);
LV_IMG_DECLARE(weather_9);
LV_IMG_DECLARE(weather_14);
LV_IMG_DECLARE(weather_5);
LV_IMG_DECLARE(weather_25);
LV_IMG_DECLARE(weather_30);
LV_IMG_DECLARE(weather_26);
LV_IMG_DECLARE(weather_11);
LV_IMG_DECLARE(weather_23);
LV_IMG_DECLARE(temp);
LV_IMG_DECLARE(humi);
LV_IMG_DECLARE(man_0);
LV_IMG_DECLARE(man_1);
LV_IMG_DECLARE(man_2);
LV_IMG_DECLARE(man_3);
LV_IMG_DECLARE(man_4);
LV_IMG_DECLARE(man_5);
LV_IMG_DECLARE(man_6);
LV_IMG_DECLARE(man_7);
LV_IMG_DECLARE(man_8);
LV_IMG_DECLARE(man_9);

extern lv_indev_t * indev_encoder;          // 旋转编码器，绑定组使用
extern ConfigStruct cfgstruct;

static hw_timer_t *timer_weather = NULL;

static lv_group_t *weather_group = NULL;

static lv_style_t scr_weather_style;
static lv_style_t chFont_style;
static lv_style_t numberSmall_style;
static lv_style_t numberBig_style;
static lv_style_t btn_style;
static lv_style_t bar_style;

static WeatherStructure weather_data;

static lv_obj_t *scr_weather;
static lv_obj_t *weatherImg = NULL;
static lv_obj_t *cityLabel = NULL;
static lv_obj_t *btn = NULL, *btnLabel = NULL;
static lv_obj_t *txtLabel = NULL;
static lv_obj_t *clockLabel_1 = NULL;
static lv_obj_t *clockLabel_2 = NULL;
static lv_obj_t *dateLabel = NULL;
static lv_obj_t *tempImg = NULL;
static lv_obj_t *tempBar = NULL;
static lv_obj_t *tempLabel = NULL;
static lv_obj_t *humiImg = NULL;
static lv_obj_t *humiBar = NULL;
static lv_obj_t *humiLabel = NULL;
static lv_obj_t *spaceImg = NULL;

// 天气图标路径的映射关系
const void *weaImage_map[] = {&weather_0, &weather_9, &weather_14, &weather_5, &weather_25,&weather_30, &weather_26, &weather_11, &weather_23};
// 太空人图标路径的映射关系
const void *manImage_map[] = {&man_0, &man_1, &man_2, &man_3, &man_4, &man_5, &man_6, &man_7, &man_8, &man_9};                      
const char weekDayCh[7][4] = {"日", "一", "二", "三", "四", "五", "六"};
const char airQualityCh[6][10] = {"优", "良", "轻度", "中度", "重度", "严重"};
std::map<String, int> weatherMap = {{"qing", 0}, {"yin", 1}, {"yu", 2}, {"yun", 3}, {"bingbao", 4}, {"wu", 5}, {"shachen", 6}, {"lei", 7}, {"xue", 8}};

// const char *STA_SSID = "wyj";   //热点名称
// const char *STA_PASS = "12345678"; //密码
static bool isFirsttime = true;

static void weather_init(void);
static void weather_style_init(void);
static void weather_display_init(void);
static long long IRAM_ATTR get_timestamp(void);
static long long get_timestamp(String url);
static int windLevelAnalyse(String str);
static int airQulityLevel(int q);
static void get_weather(void);
static void get_network_info(void);
static void scr_weather_cb(lv_event_t *e);
static void weather_style_reset(void);
static void weather_display(struct Weather weaInfo);
static void display_time(struct TimeStr timeInfo);
static void IRAM_ATTR UpdateTime_RTC(long long timestamp);
static void IRAM_ATTR Weather_auto_flush_time_cb(void);
static void weather_clean(void);

void weather(void)
{
#if MEM_MONITOR
    memory_print();
#endif

    scr_weather = lv_scr_act();
    lv_obj_clean(scr_weather);

    weather_group = lv_group_create();
    lv_indev_set_group(indev_encoder,weather_group);

    weather_init();
    weather_display_init();

    weather_display(weather_data.wea);

    timer_weather = timerBegin(1,80,true);
    timerAttachInterrupt(timer_weather, Weather_auto_flush_time_cb, true);
    timerAlarmWrite(timer_weather, AUTO_TIME_FLUSH_INTERVAL*1000, true);
    timerAlarmEnable(timer_weather);
}

static void weather_init(void)
{
    weather_style_init();

    // 初始化一些数据
    //weather_data = (WeatherStructure *)malloc(sizeof(WeatherStructure));
    if(isFirsttime)
    {
        strcpy(weather_data.wea.cityname, "杭州");
        strcpy(weather_data.wea.windDir, "东南");
        weather_data.wea.airQulity = 0;
        weather_data.wea.humidity = 60;
        weather_data.wea.maxTmep = 30;
        weather_data.wea.minTemp = 10;
        weather_data.wea.weather_code = 0;
        weather_data.wea.windLevel = 3;
        weather_data.wea.temperature = 25;

        weather_data.preNetTimestamp = 1577808000000; 
        weather_data.preLocalTimestamp = millis();     // 上一次的本地机器时间戳
        //isFirsttime = false;
    }
    else{
        get_timestamp();
    }
}

static void weather_style_init(void)
{
    lv_style_init(&scr_weather_style);
    lv_style_set_bg_color(&scr_weather_style, lv_color_black());

    lv_style_init(&chFont_style);
    lv_style_set_text_opa(&chFont_style, LV_OPA_100);
    lv_style_set_text_color(&chFont_style, lv_color_white());
    lv_style_set_text_font(&chFont_style, &ch_font20);

    lv_style_init(&numberSmall_style);
    lv_style_set_text_opa(&numberSmall_style, LV_OPA_100);
    lv_style_set_text_color(&numberSmall_style, lv_color_white());
    lv_style_set_text_font(&numberSmall_style, &lv_font_ibmplex_64);

    lv_style_init(&numberBig_style);
    lv_style_set_text_opa(&numberBig_style, LV_OPA_100);
    lv_style_set_text_color(&numberBig_style, lv_color_white());
    lv_style_set_text_font(&numberBig_style, &lv_font_ibmplex_115);

    lv_style_init(&btn_style);
    lv_style_set_border_width(&btn_style, 0);
    
    lv_style_init(&bar_style);
    lv_style_set_bg_color(&bar_style, lv_color_black());
    lv_style_set_border_width(&bar_style, 2);
    lv_style_set_border_color(&bar_style, lv_color_white());
    lv_style_set_pad_top(&bar_style, 1); //指示器到背景四周的距离
    lv_style_set_pad_bottom(&bar_style, 1);
    lv_style_set_pad_left(&bar_style, 1);
    lv_style_set_pad_right(&bar_style, 1);
}

static void weather_display_init(void)
{    
    // 屏幕样式
    lv_obj_add_style(scr_weather, &scr_weather_style, 0);

    weatherImg = lv_img_create(scr_weather);
    lv_img_set_src(weatherImg, weaImage_map[0]);

    // 城市图标
    cityLabel = lv_label_create(scr_weather);
    lv_obj_add_style(cityLabel, &chFont_style, LV_PART_MAIN);
    lv_label_set_recolor(cityLabel, true);
    lv_label_set_text(cityLabel, "上海");

    // 按钮
    btn = lv_btn_create(scr_weather);
    lv_obj_add_style(btn, &btn_style, LV_PART_MAIN);
    lv_obj_set_pos(btn, 75, 15);
    lv_obj_set_size(btn, 50, 25);
    lv_obj_set_style_bg_color(btn, lv_color_make(0xFF, 0xA5, 0x00), LV_PART_MAIN);  //橙色
    lv_group_add_obj(weather_group, btn);
    lv_obj_add_event_cb(btn, scr_weather_cb, LV_EVENT_ALL, NULL);
    lv_group_focus_obj(btn);

    btnLabel = lv_label_create(btn);
    lv_obj_align(btnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(btnLabel, &chFont_style, LV_PART_MAIN);
    lv_label_set_text(btnLabel, airQualityCh[0]);

    txtLabel = lv_label_create(scr_weather);
    lv_obj_add_style(txtLabel, &chFont_style, LV_PART_MAIN);
    lv_label_set_long_mode(txtLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(txtLabel, 150);
    lv_label_set_text_fmt(txtLabel, "最低气温%d°C, 最高气温%d°C, %s%d 级.   ", 15, 20, "西北风", 0);


    clockLabel_1 = lv_label_create(scr_weather);
    lv_obj_add_style(clockLabel_1, &numberBig_style, LV_PART_MAIN);
    lv_label_set_recolor(clockLabel_1, true);
    lv_label_set_text_fmt(clockLabel_1, "%02d#ffa500 %02d#", 16, 00);

    clockLabel_2 = lv_label_create(scr_weather);
    lv_obj_add_style(clockLabel_2, &numberSmall_style, LV_PART_MAIN);
    lv_label_set_recolor(clockLabel_2, true);
    lv_label_set_text_fmt(clockLabel_2, "%02d", 00);

    dateLabel = lv_label_create(scr_weather);
    lv_obj_add_style(dateLabel, &chFont_style, LV_PART_MAIN);
    lv_label_set_text_fmt(dateLabel, "%2d月%2d日   周%s", 11, 23, weekDayCh[1]);

    if(isFirsttime)
    {
        isFirsttime = false;
    }
    else{
        UpdateTime_RTC(get_timestamp());
    }

    tempImg = lv_img_create(scr_weather);
    lv_img_set_src(tempImg, &temp);
    lv_img_set_zoom(tempImg, 180);
    tempBar = lv_bar_create(scr_weather);
    lv_obj_add_style(tempBar, &bar_style, LV_PART_MAIN);
    lv_bar_set_range(tempBar, -50, 50);   // 设置进度条表示的温度为-20~50
    lv_obj_set_size(tempBar, 60, 12);
    lv_obj_set_style_bg_color(tempBar, lv_color_make(0xFF, 0x00, 0x00), LV_PART_INDICATOR);
    lv_bar_set_value(tempBar, 10, LV_ANIM_OFF);
    tempLabel = lv_label_create(scr_weather);
    lv_obj_add_style(tempLabel, &chFont_style, LV_PART_MAIN);
    lv_label_set_text_fmt(tempLabel, "%2d°C", 18);


    humiImg = lv_img_create(scr_weather);
    lv_img_set_src(humiImg, &humi);
    lv_img_set_zoom(humiImg, 180);
    humiBar = lv_bar_create(scr_weather);
    lv_obj_add_style(humiBar, &bar_style, LV_PART_MAIN);
    lv_bar_set_range(humiBar, 0, 100);
    lv_obj_set_size(humiBar, 60, 12);
    lv_obj_set_style_bg_color(humiBar, lv_color_make(0x00, 0x00, 0xFF), LV_PART_INDICATOR);
    lv_bar_set_value(humiBar, 49, LV_ANIM_OFF);
    humiLabel = lv_label_create(scr_weather);
    lv_obj_add_style(humiLabel, &chFont_style, LV_PART_MAIN);
    lv_label_set_text(humiLabel, "50%");

    // 太空人图标
    spaceImg = lv_img_create(scr_weather);
    lv_img_set_src(spaceImg, manImage_map[0]);

    // 绘制图形
    lv_obj_align(weatherImg, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_align(cityLabel, LV_ALIGN_TOP_LEFT, 20, 15);
    lv_obj_align(txtLabel, LV_ALIGN_TOP_LEFT, 0, 50);
    lv_obj_align(tempImg, LV_ALIGN_LEFT_MID, 10, 70);
    lv_obj_align(tempBar, LV_ALIGN_LEFT_MID, 35, 70);
    lv_obj_align(tempLabel, LV_ALIGN_LEFT_MID, 103, 70);
    lv_obj_align(humiImg, LV_ALIGN_LEFT_MID, 0, 100);
    lv_obj_align(humiBar, LV_ALIGN_LEFT_MID, 35, 100);    
    lv_obj_align(humiLabel, LV_ALIGN_LEFT_MID, 103, 100);
    lv_obj_align(spaceImg, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_align(clockLabel_1, LV_ALIGN_LEFT_MID, 0, 10);
    lv_obj_align(clockLabel_2, LV_ALIGN_LEFT_MID, 165, 9);
    lv_obj_align(dateLabel, LV_ALIGN_LEFT_MID, 10, 32);
}

static long long IRAM_ATTR get_timestamp(void)
{
    // 使用本地的机器时钟
    weather_data.preNetTimestamp = weather_data.preNetTimestamp + (millis() - weather_data.preLocalTimestamp);
    weather_data.preLocalTimestamp = millis();
    return weather_data.preNetTimestamp;   
}

static long long get_timestamp(String url)
{
    if (WL_CONNECTED != WiFi.status())
    {
        return 0;
        Serial.println("No wifi connected");
    }

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
            weather_data.preNetTimestamp = atoll(time.c_str()) + + TIMEZERO_OFFSIZE;
            weather_data.preLocalTimestamp = millis();
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        // 得不到网络时间戳时
        weather_data.preNetTimestamp = weather_data.preNetTimestamp + (millis() - weather_data.preLocalTimestamp);
        weather_data.preLocalTimestamp = millis();
    }
    http.end();

    return weather_data.preNetTimestamp;
}

static int windLevelAnalyse(String str)
{
    int ret = 0;
    for (char ch : str)
    {
        if (ch >= '0' && ch <= '9')
        {
            ret = ret * 10 + (ch - '0');
        }
    }
    return ret;
}

static int airQulityLevel(int q)
{
    if (q < 50)
    {
        return 0;
    }
    else if (q < 100)
    {
        return 1;
    }
    else if (q < 150)
    {
        return 2;
    }
    else if (q < 200)
    {
        return 3;
    }
    else if (q < 300)
    {
        return 4;
    }
    return 5;
}

static void get_weather(void)
{
    if (WL_CONNECTED != WiFi.status())
        return;

    // 知心天气
    String Address_ZH;
    HTTPClient http1;
    http1.setTimeout(1000);
    char api1[128] = {0};
    snprintf(api1, 128, ZHIXIN_WEATHER_API, "S-T7M_bB91yAn7ymA", cfgstruct.Address);     // 通过知心天气获得城市的中文名字
    Serial.print("Zhixin API = ");
    Serial.println(api1);
    http1.begin(api1);
    int httpCode1 = http1.GET();

    if (httpCode1 > 0)
    {
        if (httpCode1 == HTTP_CODE_OK || httpCode1 == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String response = http1.getString();
            Serial.println(response);
            DynamicJsonDocument doc1(1024);
            deserializeJson(doc1, response);
            JsonObject root = doc1.as<JsonObject>();
            JsonArray results = root["results"];
            JsonObject location = results[0]["location"];
            Address_ZH = location["name"].as<String>().c_str();
        }
    }
    else{
        Serial.printf("[HTTP] GET... failed, error: %s\n", http1.errorToString(httpCode1).c_str());
    }
    http1.end();
    
    // Yike 天气
    HTTPClient http2;
    http2.setTimeout(1000);
    char api2[128] = {0};
    snprintf(api2, 128, WEATHER_NOW_API_UPDATE, cfgstruct.appid, cfgstruct.appsecret , Address_ZH);
    Serial.print("Yike API = ");
    Serial.println(api2);
    http2.begin(api2);
    int httpCode2 = http2.GET();
    if (httpCode2 > 0)
    {
        // file found at server
        if (httpCode2 == HTTP_CODE_OK || httpCode2 == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = http2.getString();
            Serial.println(payload);
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            JsonObject sk = doc.as<JsonObject>();
            strcpy(weather_data.wea.cityname, sk["city"].as<String>().c_str());
            weather_data.wea.weather_code = weatherMap[sk["wea_img"].as<String>()];
            weather_data.wea.temperature = sk["tem"].as<int>();

            // 获取湿度
            weather_data.wea.humidity = 50;
            char humidity[8] = {0};
            strncpy(humidity, sk["humidity"].as<String>().c_str(), 8);
            humidity[strlen(humidity) - 1] = 0; // 去除尾部的 % 号
            weather_data.wea.humidity = atoi(humidity);

            weather_data.wea.maxTmep = sk["tem1"].as<int>();
            weather_data.wea.minTemp = sk["tem2"].as<int>();
            strcpy(weather_data.wea.windDir, sk["win"].as<String>().c_str());
            weather_data.wea.windLevel = windLevelAnalyse(sk["win_speed"].as<String>());
            weather_data.wea.airQulity = airQulityLevel(sk["air"].as<int>());
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http2.errorToString(httpCode2).c_str());
    }
    http2.end();
}

static void get_network_info(void)
{
    // 防止进中断， cached disabled
    timerAlarmDisable(timer_weather);
    lv_Timer_Stop();
    Encoder_Interrupt_en(false);
    search_wifi();
    
    start_conn_wifi(cfgstruct.Wifi_name.c_str(), (char*)cfgstruct.Wifi_pwd.c_str());
    WiFi.status();
    delay(2000);

    if( WiFi.status() == WL_CONNECTED )
    {
        Serial.println("Connect SUCC");
        get_timestamp(TIME_API);
        get_weather(); 
        weather_display(weather_data.wea);
    }
    else{
        Serial.println("Connect ERROR");
    }

    close_wifi();
    timerAlarmEnable(timer_weather);
    lv_Timer_Start();
    Encoder_Interrupt_en(true);
}

static void scr_weather_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);    // 获取触发事件
    if(code == LV_EVENT_LONG_PRESSED)
    {
        Serial.println("LONG PRESSED!!!!!!!!");
        if (NULL != scr_weather)
        {
            weather_clean();
            scr_weather = NULL;
        }
        timerAlarmDisable(timer_weather);
        timerEnd(timer_weather);
        close_wifi();
        APP_selector();
    }
    else if(code == LV_EVENT_CLICKED)
    {
        Serial.println("CLICKED!!!!");
        get_network_info();
    }
}

static void weather_style_reset(void)
{
    lv_style_reset(&scr_weather_style);
    lv_style_reset(&chFont_style);
    lv_style_reset(&numberSmall_style);
    lv_style_reset(&numberBig_style);
    lv_style_reset(&btn_style);
    lv_style_reset(&bar_style);
}

static void weather_display(struct Weather weaInfo)
{
    lv_label_set_text(cityLabel, weaInfo.cityname);
    lv_label_set_text(btnLabel, airQualityCh[weaInfo.airQulity]);
    // 下面这行代码可能会出错
    lv_label_set_text_fmt(txtLabel, "最低气温%d°C, 最高气温%d°C, %s%d 级.   ",weaInfo.minTemp, weaInfo.maxTmep, weaInfo.windDir, weaInfo.windLevel);
    lv_label_set_long_mode(txtLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_img_set_src(weatherImg, weaImage_map[weaInfo.weather_code]);
    
    lv_bar_set_value(tempBar, weaInfo.temperature, LV_ANIM_OFF);
    lv_label_set_text_fmt(tempLabel, "%2d°C", weaInfo.temperature);
    lv_bar_set_value(humiBar, weaInfo.humidity, LV_ANIM_OFF);
    lv_label_set_text_fmt(humiLabel, "%d%%", weaInfo.humidity);

    lv_scr_load(scr_weather); 
}

static void display_time(struct TimeStr timeInfo)
{
    //display_weather_init(anim_type);
    lv_label_set_text_fmt(clockLabel_1, "%02d#ffa500 %02d#", timeInfo.hour, timeInfo.minute);
    lv_label_set_text_fmt(clockLabel_2, "%02d", timeInfo.second);
    lv_label_set_text_fmt(dateLabel, "%2d月%2d日   周%s", timeInfo.month, timeInfo.day,
                          weekDayCh[timeInfo.weekday]);
}

static void IRAM_ATTR UpdateTime_RTC(long long timestamp)
{
    struct TimeStr t;
    weather_data.g_rtc.setTime(timestamp / 1000);
    t.month = weather_data.g_rtc.getMonth() + 1;
    t.day = weather_data.g_rtc.getDay();
    t.hour = weather_data.g_rtc.getHour(true);
    t.minute = weather_data.g_rtc.getMinute();
    t.second = weather_data.g_rtc.getSecond();
    t.weekday = weather_data.g_rtc.getDayofWeek();
    display_time(t);
}

static int DRAM_ATTR Spaceman_counter = 1;
static void IRAM_ATTR Weather_auto_flush_time_cb(void)
{
    if(Spaceman_counter == 0)
    {
        UpdateTime_RTC(get_timestamp());
    }
    lv_img_set_src(spaceImg, manImage_map[Spaceman_counter]);
    Spaceman_counter = (Spaceman_counter+1) % 10;
}

static void weather_clean(void)
{
    lv_obj_clean(scr_weather);
    weather_style_reset();
    lv_group_del(weather_group);
}