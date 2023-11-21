#include <Arduino.h>
#include <FastLED.h>

#include <face.h>
#include <bemfa.h>

void face_update(int face[16][18],CRGB leds[],CRGB color)
{
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<16;j++)
        {
            if(i%2) leds[16*i+j]=face[15-i][16-j] ? color : CRGB::Black;
            else leds[16*i+j]= face[15-i][j+1] ? color : CRGB::Black;
        }
    }
}

void face_update_by_string(const String hexString,CRGB leds[],CRGB color)
{
    int face[16][18];
    decodeHexString(hexString,face);
    face_update(face,leds,color);
    FastLED.show();
}

String get_face(CRGB leds[]) {
    int face[16][18]={0};

    for(int i=0;i<16;i++) 
    {
        for(int j=0;j<16;j++)
        {
            if(i%2==0) 
            {
                face[15-i][j+1]=leds[16*i + j]==CRGB::Black ? 0 : 1;
            }
            else
            {
                face[15-i][16-j]=leds[16 * i + j]==CRGB::Black ? 0 : 1;
            }
        }
    }

    String binaryString;
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<18;j++)
        {
            binaryString+=face[i][j] ? '1' : '0';
        }
    }

    String hexString;
    for(size_t i=0;i<binaryString.length();i+=4)
    {
        int value = 0;
        for(int j=0;j<4; j++)
        {
            value = (value << 1) | (binaryString[i + j] - '0');
        }
        hexString += String(value, HEX);
    }

    return hexString;
}