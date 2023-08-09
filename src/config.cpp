/*
 * @Author: WYJ
 * @Date: 2022-03-30 10:12:26
 * @LastEditors: WYJ
 * @LastEditTime: 2022-04-01 14:35:25
 * @FilePath: \littleVisual\src\config.cpp
 * @Description: 
 */

#include "config.h"
#include <Preferences.h>

Preferences prefs;
ConfigStruct cfgstruct;

void Config_init(void)
{
    Serial.println("Read Config");
    
    prefs.begin("Config");
        cfgstruct.DispRotation = prefs.getInt("DispRotation", 0);
        cfgstruct.Wifi_name = prefs.getString("Name", "wyj");
        cfgstruct.Wifi_pwd = prefs.getString("Pwd", "12345678");
        cfgstruct.Backlight = prefs.getInt("BL", 500);
        cfgstruct.Address = prefs.getString("Addr", "Shanghai");
        cfgstruct.Beep = prefs.getBool("Beep", true);
        cfgstruct.appid = prefs.getString("Appid", "19219431");
        cfgstruct.appsecret = prefs.getString("Appsecret", "7ZCU60wm");
        cfgstruct.Autoflush = prefs.getBool("Autoflush", true);
    prefs.end();

    Serial.println("Read cfg end");
}

void Config_save(void)
{
    Serial.println("Save Config");

    prefs.begin("Config");
        prefs.putString("Name", cfgstruct.Wifi_name);
        prefs.putString("Pwd", cfgstruct.Wifi_pwd);
        prefs.putInt("BL", cfgstruct.Backlight);
        prefs.putString("Addr", cfgstruct.Address);
        prefs.putBool("Beep", cfgstruct.Beep);
        prefs.putString("Appid", cfgstruct.appid);
        prefs.putString("Appsecret", cfgstruct.appsecret);
        prefs.putInt("DispRotation", cfgstruct.DispRotation);
        prefs.putBool("Autoflush", cfgstruct.Autoflush);
    prefs.end();

    Serial.println("Save cfg end");
}