#pragma once
#include <Arduino.h>
#include <FastLED.h>

#define FPS 30.0

String get_face(CRGB leds[]);

// 更新脸部表情
void face_update(int face[16][18],CRGB leds[],CRGB color);
void face_update_by_string(const String hexString,CRGB leds[],CRGB color);
