#include <Arduino.h>
#include <FastLED.h>
#include <udpsocket.h>
#include <led.h>
#include <wifi.h>

#define firmware_version "0.0.7"

CRGB leds[NUM_LEDS];
CRGB def_color(249,113,212);

void setup()
{
    Serial.begin(9600);
    init_led_builtin();
    digitalWrite(LED_BUILTIN,HIGH);
    pinMode(DATA_PIN,OUTPUT);
    LEDS.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS); 
    init_led(leds,def_color);
    Serial.println("LED初始化完成,WIFI连接程序开始.");

    init_wifi_manager();
    init_wifi();
    end_wifi_manager();
}

void loop()
{
    UDPSocket(leds);
}
