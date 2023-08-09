/*
 * @Author: WYJ
 * @Date: 2022-03-24 10:28:45
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 14:44:10
 * @FilePath: \littleVisual\src\driver\display.cpp
 * @Description: 
 */

#include "display.h"
#include "TFT_eSPI.h"
#include "driver/blacklight.h"
#include "config.h"

extern TFT_eSPI screen;
extern ConfigStruct cfgstruct;

void Display_init(void)
{
    screen.begin(0);
    screen.setRotation(cfgstruct.DispRotation);
    screen.fillScreen(TFT_BLACK);
    Backlight_SetValue(cfgstruct.Backlight);
}