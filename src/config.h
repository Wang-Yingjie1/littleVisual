/*
 * @Author: WYJ
 * @Date: 2022-03-24 10:13:25
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-01 14:34:55
 * @FilePath: \littleVisual\src\config.h
 * @Description: 
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
// #ifdef __cplusplus
// extern "C" {
// #endif

// // #include <Arduino.h>

// #ifdef __cplusplus
// } /* extern "C" */
// #endif

#define VERSION_FIRMWARE_NAME       "LVGL_MINI_DISPLAY"
#define VERSION_SOFTWARE            "v1.0"
#define VERSION_AUTHOR_NAME         "WYJ"

/*=========================
   Hardware Configuration
 *=========================*/

#define NULL_PIN                    PD0

/* Screen */
#define CONFIG_SCREEN_CS_PIN        PB0
#define CONFIG_SCREEN_DC_PIN        PA4
#define CONFIG_SCREEN_RST_PIN       PA6
#define CONFIG_SCREEN_SCK_PIN       PA5
#define CONFIG_SCREEN_MOSI_PIN      PA7
#define CONFIG_SCREEN_BLK_PIN       12

#define CONFIG_SCREEN_HOR_RES       240
#define CONFIG_SCREEN_VER_RES       240
#define CONFIG_SCREEN_BUFFER_SIZE   (CONFIG_SCREEN_HOR_RES * CONFIG_SCREEN_VER_RES /2)

/* Battery */
#define CONFIG_BAT_DET_PIN          37
#define CONFIG_BAT_CHG_DET_PIN      38

/* Buzzer */
#define CONFIG_BUZZ_PIN             25
#define CONFIG_BUZZ_CHANNEL         2
#define CONFIG_SOUND_ENABLE_DEFAULT false

/* IMU */
#define CONFIG_IMU_INT1_PIN         32
#define CONFIG_IMU_INT2_PIN         33

/* I2C */
#define CONFIG_MCU_SDA_PIN          32
#define CONFIG_MCU_SCL_PIN          33

/* Encoder */
#define CONFIG_ENCODER_B_PIN        34
#define CONFIG_ENCODER_A_PIN        35
#define CONFIG_ENCODER_PUSH_PIN     27

/* Power */
#define CONFIG_POWER_EN_PIN         21

/* Debug USART */
#define CONFIG_DEBUG_SERIAL         Serial

/* SD CARD */
#define CONFIG_SD_SPI               SPI_SD
#define CONFIG_SD_CD_PIN            -1
#define CONFIG_SD_MOSI_PIN          PB15
#define CONFIG_SD_MISO_PIN          PB14
#define CONFIG_SD_SCK_PIN           PB13
#define CONFIG_SD_CS_PIN            15
#define CONFIG_SD_DET_PIN           22


#define MEM_MONITOR     0

struct ConfigStruct
{     
    int     DispRotation;   // "DispRotation"      
    int     Backlight;      // "BL"
    bool    Beep;           // "Beep"
    bool    Autoflush;      // "Autoflush"  ; 相册自动刷新
    String  Wifi_name;      // "Name"
    String  Wifi_pwd;       // "Pwd"
    String  Address;        // "Addr"
    String  appid;          // "Appid"
    String  appsecret;      // "Appsecret"
};


void Config_init(void);
void Config_save(void);


#endif