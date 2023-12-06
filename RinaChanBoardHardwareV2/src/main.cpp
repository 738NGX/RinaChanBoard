#include <Arduino.h>
#include <FastLED.h>

#include <led.h>
#include <configwifi.h>
#include <udpsocket.h>

CRGB leds[NUM_LEDS];
CRGB def_color(249,113,212);

void setup()
{
    Serial.begin(9600);
    pinMode(DATA_PIN,OUTPUT);
    LEDS.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS); 
    init_led(leds,def_color);
    Serial.println("LED初始化完成,WIFI连接程序开始.");

    init_wifi_manager();
    init_wifi();
}

void loop()
{
    UDPSocket(leds);
}