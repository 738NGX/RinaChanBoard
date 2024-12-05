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
void init_led(CRGB leds[], CRGB color);

/**
 * @brief 表情信息解码
 *
 * @param hexString
 * @param cells
 */
void decodeHexString(const String hexString, int cells[16][18]);

/**
 * @brief 将颜色编码为十六进制字符串
 *
 */
// [[deprecated]]String encodeColor(unsigned R, unsigned int G, unsigned int B);

/**
 * @brief 将亮度编码为十进制字符串
 *
 */
// [[deprecated]]String encodeBright(u_int8_t bright);

/**
 * @brief 更新led颜色
 *
 * @param color_code
 * @param leds
 */
void updateColor(String color_code, CRGB leds[], unsigned int &R, unsigned int &G, unsigned int &B);

/**
 * @brief 从数组更新璃奈板表情
 *
 * @param face 16*18的数组
 * @param leds 目标led
 * @param color 目标颜色
 */
void face_update(int face[16][18], CRGB leds[], CRGB color);

/**
 * @brief 从状态字符串更新璃奈板表情
 *
 * @param hexString 72位长的十六进制状态字符串,可以被译码位16*18的数组
 * @param leds 目标led
 * @param color 目标颜色
 */
[[deprecated]] void face_update_by_string(const String hexString, CRGB leds[], CRGB color);

/**
 * @brief 从led开闭状态获取状态字符串
 *
 * @param leds 源led
 * @return String 状态字符串
 */
[[deprecated]] String get_face(CRGB leds[]);
/**
 * @brief 从led开闭状态获取状态，使用bit位表示
 *
 * @param leds      in  源led
 * @param hexData   out 目标数组
 */

void get_face_hex(CRGB leds[], uint8_t *hexData);