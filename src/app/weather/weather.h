/*
 * @Author: WYJ
 * @Date: 2022-03-24 22:03:05
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-01 23:29:45
 * @FilePath: \littleVisual\src\app\weather\weather.h
 * @Description: 
 */

#ifndef WEATHER_H
#define WEATHER_H

#include "ESP32Time.h"

#define AUTO_TIME_FLUSH_INTERVAL 100  //ms
#define TIME_API "http://api.m.taobao.com/rest/api3.do?api=mtop.common.gettimestamp"
#define ZHIXIN_WEATHER_API "https://api.seniverse.com/v3/weather/now.json?key=%s&location=%s&language=zh-Hans&unit=c"
#define WEATHER_NOW_API "https://www.yiketianqi.com/free/day?appid=%s&appsecret=%s&unescape=1&city=%s" // &city=%s
#define WEATHER_NOW_API_UPDATE "https://yiketianqi.com/api?unescape=1&version=v6&appid=%s&appsecret=%s&city=%s"
#define TIMEZERO_OFFSIZE  28800000  // 时区偏移8小时
struct Weather
{
    int weather_code; // 天气现象代码
    int temperature;  // 温度
    int humidity;     // 湿度
    int maxTmep;      // 最高气温
    int minTemp;      // 最低气温
    char windDir[20];
    char cityname[10]; // 城市名
    int windLevel;
    int airQulity;

    short daily_max[7];
    short daily_min[7];
};

struct TimeStr
{
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int weekday;
};

struct WeatherStructure{
    long long preNetTimestamp;           // 上一次的网络时间戳
    long long preLocalTimestamp;         // 上一次的本地机器时间戳

    ESP32Time g_rtc; // 用于时间解码
    Weather wea;     // 保存天气状况
};

void weather(void);

#endif