/*
 * @Author: WYJ
 * @Date: 2022-03-24 13:42:55
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-31 14:29:48
 * @FilePath: \littleVisual - 完成键盘呼出\src\driver\encoder.h
 * @Description: 
 */

#ifndef ENCODER_H
#define ENCODER_H

#define ENCODER_A_PIN       CONFIG_ENCODER_A_PIN
#define ENCODER_B_PIN       CONFIG_ENCODER_B_PIN
#define ENCODER_PUSH_PIN    CONFIG_ENCODER_PUSH_PIN

typedef enum {
    ENCODER_STATE_RELEASED = 0,
    ENCODER_STATE_PRESSED
} encoder_state_t;

void Encoder_Init(void);
encoder_state_t Encoder_GetState(void);
void Encoder_SetEnable(bool en);
int16_t Encoder_GetDiff(void);
void Encoder_Interrupt_en(bool en);

#endif
