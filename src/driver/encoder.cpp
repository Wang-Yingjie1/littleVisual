/*
 * @Author: WYJ
 * @Date: 2022-03-24 13:42:50
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-31 23:34:39
 * @FilePath: \littleVisual\src\driver\encoder.cpp
 * @Description:
 */
#include "Arduino.h"
#include "encoder.h"
#include "config.h"
#include "buzz.h"

static bool DRAM_ATTR EncoderEnable = true;
static bool DRAM_ATTR EncoderDiffDisable = false;
static int16_t DRAM_ATTR EncoderDiff = 0;

static void IRAM_ATTR Encoder_IrqHandler_Rotate(void)
{
    //Serial.println("Interrupt");
    if (!EncoderEnable || EncoderDiffDisable)
    {
        return;
    }

    static volatile int DRAM_ATTR count, countLast;
    static volatile DRAM_ATTR uint8_t a0, b0;
    static volatile DRAM_ATTR uint8_t ab0;
    uint8_t a = digitalRead(ENCODER_A_PIN);
    uint8_t b = digitalRead(ENCODER_B_PIN);
    if (a != a0)
    {
        a0 = a;
        if (b != b0)
        {
            b0 = b;
            count += ((a == b) ? 1 : -1);
            if ((a == b) != ab0)
            {
                count += ((a == b) ? 1 : -1);
            }
            ab0 = (a == b);
        }
    }

    if (count != countLast)
    {
        EncoderDiff += (count - countLast) > 0 ? 1 : -1;
        countLast = count;
    }
}

void Encoder_Init(void)
{
    pinMode(ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(ENCODER_B_PIN, INPUT_PULLUP);
    pinMode(ENCODER_PUSH_PIN, INPUT_PULLUP);

    attachInterrupt(ENCODER_A_PIN, Encoder_IrqHandler_Rotate, CHANGE);
}

int16_t Encoder_GetDiff(void)
{
    //Serial.print("encoder.diff: ");
    int16_t diff = -EncoderDiff / 2;
    if (diff != 0)
    {
        // EncoderDiff是实际的脉冲数；把本次变量用掉了，需要重新置0
        EncoderDiff = 0;
        Buzz_Tone(700, 80, 100);
    }
    
    //Serial.println(diff,10);
    return diff; 
}

encoder_state_t Encoder_GetState(void)
{
    //Serial.print("encoder.state: ");
    if (!EncoderEnable)
    {
        //Serial.println("Disabled");
        return ENCODER_STATE_RELEASED;
    }
    
    if( digitalRead(ENCODER_PUSH_PIN) == LOW)
    {   
        //Serial.println("Pressed");
        Buzz_Tone(400, 80, 100);
        return ENCODER_STATE_PRESSED;
    }

    //Serial.println("Released");
    return ENCODER_STATE_RELEASED;
}

void Encoder_SetEnable(bool en)
{
    EncoderEnable = en;
}

void Encoder_Interrupt_en(bool en)
{
    if(en)
    {
        attachInterrupt(ENCODER_A_PIN, Encoder_IrqHandler_Rotate, CHANGE);
    }
    else{
        detachInterrupt(ENCODER_A_PIN);
    }
}