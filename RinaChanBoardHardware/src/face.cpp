#include <Arduino.h>
#include <FastLED.h>

#include <voice.h>
#include <face.h>
#include <bemfa.h>

const int leye[][8][8]=LEYE;
const int mouth[][8][8]=MOUTH;
const int cheek[][2][5]=CHEEK;

void face_update(int face[16][18],CRGB leds[])
{
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<16;j++)
        {
            if(i%2) leds[16*i+j]=face[15-i][16-j] ? CRGB::White : CRGB::Black;
            else leds[16*i+j]= face[15-i][j+1] ? CRGB::White : CRGB::Black;
        }
    }
}

void face_update_by_string(const String hexString,CRGB leds[])
{
    int face[16][18];
    decodeHexString(hexString,face);
    face_update(face,leds);
    FastLED.show();
}

void set_face(CRGB leds[],int leye_idx,int reye_idx,int mouth_idx,int cheek_idx)
{
    int face[16][18]={0};

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            face[i][j]=leye[leye_idx][i][j];
            face[i][j+9]=leye[reye_idx][i][8-j];
            face[i+8][j+5]=mouth[mouth_idx][i][j];
        }
    }
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<5;j++)
        {
            face[i+8][j]=cheek[cheek_idx][i][j];
            face[i+8][j+13]=cheek[cheek_idx][i][5-j];
        }
    }
    face_update(face,leds);
    FastLED.show();
}