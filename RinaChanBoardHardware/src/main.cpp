#include <Arduino.h>
#include <FastLED.h>

#include <bemfa.h>
#include <face.h>
#include <voice.h>

CRGB leds[NUM_LEDS];

void setup()
{
    Serial.begin(9600);
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(DATA_PIN,OUTPUT);
    pinMode(BUTTON,INPUT_PULLUP);
    
    digitalWrite(LED_BUILTIN,HIGH);
    
    LEDS.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS); 
    FastLED.setBrightness(64);
    CRGB def_color(249,113,212);
    face_update_by_string("0000000007def810a205ef810a205ef800000000117a26505155884d25117a2000000000",leds,def_color);
    FastLED.show();
    delay(1000);
    face_update_by_string("0000000001e3e044201e0804820113e00000000004420191405488133e04488000000000",leds,def_color);
    FastLED.show();
    delay(1000);
    set_face(leds,def_color,1,1,7,0);
    FastLED.show();

    Serial.println("Beginning...");
}

void loop()
{
    doWiFiTick();
    doTCPClientTick(leds);
}