#include "AsyncUDP.h"
#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>

#include <udpsocket.h>
#include <led.h>
#define ENABLE_SERIAL_DEBUG

#ifdef ENABLE_SERIAL_DEBUG
extern HardwareSerial debugSerial;
#endif // ENABLE_SERIAL_DEBUG

void LedUDPHandler::begin()
{

    if (!Udp.listen(localUDPPort))
    {
#ifdef ENABLE_SERIAL_DEBUG
        debugSerial.println("监听失败"); // TODO: 失败的逻辑是？重启？

#endif // ENABLE_SERIAL_DEBUG
    }
#ifdef ENABLE_SERIAL_DEBUG
    debugSerial.println("监听成功");
    debugSerial.printf("现在收听IP：%s, UDP端口：%d\n",
                       WiFi.localIP().toString().c_str(),
                       localUDPPort);
#endif // ENABLE_SERIAL_DEBUG
    Udp.onPacket([this](AsyncUDPPacket packet) {
        this->handlePacket(packet); // 调用独立的成员函数需要转一下
    });
}

void LedUDPHandler::handlePacket(AsyncUDPPacket packet)
{
    size_t len = packet.length();
    if (len > UDP_INCOME_PACKET_MAXLEN) len = UDP_INCOME_PACKET_MAXLEN; // 防止溢出
    memcpy(incomingPacket, packet.data(), len);                         // 读取Udp数据包并存放在incomingPacket
#ifdef ENABLE_SERIAL_DEBUG
    debugSerial.printf(">>[get]远程IP:%s:%d length:%d\n",
                       packet.remoteIP().toString().c_str(),
                       packet.remotePort(),
                       packet.length());
    for (uint8_t i = 0; i < len; i++)
        debugSerial.printf("%02X ", incomingPacket[i]);

    debugSerial.printf("\n");
#endif // ENABLE_SERIAL_DEBUG
    switch (len)
    {
        case static_cast<uint8_t>(PackTypeLen::FACE_FULL): {
            // 从上位机接受全脸状态更新
            decodeFaceHex(incomingPacket,
                          faceBuf,
                          static_cast<uint8_t>(PackTypeLen::FACE_FULL));

            faceUpdate_FullPack(faceBuf,
                                this->leds,
                                CRGB(R, G, B));
            FastLED.show();
            break;
        }
        case static_cast<uint8_t>(PackTypeLen::FACE_LITE): {
            // 从上位机接受表情部件状态更新
            break;
        }
        case static_cast<uint8_t>(PackTypeLen::COLOR): {
            // 从上位机接受颜色更新
            R = (uint8_t)incomingPacket[0];
            G = (uint8_t)incomingPacket[1];
            B = (uint8_t)incomingPacket[2];
            updateColor(this->leds, R, G, B);
            FastLED.show();
            break;
        }
        case static_cast<uint8_t>(PackTypeLen::REQUEST): {
            // 从上位机接受请求包
            handleRequest(packet, incomingPacket);
            break;
        }
        break;
        case static_cast<uint8_t>(PackTypeLen::BRIGHT): {
            // 从上位机接受亮度更新
            bright = incomingPacket[0];
            FastLED.setBrightness(bright);
            FastLED.show();
            break;
        }
        default:
            sendCallBack(packet, "Command Error!");
            break;
    }
}

void LedUDPHandler::handleRequest(AsyncUDPPacket packet, char incomingPacket[])
{
    uint16_t requestPacket = (incomingPacket[0] << 8) | incomingPacket[1];
    switch (requestPacket)
    {
        case static_cast<uint16_t>(LedUDPHandler::RequestType::FACE): {
            // 发送状态hex字节流到上位机
            getFaceHex(this->leds, faceHexBuffer);
            sendCallBack(packet,
                         faceHexBuffer,
                         static_cast<uint8_t>(PackTypeLen::FACE_FULL));
            break;
        }
        case static_cast<uint16_t>(LedUDPHandler::RequestType::COLOR): {
            // 发送颜色hex字节流到上位机
            sendCallBack(packet,
                         R << 16 | G << 8 | B,
                         static_cast<uint8_t>(PackTypeLen::COLOR));
            break;
        }
        case static_cast<uint16_t>(LedUDPHandler::RequestType::BRIGHT): {
            // 发送亮度字节流到上位机
            sendCallBack(packet,
                         bright,
                         static_cast<uint8_t>(PackTypeLen::BRIGHT));
            break;
        }
        default: {
            sendCallBack(packet, "Command Error!");
            break;
        }
    }
}

void LedUDPHandler::sendCallBack(AsyncUDPPacket &packet, const int value, uint8_t length)
{
    Udp.writeTo((uint8_t *)&value, length, packet.remoteIP(), remoteUDPPort);
#ifdef ENABLE_SERIAL_DEBUG
    debugSerial.printf("<<[SEND]远程IP: %s:%d\n %X \n ",
                       packet.remoteIP().toString().c_str(),
                       remoteUDPPort,
                       value);
#endif // ENABLE_SERIAL_DEBUG
}

void LedUDPHandler::sendCallBack(AsyncUDPPacket &packet, const uint8_t *hexBuffer, uint8_t length)
{
    Udp.writeTo((uint8_t *)&hexBuffer, length, packet.remoteIP(), remoteUDPPort);
#ifdef ENABLE_SERIAL_DEBUG
    debugSerial.printf("<<[SEND]远程IP: %s:%d\n<<",
                       packet.remoteIP().toString().c_str(),
                       remoteUDPPort);
    for (uint8_t i = 0; i < length; i++)
        debugSerial.printf("%02X ", (uint8_t)hexBuffer[i]);
#endif // ENABLE_SERIAL_DEBUG
}

void LedUDPHandler::sendCallBack(AsyncUDPPacket &packet, const char *buffer)
{
    AsyncUDPMessage msg;
    msg.printf("%s", buffer); // 格式化字符串
    Udp.sendTo(msg, packet.remoteIP(), remoteUDPPort);
}

void LedUDPHandler::sendCallBack(AsyncUDPPacket &packet, const String str)
{
    sendCallBack(packet, str.c_str());
}
