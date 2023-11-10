#pragma once
#include <Arduino.h>
#include <FastLED.h>

void play_voice_face(CRGB leds[],const String id);

#define VOICE_DATA {\
    { 2, 2, 6, 0, 19},\
    { 2, 2, 3, 0,  3},\
    { 2, 2, 6, 0,  7},\
    { 2, 2, 3, 0, 27},\
    { 1, 1, 3, 0,  3},\
    { 1, 1, 6, 0,  6},\
    { 1, 1, 3, 0,  4},\
    { 1, 1, 6, 0, 20},\
    { 1, 1, 3, 0,  9},\
    { 4, 4, 6, 2,  3},\
    { 4, 4, 3, 2,  6},\
    { 4, 4, 6, 2,  4},\
    { 4, 4, 3, 2,  6},\
    { 4, 4, 6, 2, 11},\
    { 4, 4, 3, 2,  4},\
    { 1, 1, 3, 0,  1},\
}

#define VOICE_DATA_RANGE {\
    {0,16},\
}
