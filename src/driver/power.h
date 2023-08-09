/*
 * @Author: WYJ
 * @Date: 2022-03-24 10:17:01
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-31 14:34:06
 * @FilePath: \littleVisual - 完成键盘呼出\src\driver\power.h
 * @Description: 
 */


#ifndef POWER_H
#define POWER_H

#include <Arduino.h>

#define BATT_MAX_VOLTAGE    3900
#define BATT_MIN_VOLTAGE    3300

typedef struct
{
    uint16_t voltage;
    uint8_t usage;
    bool isCharging;
} Power_Info_t;

void Power_init(void);
// void Power_SetAutoLowPowerTimeout(uint16_t sec);
// void Power_HandleTimeUpdate(void);
// void Power_SetAutoLowPowerEnable(bool en);
// void Power_ADC_TrigUpdate(void);
// uint16_t Power_GetAutoLowPowerTimeout(void);
// void Power_Update(void);
// void Power_GetInfo(Power_Info_t *info);
void Power_Shutdown(void);

#endif