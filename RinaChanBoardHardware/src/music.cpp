#include <Arduino.h>
#include <FastLED.h>

#include <music.h>
#include <face.h>
#include <bemfa.h>

#include <map>

const int music_data[][5]=MUSIC_DATA;

const int music_data_range[][2]=MUSIC_DATA_RANGE;

const std::map<String,int> ids={
    {"music_02_00_02_09_0",0},
};

void play_music_face(CRGB leds[],const String id)
{
    int idx=ids.at(id);
    for(int i=music_data_range[idx][0];i<music_data_range[idx][1];i++)
    {
        set_face(leds,music_data[i][0],music_data[i][1],music_data[i][2],music_data[i][3]);
        FastLED.show();
        delay(1000/FPS*music_data[i][4]);
    }
}