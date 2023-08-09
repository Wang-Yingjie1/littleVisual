/*
 * @Author: your name
 * @Date: 2022-03-24 11:40:24
 * @LastEditTime: 2022-04-03 17:10:40
 * @LastEditors: WYJ
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \littleVisual\src\app\apps.h
 */

#ifndef APPS_H
#define APPS_H

#include "config.h"

#include "driver/power.h"

#include "weather/weather.h"
#include "album/album.h"
#include "weather/weather.h"
#include "tictac/tictac.h"
#include "calendar/calendar.h"
// #include "video/video.h"
// #include "game/game.h"
#include "setting/setting.h"

#define CONT_HOR_RES        CONFIG_SCREEN_HOR_RES
#define CONT_VER_RES        CONFIG_SCREEN_VER_RES

#define APP_NUM             8
#define APP_HEIGHT          65

#define LV_COLOR_WHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define LV_COLOR_SILVER LV_COLOR_MAKE(0xC0, 0xC0, 0xC0)
#define LV_COLOR_GRAY LV_COLOR_MAKE(0x80, 0x80, 0x80)
#define LV_COLOR_BLACK LV_COLOR_MAKE(0x00, 0x00, 0x00)
#define LV_COLOR_RED LV_COLOR_MAKE(0xFF, 0x00, 0x00)
#define LV_COLOR_MAROON LV_COLOR_MAKE(0x80, 0x00, 0x00)
#define LV_COLOR_YELLOW LV_COLOR_MAKE(0xFF, 0xFF, 0x00)
#define LV_COLOR_OLIVE LV_COLOR_MAKE(0x80, 0x80, 0x00)
#define LV_COLOR_LIME LV_COLOR_MAKE(0x00, 0xFF, 0x00)
#define LV_COLOR_GREEN LV_COLOR_MAKE(0x00, 0x80, 0x00)
#define LV_COLOR_CYAN LV_COLOR_MAKE(0x00, 0xFF, 0xFF)
#define LV_COLOR_AQUA LV_COLOR_CYAN
#define LV_COLOR_TEAL LV_COLOR_MAKE(0x00, 0x80, 0x80)
#define LV_COLOR_BLUE LV_COLOR_MAKE(0x00, 0x00, 0xFF)
#define LV_COLOR_NAVY LV_COLOR_MAKE(0x00, 0x00, 0x80)
#define LV_COLOR_MAGENTA LV_COLOR_MAKE(0xFF, 0x00, 0xFF)
#define LV_COLOR_PURPLE LV_COLOR_MAKE(0x80, 0x00, 0x80)
#define LV_COLOR_ORANGE LV_COLOR_MAKE(0xFF, 0xA5, 0x00)

void APP_selector(void);

#endif