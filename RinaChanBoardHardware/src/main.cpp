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

    Serial.println("Beginning...");
}

void loop()
{
    doWiFiTick();
    doTCPClientTick(leds);
}