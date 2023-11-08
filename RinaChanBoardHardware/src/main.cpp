#include <Arduino.h>
#include <FastLED.h>

#include <bemfa.h>
#include <face.h>
#include <voice.h>
 
#define NUM_LEDS 256
#define DATA_PIN D1
#define BUTTON D2
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
 
uint8_t max_bright=64;
 
CRGB leds[NUM_LEDS];

void setup()
{
    Serial.begin(9600);
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(DATA_PIN,OUTPUT);
    pinMode(BUTTON,INPUT_PULLUP);
    
    digitalWrite(LED_BUILTIN,HIGH);
    
    LEDS.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS); 
    FastLED.setBrightness(max_bright);

    Serial.println("Beginning...");
}

void loop()
{
    doWiFiTick();
    doTCPClientTick(leds);
}