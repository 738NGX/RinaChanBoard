#include <FastLED.h>
#include <Arduino.h>

#include <face.h>

void face_update(CRGB leds[],const int (*face)[16])
{
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<16;j++)
        {
            leds[16*i+j]= face[i][j] ? CRGB::White : CRGB::Black;
        }
    }
}