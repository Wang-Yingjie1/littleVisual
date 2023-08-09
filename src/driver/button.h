/*
 * @Author: WYJ
 * @Date: 2022-03-24 12:32:36
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-24 13:35:41
 * @FilePath: \littleVisual\src\driver\button.h
 * @Description: 
 */

#ifndef BUTTON_H
#define BUTTON_H

#include "config.h"

#define BUTTION0 CONFIG_ENCODER_PUSH_PIN

bool Button_GetIsPush(void);

#endif