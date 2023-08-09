/*
 * @Author: WYJ
 * @Date: 2022-04-03 15:37:26
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 16:32:17
 * @FilePath: \littleVisual\src\driver\mpu.cpp
 * @Description: 
 */

#include "mpu.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;

bool MPU_init(void)
{
    Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);
    Wire.setClock(400000);
    unsigned long timeout = 5000;
    unsigned long preMillis = millis();

    mpu = MPU6050(0x68);

    while(!mpu.testConnection())
    {
        if(millis() - preMillis >= timeout)
        {
            break;
        }
    }

    if (!mpu.testConnection())
    {
        Serial.print(F("Unable to connect to MPU6050.\n"));
        return false;
    }

    Serial.print(F("Initialization MPU6050 now, Please don't move.\n"));
    mpu.initialize();

    {
        // 启动自动校准
        // 7次循环自动校正
        mpu.CalibrateAccel(7);
        mpu.CalibrateGyro(7);
        mpu.PrintActiveOffsets();
    }

    Serial.print(F("Initialization MPU6050 success.\n"));
    return true;
}