#include "AsyncUDP.h"
#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>

#include <cstdint>
#include <udpsocket.h>

#include <led.h>
#define INCOME_PACKET_MAXLEN 255
// AsyncUDP Udp;                                   // 实例化WiFiUDP
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
    // Serial.printf("正在连接 %s ",ssid);
    // WiFi.begin(ssid,password);                      // 连接到wifi
    // while(WiFi.status()!=WL_CONNECTED)              // 等待连接
    //{
    //     delay(500);
    //     Serial.print(".");
    // }
    Serial.println("连接成功");
    if (Udp.listen(localUDPPort))
    {
        // 启动Udp监听服务
        Serial.println("监听成功");
        Serial.printf("现在收听IP：%s, UDP端口：%d\n",
                      WiFi.localIP().toString().c_str(),
                      localUDPPort);
        Udp.onPacket([this](AsyncUDPPacket packet) {
            this->handlePacket(packet); // 调用独立的成员函数
        });
    }
    else
    {
        Serial.println("监听失败");
        // TODO: 失败的逻辑是？重启？
    }
}

void LedUDPHandler::handlePacket(AsyncUDPPacket packet)
{
    if (packet.length() <= 0) // TODO: 该函数只会在接到包的时候被调用，可能可以去掉
        return;
    // 收到Udp数据包
    Serial.printf("收到来自远程IP：%s（远程端口：%d）的数据包字节数：%d\n",
                  packet.remoteIP().toString().c_str(), packet.remotePort(),
                  packet.length());

    // 读取Udp数据包并存放在incomingPacket
    size_t len = packet.length();
    if (len > INCOME_PACKET_MAXLEN) len = INCOME_PACKET_MAXLEN; // 防止溢出
    memcpy(incomingPacket, packet.data(), len);
    incomingPacket[len] = 0; // 确保字符串以null结尾

    // if (len <= 0)
    //     return;

    Serial.printf("UDP数据包内容为: %s\n", incomingPacket);

    switch (len)
    {
        case 72:
            // 从上位机接受表情状态字符串
            face_update_by_string(incomingPacket, this->leds, CRGB(R, G, B));
            FastLED.show();
            break;
        case 4:
            // 从上位机接受亮度更新
            bright = (incomingPacket[1] - '0') * 100 +
                     (incomingPacket[2] - '0') * 10 + (incomingPacket[3] - '0');
            FastLED.setBrightness(bright);
            FastLED.show();
            break;
        case 7:
            // 从上位机接受颜色更新
            updateColor(incomingPacket, this->leds, R, G, B);
            FastLED.show();

        default:
            sendCallBack(packet, "Command Error!");
            break;
    }

    if (strcmp(incomingPacket, "requestFace") == 0)
    {
        // 发送状态字符串到上位机
        sendCallBack(packet, get_face(this->leds));
    }
    else if (strcmp(incomingPacket, "requestColor") == 0)
    {
        // 发送颜色字符串到上位机
        sendCallBack(packet, encodeColor(R, G, B));
    }
    else if (strcmp(incomingPacket, "requestBright") == 0)
    {
        // 发送亮度字符串到上位机
        sendCallBack(packet, encodeBright(bright));
    }
    else
    {
        sendCallBack(packet, "Command Error!");
    }
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
