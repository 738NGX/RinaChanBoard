#include <Arduino.h>
#include <FastLED.h>

#include <bemfa.h>
#include <face.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define ssid      "New_Media"       // WIFI名字最好别包含特殊符号 不然连接不上
#define password  "sufexmt123*"     // WIFI密码
WiFiUDP Udp;                        // 实例化WiFiUDP
unsigned int localUDPPort=1234;     // 本地监听端口
unsigned int remoteUDPPort=4321;    // 远程监听端口
char incomingPacket[255];           // 保存Udp工具发过来的消息

CRGB leds[NUM_LEDS];

CRGB def_color(249,113,212);

//向udp工具发送消息
void sendCallBack(const char *buffer)
{
    Udp.beginPacket(Udp.remoteIP(), remoteUDPPort);//配置远端ip地址和端口
    Udp.write(buffer); //把数据写入发送缓冲区
    Udp.endPacket(); //发送数据
}

void setup()
{
    Serial.begin(9600);//打开串口，设置波特率
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(DATA_PIN,OUTPUT);
    pinMode(BUTTON,INPUT_PULLUP);
    
    digitalWrite(LED_BUILTIN,HIGH);
    
    LEDS.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS); 
    FastLED.setBrightness(64);
    face_update_by_string("0000000007def810a205ef810a205ef800000000117a26505155884d25117a2000000000",leds,def_color);
    FastLED.show();
    delay(1000);
    face_update_by_string("0000000001e3e044201e0804820113e00000000004420191405488133e04488000000000",leds,def_color);
    FastLED.show();
    delay(1000);
    face_update_by_string("00000000000000601818060601818060000000000000003f000840012000300000000000",leds,def_color);
    FastLED.show();

    Serial.println("Beginning...");

    Serial.println();
    Serial.printf("正在连接 %s ", ssid);
    WiFi.begin(ssid, password);//连接到wifi
    while (WiFi.status() != WL_CONNECTED)//等待连接
    {
        delay(600);
        Serial.print(".");
    }
    Serial.println("连接成功");
    if (Udp.begin(localUDPPort)) 
    { 
        //启动Udp监听服务
        Serial.println("监听成功");
        //wifi模块的ip地址在这里搞
        Serial.printf("现在收听IP：%s, UDP端口：%d\n", WiFi.localIP().toString().c_str(), localUDPPort);
    } 
    else 
    {
        Serial.println("监听失败");
    }
}

void loop()
{
    //解析Udp数据包
    int packetSize = Udp.parsePacket();//获得解析包
    if (packetSize)//解析包不为空
    {
        //收到Udp数据包
        //Udp.remoteIP().toString().c_str()用于将获取的远端IP地址转化为字符串
        Serial.printf("收到来自远程IP：%s（远程端口：%d）的数据包字节数：%d\n", Udp.remoteIP().toString().c_str(), Udp.remotePort(), packetSize);
        
        // 读取Udp数据包并存放在incomingPacket
        int len = Udp.read(incomingPacket, 255);//返回数据包字节数
        if (len > 0)
        { 
            incomingPacket[len] = 0;//清空缓存
            Serial.printf("UDP数据包内容为: %s\n", incomingPacket);//向串口打印信息
        
            //strcmp函数是string compare(字符串比较)的缩写，用于比较两个字符串并根据比较结果返回整数。
            //基本形式为strcmp(str1,str2)，若str1=str2，则返回零；若str1<str2，则返回负数；若str1>str2，则返回正数。
            if(len==72)
            {
                face_update_by_string(incomingPacket,leds,def_color);
                FastLED.show();
                sendCallBack("Success");
            }
            else if(strcmp(incomingPacket,"requestFace")==0)
            {
                sendCallBack(get_face(leds).c_str());
            }
            else // 如果指令错误，调用sendCallBack
            {  
                sendCallBack("Command Error!");  
            }  
        }
    }
}
