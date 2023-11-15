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

void play_music_face(CRGB leds[],CRGB color,const String id,int start)
{
    int idx=ids.at(id);
    int current_frame=start;
    int end_face=music_data_range[idx][1];
    int current_face=music_data_range[idx][0];

    set_face(leds,color,music_data[current_face][0],music_data[current_face][1],music_data[current_face][2],music_data[current_face][3]);
    FastLED.show();
    Serial.println(current_frame);
    Serial.println(music_data[end_face-1][4]);
    while(current_frame<music_data[end_face-1][4])
    {
        for(int i=current_face;i<end_face;i++)
        {
            if(current_frame<music_data[i][4])
            {
                if(i-1==current_face) break;
                current_face=i-1;
                set_face(leds,color,music_data[i-1][0],music_data[i-1][1],music_data[i-1][2],music_data[i-1][3]);
                FastLED.show();
                break;
            }
        }
        current_frame++;
        delay(98);
    }
    Serial.println("ended play");
}