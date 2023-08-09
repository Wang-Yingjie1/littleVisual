/*
 * @Author: WYJ
 * @Date: 2022-03-24 12:32:29
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-28 00:13:02
 * @FilePath: \littleVisual\src\driver\button.cpp
 * @Description: 
 */

#include "Arduino.h"
#include "button.h"

bool Button_GetIsPush(void)
{
    return (digitalRead(BUTTION0) == LOW);
}