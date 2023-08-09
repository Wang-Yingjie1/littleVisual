/*
 * @Author: WYJ
 * @Date: 2022-03-24 10:17:01
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-28 00:19:51
 * @FilePath: \littleVisual\src\driver\power.cpp
 * @Description: 
 */


#include <Arduino.h>
#include "power.h"
#include "config.h"
#include "blacklight.h"

// /*上一次操作时间(ms)*/
// static uint32_t Power_LastHandleTime = 0;

// /*自动关机时间(秒)*/
// static uint16_t Power_AutoLowPowerTimeout = 60;

// /*自动关机功能使能*/
// static bool Power_AutoLowPowerEnable = false;

static bool Power_IsShutdown = false;

// static volatile uint16_t Power_ADCValue = 0;
// static uint16_t Power_ADCValue_last[10] = {4095};

// static void Power_ADC_Init(void)
// {
//     pinMode(CONFIG_BAT_DET_PIN, INPUT);
//     pinMode(CONFIG_BAT_CHG_DET_PIN, INPUT_PULLUP);
// }

void Power_init(void)
{
    pinMode(CONFIG_BAT_CHG_DET_PIN, INPUT);

    Serial.println("Power: Waiting...");
    pinMode(CONFIG_POWER_EN_PIN, OUTPUT);

    digitalWrite(CONFIG_POWER_EN_PIN, HIGH);
    Serial.println("Power: ON");

    /*电池检测*/
    //Power_ADC_Init();
    //Power_SetAutoLowPowerTimeout(60);
    //Power_HandleTimeUpdate();
    //Power_SetAutoLowPowerEnable(false);
}

// void Power_SetAutoLowPowerTimeout(uint16_t sec)
// {
//     Power_AutoLowPowerTimeout = sec;
// }

// void Power_HandleTimeUpdate(void)
// {
//     Power_LastHandleTime = millis();
// }

// void Power_SetAutoLowPowerEnable(bool en)
// {
//     Power_AutoLowPowerEnable = en;
//     Power_HandleTimeUpdate();
// }

void Power_Shutdown(void)
{
    Backlight_SetValue(0);
    digitalWrite(CONFIG_POWER_EN_PIN, LOW);
    Power_IsShutdown = true;
}

// void Power_ADC_TrigUpdate(void)
// {
//     Power_ADCValue = analogRead(CONFIG_BAT_DET_PIN);
// }

// uint16_t Power_GetAutoLowPowerTimeout(void)
// {
//     return Power_AutoLowPowerTimeout;
// }

/**
  * @brief  自动关机监控
  * @param  无
  * @retval 无
  */
// void Power_Update(void)
// {
//     static unsigned long lasttime=0;
//     if( millis() - lasttime >= 1000 )
//     {
//         Power_ADC_TrigUpdate();
//     }

//     if (!Power_AutoLowPowerEnable)
//         return;

//     if (Power_AutoLowPowerTimeout == 0)
//         return;

//     if (millis() - Power_LastHandleTime >= (Power_AutoLowPowerTimeout * 1000))
//     {
//         Power_Shutdown();
//     }
// }

// void Power_GetInfo(Power_Info_t *info)
// {
//     uint32_t sum = Power_ADCValue;
//     for (int i = 9; i > 0; i--)
//     {
//         Power_ADCValue_last[i] = Power_ADCValue_last[i - 1];
//         sum += Power_ADCValue_last[i - 1];
//     }
//     Power_ADCValue_last[0] = Power_ADCValue;

//     // ADC采样电压范围 0-3.3V
//     int voltage = map(
//         sum / 10,
//         0, 4095,
//         0, 3300
//     );

//     voltage *= 2;   //电路中分压了

//     constrain(voltage, BATT_MIN_VOLTAGE, BATT_MAX_VOLTAGE);

//     int usage = map(
//         voltage,
//         BATT_MIN_VOLTAGE, BATT_MAX_VOLTAGE,
//         0, 100
//     );

//     info->usage = usage;
//     info->isCharging = usage != 100 && !digitalRead(CONFIG_BAT_CHG_DET_PIN);
//     info->voltage = voltage;
// }

