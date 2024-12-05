#pragma once
#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 256
#define DATA_PIN D1
#define BUTTON D2
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

/**
 * @brief led灯初始化
 * 
 * @param leds 
 * @param color 
 */
void init_led(CRGB leds[],CRGB color);

/**
 * @brief 表情信息解码
 * 
 * @param hexString 
 * @param cells 
 */
void decodeHexString(const String hexString,int cells[16][18]);

/**
 * @brief 将颜色编码为十六进制字符串
 * 
 */
String encodeColor(unsigned R,unsigned int G,unsigned int B);

/**
 * @brief 将亮度编码为十进制字符串
 * 
 */
String encodeBright(u_int8_t bright);

/**
 * @brief 更新led颜色
 * 
 * @param color_code 
 * @param leds 
 */
void updateColor(String color_code,CRGB leds[],unsigned int &R,unsigned int &G,unsigned int &B);

/**
 * @brief 从数组更新璃奈板表情
 * 
 * @param face 16*18的数组 
 * @param leds 目标led 
 * @param color 目标颜色
 */
void face_update(int face[16][18],CRGB leds[],CRGB color);

/**
 * @brief 从状态字符串更新璃奈板表情
 * 
 * @param hexString 72位长的十六进制状态字符串,可以被译码位16*18的数组
 * @param leds 目标led
 * @param color 目标颜色
 */
void face_update_by_string(const String hexString,CRGB leds[],CRGB color);

/**
 * @brief 从led开闭状态获取状态字符串
 * 
 * @param leds 源led
 * @return String 状态字符串
 */
String get_face(CRGB leds[]);
