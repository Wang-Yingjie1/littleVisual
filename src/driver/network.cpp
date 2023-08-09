/*
 * @Author: WYJ
 * @Date: 2022-03-27 18:45:29
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-28 14:58:32
 * @FilePath: \littleVisual\src\driver\network.cpp
 * @Description: 
 */

#include "network.h"

const char* HOST_NAME = "littleVisual";

int search_wifi(void)
{
    Serial.println("Wifi scan start");
    int wifi_num = WiFi.scanNetworks();
    Serial.println("scan done");
    if (wifi_num == 0)
    {
        Serial.println("no networks found");
    }
    else{
        Serial.print(wifi_num);
        Serial.println(" networks found");
        for (int cnt = 0; cnt < wifi_num; ++cnt)
        {
            Serial.print(cnt + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(cnt));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(cnt));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(cnt) == WIFI_AUTH_OPEN) ? " " : "*");
        }
    }
    return wifi_num;
}

boolean start_conn_wifi(const char *ssid, const char *password)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(F("\nWiFi is OK.\n"));
        return false;
    }
    Serial.println("");
    Serial.print(F("Connecting: "));
    Serial.print(ssid);
    Serial.print(F(" @ "));
    Serial.println(password);

        //设置为STA模式并连接WIFI
    WiFi.enableSTA(true);
    // 修改主机名
    WiFi.setHostname(HOST_NAME);
    WiFi.begin(ssid, password);

    return true;
}

boolean close_wifi(void)
{
    if(!WiFi.disconnect())
    {
        return false;
    }
    WiFi.enableSTA(false);
    WiFi.enableAP(false);
    WiFi.mode(WIFI_MODE_NULL);
    
    Serial.println(F("WiFi disconnect"));
    return true;
}