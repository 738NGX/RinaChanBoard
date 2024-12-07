#pragma once
#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS    270
#define DATA_PIN    12

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

/**
 * @brief led灯初始化
 *
 * @param leds
 * @param color
 */
void initLED(CRGB leds[], CRGB color);

/**
 * @brief 表情信息解码
 *
 * @param hexBytes  [in]
 * @param cells     [out]
 * @param length    [in]

 */
void decodeFaceHex(const char hexBytes[], uint8_t (&cells)[16][18], size_t length);

/**
 * @brief 通过RGB值直接更新led颜色
 *
 * @param color_code
 * @param leds
 */
void updateColor(CRGB leds[], const uint8_t &R, const uint8_t &G, const uint8_t &B);

/**
 * @brief 使用HexString更新led颜色，不要用于传输
 *
 * @param color_code
 * @param leds
 */
[[deprecated]] void updateColor(String color_code, CRGB leds[], uint8_t &R, uint8_t &G, uint8_t &B);

/**
 * @brief 从数组更新璃奈板表情
 *
 * @param face 16*18的数组
 * @param leds 目标led
 * @param color 目标颜色
 */
void faceUpdate(uint8_t face[16][18], CRGB leds[], CRGB color);

/**
 * @brief
 *
 * @param[in]   LEyeCode   左眼表情编号
 * @param[in]   REyeCode   右眼表情编号
 * @param[in]   MouthCode  嘴巴表情编号
 * @param[in]   cheekCode  脸颊表情编号
 * @param[out]  leds       目标led矩阵
 * @param[in]   color      目标颜色
 */
void faceUpdate_LitePack(const uint8_t LEyeCode,
                         const uint8_t REyeCode,
                         const uint8_t MouthCode,
                         const uint8_t cheekCode,
                         CRGB (&leds)[],
                         CRGB color);

/**
 * @brief 从状态字符串更新璃奈板表情
 * @warning 注意：不要用于UDP传输
 * @param hexString 72位长的十六进制状态字符串,可以被译码位16*18的数组
 * @param leds 目标led
 * @param color 目标颜色
 */
void faceUpdateByString(const String hexString, CRGB leds[], CRGB color);

/**
 * @brief 从led开闭状态获取状态字符串
 *
 * @param leds 源led
 * @return String 状态字符串
 */

void getFaceHex(CRGB leds[], uint8_t *hexData);
