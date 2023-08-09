/*
 * @Author: WYJ
 * @Date: 2022-03-24 13:26:30
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 12:55:03
 * @FilePath: \littleVisual\src\driver\lv_timer.cpp
 * @Description: 
 */

#include "Arduino.h"
#include "lv_timer.h"
#include "lvgl.h"

static hw_timer_t *timer = NULL;

static void IRAM_ATTR Timer_cb(void)
{
  lv_tick_inc(5);
}

void lv_Timer_init(void)
{
  timer = timerBegin(0,80,true);      //定时器为80MHz
  timerAttachInterrupt(timer, Timer_cb, true);
  timerAlarmWrite(timer, 5000, true);
  timerAlarmEnable(timer);
}

void lv_Timer_Stop(void)
{
  timerAlarmDisable(timer);
}

void lv_Timer_Start(void)
{
  timerAlarmEnable(timer);
}