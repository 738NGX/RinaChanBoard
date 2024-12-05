#include "AsyncUDP.h"
#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>

#include <udpsocket.h>

#include <led.h>
#define INCOME_PACKET_MAXLEN 255

extern HardwareSerial debugSerial;
const uint16_t localUDPPort  = LOCAL_UDP_PORT;  // 本地监听端口
const uint16_t remoteUDPPort = REMOTE_UDP_PORT; // 远程监听端口
                                                //
char incomingPacket[INCOME_PACKET_MAXLEN];      // 保存Udp工具发过来的消息

unsigned int R = 249;
unsigned int G = 113;
unsigned int B = 212;
uint8_t bright = 16;

void LedUDPHandler::begin()
{
    debugSerial.println("连接成功");
    if (Udp.listen(localUDPPort))
    {
        // 启动Udp监听服务
        debugSerial.println("监听成功");
        debugSerial.printf("现在收听IP：%s, UDP端口：%d\n",
                           WiFi.localIP().toString().c_str(),
                           localUDPPort);
        Udp.onPacket([this](AsyncUDPPacket packet) {
            this->handlePacket(packet); // 调用独立的成员函数需要转一下
        });
    }
    else
    {
        debugSerial.println("监听失败");
        // TODO: 失败的逻辑是？重启？
    }
}

void LedUDPHandler::handlePacket(AsyncUDPPacket packet)
{
    if (packet.length() <= 0) // TODO: 该函数只会在接到包的时候被调用，可能可以去掉
        return;
    // 收到Udp数据包
    debugSerial.printf(">>[get]远程IP:%s:%d length:%d\n",
                       packet.remoteIP().toString().c_str(), packet.remotePort(),
                       packet.length());

    size_t len = packet.length();                               // 读取Udp数据包并存放在incomingPacket
    if (len > INCOME_PACKET_MAXLEN) len = INCOME_PACKET_MAXLEN; // 防止溢出
    memcpy(incomingPacket, packet.data(), len);

    for (uint8_t i = 0; i < len; i++)
    {
        debugSerial.printf("%02X ", incomingPacket[i]); // 打印每个字节的十六进制
    }
    debugSerial.printf("\n");

    switch (len)
    {
        case 36: { // 从上位机接受表情状态字符串
                   // face_update_by_string(incomingPacket, this->leds, CRGB(R, G, B));
            face_update(int(*face)[18], this->leds, CRGB(R, G, B))
                FastLED.show();
            break;
        }
        case 1: { // 从上位机接受亮度更新
            bright = incomingPacket[0];
            FastLED.setBrightness(bright);
            FastLED.show();
            break;
        }
        case 7: { // 从上位机接受颜色更新
            updateColor(incomingPacket, this->leds, R, G, B);
            FastLED.show();
            break;
        }

        case 2: {
            uint16_t requestPacket = (incomingPacket[0] << 8) | incomingPacket[1];
            switch (requestPacket)
            {
                case static_cast<uint16_t>(RequestType::FACE):
                    // 发送状态字符串到上位机
                    get_face_hex(this->leds, faceHexBuffer);
                    sendCallBack(packet,
                                 get_face(this->leds));
                    break;
                case static_cast<uint16_t>(RequestType::COLOR):
                    // 发送颜色字符串到上位机
                    sendCallBack(packet,
                                 R << 16 | G << 8 | B,
                                 3);
                    break;
                case static_cast<uint16_t>(RequestType::BRIGHT):
                    sendCallBack(packet,
                                 bright,
                                 1);
                    break;
                default:
                    sendCallBack(packet, "Command Error!");
                    break;
            }
            break;
        }
        default:
            sendCallBack(packet, "Command Error!");
            break;
    }
}
void LedUDPHandler::sendCallBack(AsyncUDPPacket &packet, const int value, uint8_t length)
{
    Udp.writeTo((uint8_t *)&value, length, packet.remoteIP(), remoteUDPPort);
    debugSerial.printf("<<[SEND]远程IP: %s:%d\n %02X \n ",
                       packet.remoteIP().toString().c_str(),
                       remoteUDPPort,
                       value);
}

void LedUDPHandler::sendCallBack(AsyncUDPPacket &packet, const char *hexBuffer, uint8_t length)
{
    Udp.writeTo((uint8_t *)&hexBuffer, length, packet.remoteIP(), remoteUDPPort);
    // 打印十六进制数据
    debugSerial.printf("<<[SEND]远程IP: %s:%d\n<<",
                       packet.remoteIP().toString().c_str(),
                       remoteUDPPort);
    for (uint8_t i = 0; i < length; i++)
    {
        debugSerial.printf("%02X ", (uint8_t)hexBuffer[i]); // 打印每个字节的十六进制值
    }
}

[[deprecated]] void LedUDPHandler::sendCallBack(AsyncUDPPacket &packet, const char *buffer)
{
    AsyncUDPMessage msg;
    msg.printf("%s", buffer); // 格式化字符串
    Udp.sendTo(msg, packet.remoteIP(), remoteUDPPort);
}

[[deprecated]] void LedUDPHandler::sendCallBack(AsyncUDPPacket &packet, const String str)
{
    sendCallBack(packet, str.c_str());
}
