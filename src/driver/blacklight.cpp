/*
 * @Author: WYJ
 * @Date: 2022-03-24 10:17:01
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-28 00:12:39
 * @FilePath: \littleVisual\src\driver\blacklight.cpp
 * @Description: 
 */


#include <Arduino.h>
#include "blacklight.h"
#include "config.h"
//#include "lvgl.h"

static uint32_t backlightValue = 0;

/**
 * @description: 背光初始化
 * @param {*}
 * @return {*}
 */
void Backlight_Init(void)
{
    // PWM 初始化，5kHz， 1024级
    ledcSetup(0, 5000, 10);
    ledcAttachPin(CONFIG_SCREEN_BLK_PIN, 0);
    ledcWrite(0, 0);
}

/**
 * @description: 渐变背光效果设置
 * @param {uint32_t} target: 目标亮度 0~1000 -> 0 ~ 100%
 * @param {uint16_t} time
 * @return {*}
 */

void Backlight_SetValue(int32_t val)
{
    val = constrain(val, 0, 1024);
    backlightValue = val;
    val = backlightValue;
    ledcWrite(0, val);
}
