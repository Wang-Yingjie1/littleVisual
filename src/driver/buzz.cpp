/*
 * @Author: WYJ
 * @Date: 2022-03-28 15:37:26
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 13:18:02
 * @FilePath: \littleVisual\src\driver\buzz.cpp
 * @Description: 
 */
#include "buzz.h"
#include "Arduino.h"
#include "config.h"

extern ConfigStruct cfgstruct;

static hw_timer_t *timer_buzz;
static bool IsBusy = false;

static bool IsEnable = true;

void Buzz_init(void)
{
    pinMode(BUZZ_PIN, OUTPUT);
    ledcAttachPin(BUZZ_PIN, BUZZ_CHANNEL);
    ledcSetup(BUZZ_CHANNEL, 0, 8);
    ledcWriteTone(BUZZ_CHANNEL, 0);

    Buzz_Enable(cfgstruct.Beep);
}

static void Buzz_timer_cb(void)
{
    ledcWriteTone(CONFIG_BUZZ_CHANNEL, 0);
    timerAlarmDisable(timer_buzz);
    timerEnd(timer_buzz);
    IsBusy = false;
}
/**
 * @description: 
 * @param {uint32_t} freq
 * @param {uint32_t} duration   [ms]
 * @param {uint8_t} amplitude
 * @return {*}
 */
void Buzz_Tone(uint32_t freq, uint32_t duration, uint8_t amplitude)
{
    if(IsEnable && !IsBusy)
    {
        IsBusy = true;
        constrain(amplitude, 0, 200);
        ledcWrite(BUZZ_CHANNEL, amplitude);
        ledcWriteTone(BUZZ_CHANNEL, freq);

        timer_buzz = timerBegin(2, 80, true);
        timerAttachInterrupt(timer_buzz, Buzz_timer_cb, true);
        timerAlarmWrite(timer_buzz, duration*1000, true);
        timerAlarmEnable(timer_buzz);
    }
    else{
        return;
    }
}

void Buzz_Enable(bool en)
{
    IsEnable = en;
}