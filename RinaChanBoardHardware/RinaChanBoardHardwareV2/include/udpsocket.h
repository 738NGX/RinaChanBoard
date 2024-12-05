#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <AsyncUDP.h>
//-------------------------------------//
//               Wifi 信息
//-------------------------------------//
// #define ssid        "New_Media"
// #define password    "sufexmt123*"

// #define ssid        "738NGX's Magic5"
// #define password    "20040210"

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
// void UDPSocket(CRGB leds[]);

class LedUDPHandler
{

private:
    AsyncUDP Udp;
    CRGB *leds;

public:
    LedUDPHandler(CRGB *leds)
        : leds(leds)
    {
    }
    void begin();
    void handlePacket(AsyncUDPPacket packet);
    void sendCallBack(AsyncUDPPacket &packet, const char *buffer);
    void sendCallBack(AsyncUDPPacket &packet, const String str);
    // ~LedUDPHandler();
};