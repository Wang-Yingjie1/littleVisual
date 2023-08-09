/*
 * @Author: WYJ
 * @Date: 2022-03-27 18:45:36
 * @LastEditors: WYJ
 * @LastEditTime: 2022-03-28 14:51:05
 * @FilePath: \littleVisual\src\driver\network.h
 * @Description: 
 */
#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h> 
#include <WiFi.h>      // Built-in
#include <WiFiMulti.h> // 当我们需要使用ESP8266开发板存储多个WiFi网络连接信息时，可以使用ESP8266WiFiMulti库来实现。
#include <WebServer.h> // https://github.com/Pedroalbuquerque/ESP32WebServer download and place in your Libraries folder
#include <ESPmDNS.h>
#include <HTTPClient.h>

int search_wifi(void);
boolean start_conn_wifi(const char *ssid, const char *password);
//boolean end_conn_wifi(void);
boolean close_wifi(void);
//boolean open_ap(const char *ap_ssid = AP_SSID, const char *ap_password = NULL);

#endif