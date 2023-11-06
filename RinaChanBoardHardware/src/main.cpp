#include <Arduino.h>
#include <FastLED.h>

#include <bemfa.h>
 
#define NUM_LEDS 256
#define DATA_PIN D1
#define BUTTON D2
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
 
uint8_t max_bright = 64;
 
CRGB leds[NUM_LEDS];

int face_id;

//const int face0[16][16]=FACE0;
//const int face1[16][16]=FACE1;
//const int face2[16][16]=FACE2;
//const int face3[16][16]=FACE3;
//const int face4[16][16]=FACE4;

// 初始化，相当于main 函数
void setup()
{
    Serial.begin(9600);
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(DATA_PIN,OUTPUT);
    pinMode(BUTTON,INPUT_PULLUP);
    
    digitalWrite(LED_BUILTIN, HIGH);
    LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带 
    FastLED.setBrightness(max_bright);
    face_id=0;

    Serial.println("Beginning...");
}

// 循环

void loop()
{
    doWiFiTick();
    doTCPClientTick(leds);
    
    //int switchValue = 0; //定义变量并赋初值为0
    //Serial.print(" Value of switch = ");  //输出到串口监视器
    //Serial.println(switchValue); //将读取的按键值输出到串口监视器
    //if(digitalRead(BUTTON)) 
    
    /*
    // 更新LED色彩
    if(digitalRead(BUTTON) == HIGH)
    {
        face_id++;
        if(face_id>4) face_id=0;
        switch(face_id)
        {
            case 0:face_update(leds,face0);break;
            case 1:face_update(leds,face1);break;
            case 2:face_update(leds,face2);break;
            case 3:face_update(leds,face3);break;
            case 4:face_update(leds,face4);break;
            default:face_update(leds,face0);
        }
        Serial.print("Changed Face to ");
        Serial.println(face_id);
        delay(200);//延时消抖
    }
    FastLED.show();
    delay(100);//延时消抖
    */
}