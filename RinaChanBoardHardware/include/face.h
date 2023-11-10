#pragma once
#include <Arduino.h>
#include <FastLED.h>

#define FPS 30.0

// 更新脸部表情
void face_update(int face[16][18],CRGB leds[]);
void face_update_by_string(const String hexString,CRGB leds[]);

// 从预设库中设置表情
void set_face(CRGB leds[],int leye_idx,int reye_idx,int mouth_idx,int cheek_idx);

#define MOUTH {\
{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,1,1,1,1,1,1,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {1,0,0,0,0,0,0,1},\
    {0,1,1,1,1,1,1,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {1,0,0,0,0,0,0,1},\
    {0,1,0,0,0,0,1,0},\
    {0,0,1,1,1,1,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,1,0,0,0,0,1,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {1,1,1,1,1,1,1,1},\
    {1,0,0,0,0,0,0,1},\
    {0,1,0,0,0,0,1,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {1,1,1,1,1,1,1,1},\
    {1,0,0,0,0,0,0,1},\
    {1,0,0,0,0,0,0,1},\
    {0,1,0,0,0,0,1,0},\
    {0,0,1,1,1,1,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,1,1,1,1,1,1,0},\
    {0,1,0,0,0,0,1,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,1,1,1,1,0,0},\
    {0,1,0,0,0,0,1,0},\
    {1,0,0,0,0,0,0,1},\
    {1,0,0,0,0,0,0,1},\
    {1,1,1,1,1,1,1,1},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {1,1,1,1,1,1,1,1},\
    {1,0,0,0,0,0,0,1},\
    {0,1,1,1,1,1,1,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,1,0,0,0,0,1,0},\
    {0,1,0,0,0,0,1,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,0,1,1,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,1,1,0,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,1,1,1,1,1,1,0},\
    {1,0,0,0,0,0,0,1},\
    {1,1,1,1,1,1,1,1},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,1,0,0,1,0,0},\
    {0,1,0,1,1,0,1,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {1,0,0,0,0,0,0,1},\
    {0,1,0,0,0,0,1,0},\
    {0,1,1,1,1,1,1,0},\
    {0,1,0,0,0,0,1,0},\
    {1,0,0,0,0,0,0,1},\
    {0,0,0,0,0,0,0,0},\
}}

# define LEYE {\
{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,1,0,0,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,0,1,0,1,0,0},\
    {0,0,1,0,0,0,1,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,1,1,0,0,0,0},\
    {0,0,0,0,1,1,0,0},\
    {0,0,0,0,0,0,1,0},\
    {0,0,0,0,1,1,0,0},\
    {0,0,1,1,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,1,1,0,1,0,0},\
    {0,0,1,1,1,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,1,0,0,0,0,0,0},\
    {0,0,1,1,1,1,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,1,1,1,1,1,1,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,1,1,1,1,1,1,0},\
    {1,0,1,0,0,0,0,0},\
    {0,1,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,1,1,1,0,0},\
    {0,0,1,0,0,0,1,0},\
    {0,0,1,0,0,0,1,0},\
    {0,0,0,0,0,1,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,1,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,1,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,1,1,0,0,0,0},\
    {0,0,1,1,1,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,1,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,1,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,1,0,0,0,0,0},\
    {0,0,0,1,0,0,0,0},\
    {0,0,0,1,1,1,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,1,1,0,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,1,0,0,0,1,0},\
    {0,0,0,1,0,1,0,0},\
    {0,0,0,0,1,0,0,0},\
    {0,0,0,1,0,1,0,0},\
    {0,0,1,0,0,0,1,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,1,1,1,0,0},\
    {0,0,1,0,0,0,1,0},\
    {0,0,1,0,0,0,1,0},\
    {0,0,1,0,0,0,1,0},\
    {0,0,0,1,1,1,0,0},\
    {0,0,0,0,0,0,0,0},\
},{\
    {0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0},\
    {0,0,1,1,0,1,1,0},\
    {0,1,0,0,1,0,0,1},\
    {0,1,0,0,0,0,0,1},\
    {0,0,1,0,0,0,1,0},\
    {0,0,0,1,0,1,0,0},\
    {0,0,0,0,1,0,0,0},\
}}

#define CHEEK {\
{\
    {0,0,0,0,0},\
    {0,0,0,0,0},\
},{\
    {0,0,0,0,0},\
    {0,0,1,1,0},\
},{\
    {0,0,0,0,0},\
    {0,0,1,0,1},\
},{\
    {0,0,1,0,1},\
    {0,1,0,1,0},\
},{\
    {0,1,0,1,0},\
    {0,0,1,0,1},\
}}