/*
 * @Author: WYJ
 * @Date: 2022-03-16 11:49:03
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-28 00:12:52
 * @FilePath: \littleVisual\src\driver\blacklight.h
 * @Description: 
 * 
 */

#ifndef BLACKLIGHT_H
#define BLACKLIGHT_H

#include <Arduino.h>

void Backlight_Init(void);
void Backlight_SetValue(int32_t val);


#endif