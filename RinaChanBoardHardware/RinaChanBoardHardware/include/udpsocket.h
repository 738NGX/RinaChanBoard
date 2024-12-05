#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//-------------------------------------//
//               Wifi 信息
//-------------------------------------//
//#define ssid        "New_Media"
//#define password    "sufexmt123*"

//#define ssid        "738NGX's Magic5"
//#define password    "20040210"

//-------------------------------------//
//               端口信息
//-------------------------------------//
#define LOCAL_UDP_PORT  1234
#define REMOTE_UDP_PORT 4321

//-------------------------------------//
//               功能函数
//-------------------------------------//

/**
 * @brief 向UDP客户端发送消息
 * 
 * @param buffer 缓冲消息
 */
void sendCallBack(const char *buffer);
/**
 * @brief 向UDP客户端发送消息
 * 
 * @param buffer 缓冲消息
 */
void sendCallBack(const String buffer);

/**
 * @brief 初始化wifi
 * 
 */
void init_wifi();

/**
 * @brief UDP主函数
 * 
 */
void UDPSocket(CRGB leds[]);
