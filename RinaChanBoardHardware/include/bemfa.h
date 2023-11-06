#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>

#define server_ip "bemfa.com" // 巴法云服务器地址默认即可
#define server_port "8344"    // 服务器端口，tcp创客云端口8344

//********************需要修改的部分*******************//

// WIFI名称，区分大小写，不要写错
#define DEFAULT_STASSID "New_Media"
// WIFI密码
#define DEFAULT_STAPSW "sufexmt123*"
// 用户私钥，可在控制台获取,修改为自己的UID
#define UID "a8a83e1f0a4c4e42b031e1c323dd9159"
// 主题名字，可在控制台新建
#define TOPIC "RinaChanBoard"

//**************************************************//

// 最大字节数
#define MAX_PACKETSIZE 512
// 设置心跳值60s
#define KEEPALIVEATIME 60 * 1000

// 连接WIFI
void doWiFiTick();

// 初始化wifi连接
void startSTA();

// 检查数据,发送心跳,断开连接后尝试重连
void doTCPClientTick(CRGB leds[]);

// 初始化TCP和服务器建立连接
void startTCPClient();

// 发送数据到TCP服务器
void sendtoTCPServer(String p);

// 开启LED
void turnOnLed();

// 关闭LED
void turnOffLed();

// 表情信息解码
void decodeHexString(const String hexString,int cells[16][18]);

// 更新脸部表情
void face_update(const String hexString,CRGB leds[]);