/*
 * @Author: WYJ
 * @Date: 2022-03-28 15:37:30
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-31 14:40:30
 * @FilePath: \littleVisual\src\driver\buzz.h
 * @Description: 
 */

#ifndef BUZZ_H
#define BUZZ_H

#include <Arduino.h>

#define BUZZ_PIN CONFIG_BUZZ_PIN
#define BUZZ_CHANNEL CONFIG_BUZZ_CHANNEL

void Buzz_init(void);
void Buzz_Tone(uint32_t freq, uint32_t duration, uint8_t amplitude);
void Buzz_Enable(bool en);

#endif