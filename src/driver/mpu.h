/*
 * @Author: WYJ
 * @Date: 2022-04-03 15:37:40
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-03 16:02:37
 * @FilePath: \littleVisual\src\driver\mpu.h
 * @Description: 
 */

#ifndef MPU_H
#define MPU_H

#define IMU_I2C_SDA 32
#define IMU_I2C_SCL 33

enum MPUSTATUS
{
    UNINITIALIZED = 0,
    INITIALIZING,
    INITIALIZED
};

bool MPU_init(void);


#endif