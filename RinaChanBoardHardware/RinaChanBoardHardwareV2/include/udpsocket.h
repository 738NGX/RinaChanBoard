#pragma once

#include "sys/_intsup.h"
#include <cstdint>
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
    uint8_t faceHexBuffer[36];
    int faceBuf[16][18];

public:
    enum class RequestType : uint16_t {
        FACE   = 0x1001,
        COLOR  = 0x1002,
        BRIGHT = 0x1003
    };
    LedUDPHandler(CRGB *leds)
        : leds(leds)
    {
    }
    void begin();
    void handlePacket(AsyncUDPPacket packet);
    void sendCallBack(AsyncUDPPacket &packet, const char *buffer);
    void sendCallBack(AsyncUDPPacket &packet, const String str);
    void sendCallBack(AsyncUDPPacket &packet, const int value, uint8_t length);
    void sendCallBack(AsyncUDPPacket &packet, const char *hexBuffer, uint8_t length);
};