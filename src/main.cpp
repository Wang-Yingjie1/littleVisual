/*
 * @Author: WYJ
 * @Date: 2022-03-24 09:57:21
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-07 17:26:29
 * @FilePath: \littleVisual\src\main.cpp
 * @Description: 本项目硬件使用稚晖君开源项目Peak(https://github.com/peng-zhihui/Peak)的方案
 *               软件由自己编写，UI采用lvgl
 *                
 */

#include <Arduino.h>

#include "config.h"

#include "TFT_eSPI.h"
#include "SD.h"
// #include "MPU6050.h"

#include "lvgl.h"
#include "lv_port/lv_port_disp.h"
#include "lv_port/lv_port_indev.h"
#include "lv_port/lv_port_fs.h"

#include "driver/lv_timer.h"
#include "driver/power.h"
#include "driver/blacklight.h"
#include "driver/display.h"
#include "driver/encoder.h"
#include "driver/sd_card.h"
#include "driver/buzz.h"
// #include "driver/mpu.h"

#include "app/apps.h"

TFT_eSPI screen;
SdCard tf;
// extern MPU6050 mpu;

// int16_t ax,ay,az,gx,gy,gz;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(VERSION_FIRMWARE_NAME);
  Serial.println("Version: " VERSION_SOFTWARE);
  Serial.println("Author: " VERSION_AUTHOR_NAME);

  Power_init();
  Config_init();
  lv_Timer_init(); // 作为lvgl的心脏
  Backlight_Init();
  Display_init();
  Encoder_Init();
  Buzz_init();
  tf.init();

  // lvgl init
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
  lv_port_fs_init();
 
  APP_selector();
}

void loop() {
  // put your main code here, to run repeatedly:
  lv_task_handler();
  delay(5);
}

  // disableCore1WDT();
  // enableLoopWDT();
        // char mpu_info[60] = {0};
        // Serial.println("MPU data:");
        // mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

        // sprintf(mpu_info, "ax: %d, ay: %d, az: %d, gx: %d, gy: %d, gz: %d", ax, ay, az, gx, gy, gz);
        // Serial.println(mpu_info);
  // feedLoopWDT();