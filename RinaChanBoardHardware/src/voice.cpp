#include <Arduino.h>
#include <FastLED.h>

#include <voice.h>
#include <face.h>
#include <bemfa.h>

#include <map>

const int voice_data[][5]=VOICE_DATA;

const int voice_data_range[][2]=VOICE_DATA_RANGE;

const std::map<String,int> ids={
    {"vo_na_m0209_0001",0},{"vo_na_m0209_0002",1},{"vo_na_m0209_0003",2},
};

void play_voice_face(CRGB leds[],const String id)
{
    int idx=ids.at(id);
    for(int i=voice_data_range[idx][0];i<voice_data_range[idx][1];i++)
    {
        set_face(leds,voice_data[i][0],voice_data[i][1],voice_data[i][2],voice_data[i][3]);
        FastLED.show();
        delay(1000/FPS*voice_data[i][4]);
    }
}

