#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>

#include <cstdint>
#include <udpsocket.h>
#include <AsyncUDP.h>
#include <led.h>

WiFiUDP Udp;                                  // 实例化WiFiUDP
unsigned int localUDPPort  = LOCAL_UDP_PORT;  // 本地监听端口
unsigned int remoteUDPPort = REMOTE_UDP_PORT; // 远程监听端口
char incomingPacket[255];                     // 保存Udp工具发过来的消息

unsigned int R = 249;
unsigned int G = 113;
unsigned int B = 212;
uint8_t bright = 16;

void sendCallBack(const char *buffer)
{
    Udp.beginPacket(Udp.remoteIP(), remoteUDPPort); // 配置远端ip地址和端口
    Udp.print(buffer);                              // 把数据写入发送缓冲区
    Udp.endPacket();                                // 发送数据
}

void sendCallBack(const String str)
{
    Udp.beginPacket(Udp.remoteIP(), remoteUDPPort); // 配置远端ip地址和端口
    Udp.print(str.c_str());                         // 把数据写入发送缓冲区
    Udp.endPacket();                                // 发送数据
}

void init_wifi()
{
    // Serial.printf("正在连接 %s ",ssid);
    // WiFi.begin(ssid,password);                      // 连接到wifi
    // while(WiFi.status()!=WL_CONNECTED)              // 等待连接
    //{
    //     delay(500);
    //     Serial.print(".");
    // }
    Serial.println("连接成功");
    if (Udp.begin(localUDPPort))
    {
        // 启动Udp监听服务
        Serial.println("监听成功");
        // wifi模块的ip地址在这里搞
        Serial.printf("现在收听IP：%s, UDP端口：%d\n", WiFi.localIP().toString().c_str(), localUDPPort);
    }
    else
    {
        Serial.println("监听失败");
    }
}

void UDPSocket(CRGB leds[])
{
    // 解析Udp数据包
    int packetSize = Udp.parsePacket(); // 获得解析包
    if (!packetSize)
        return;
    // 收到Udp数据包
    Serial.printf("收到来自远程IP：%s（远程端口：%d）的数据包字节数：%d\n",
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  packetSize);

    // 读取Udp数据包并存放在incomingPacket
    int len = Udp.read(incomingPacket, 255);
    if (len <= 0)
        return;

    incomingPacket[len] = 0; // 清空缓存
                             // TODO:规避动态分配内存
    Serial.printf("UDP数据包内容为: %s\n", incomingPacket);

    switch (len)
    {
        case 72:
            // 从上位机接受表情状态字符串
            face_update_by_string(incomingPacket, leds, CRGB(R, G, B));
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
            updateColor(incomingPacket, leds, R, G, B);
            FastLED.show();

        default:
            sendCallBack("Command Error!");
            break;
    }

    if      (strcmp(incomingPacket, "requestFace") == 0)
    {
        // 发送状态字符串到上位机
        sendCallBack(get_face(leds));
    }
    else if (strcmp(incomingPacket, "requestColor") == 0)
    {
        // 发送颜色字符串到上位机
        sendCallBack(encodeColor(R, G, B));
    }
    else if (strcmp(incomingPacket, "requestBright") == 0)
    {
        // 发送亮度字符串到上位机
        sendCallBack(encodeBright(bright));
    }
    else
    {
        sendCallBack("Command Error!");
    }
}
