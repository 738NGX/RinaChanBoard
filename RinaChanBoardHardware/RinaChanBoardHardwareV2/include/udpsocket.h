#pragma once

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

#define UDP_INCOME_PACKET_MAXLEN 255
//-------------------------------------//
//               端口信息
//-------------------------------------//
#define LOCAL_UDP_PORT  1234
#define REMOTE_UDP_PORT 4321

//-------------------------------------//
//               功能函数
//-------------------------------------//

/**
 * @brief 初始化wifi
 *
 */
void init_wifi();

/**
 * @brief UDP主函数
 *
 */

class LedUDPHandler
{
private:
    AsyncUDP Udp;
    CRGB *leds;
    uint8_t faceHexBuffer[36];
    uint8_t faceBuf[16][18];
    const uint16_t localUDPPort  = LOCAL_UDP_PORT;
    const uint16_t remoteUDPPort = REMOTE_UDP_PORT;
    char incomingPacket[UDP_INCOME_PACKET_MAXLEN];
    uint8_t R      = 249;
    uint8_t G      = 113;
    uint8_t B      = 212;
    uint8_t bright = 16;

    /**
     * @brief 内部函数，用于处理请求包
     *
     * @param packet
     * @param incomingPacket
     */
    void handleRequest(AsyncUDPPacket packet, char incomingPacket[]);

public:
    enum class PackTypeLen : uint8_t {
        FACE_FULL      = 36,
        FACE_TEXT_LITE = 16,
        FACE_LITE      = 4,
        COLOR          = 3,
        REQUEST        = 2,
        BRIGHT         = 1,
    };
    enum class RequestType : uint16_t {
        FACE    = 0x1001,
        COLOR   = 0x1002,
        BRIGHT  = 0x1003,
        VERSION = 0x1004,
        BATTRY  = 0x1005,
    };
    LedUDPHandler(CRGB *leds)
        : leds(leds)
    {
    }
    void begin();
    void handlePacket(AsyncUDPPacket packet);

    /**
     * @brief 向UDP客户端发送消息
     *
     * @param buffer
     */
    void sendCallBack(AsyncUDPPacket &packet, const char *buffer);
    void sendCallBack(AsyncUDPPacket &packet, const String str);
    void sendCallBack(AsyncUDPPacket &packet, const int value, uint8_t length);
    void sendCallBack(AsyncUDPPacket &packet, const uint8_t *hexBuffer, uint8_t length);
};