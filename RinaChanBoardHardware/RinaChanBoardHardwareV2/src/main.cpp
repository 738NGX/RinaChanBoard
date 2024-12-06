#include "esp_wifi.h"
#include <Arduino.h>
#include <FastLED.h>

#include <led.h>
#include <configwifi.h>
#include <udpsocket.h>

CRGB leds[NUM_LEDS];
CRGB def_color(249, 113, 212);
LedUDPHandler udpHandler(leds);
HardwareSerial debugSerial(0);

void setup()
{
    debugSerial.begin(115200, SERIAL_8N1);
    pinMode(DATA_PIN, OUTPUT);
    LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    init_led(leds, def_color);
    debugSerial.println("LED初始化完成,WIFI连接程序开始.");
    esp_wifi_set_ps(WIFI_PS_NONE);

    face_update_by_string("0000000001c0408810040402010000002010000000000033001320040801320033000000", leds, def_color);
    FastLED.show();
    init_wifi_manager();
    face_update_by_string("00000000000000c00630060c02030078000000001400a01e00084002100048000c000000", leds, def_color);
    FastLED.show();
    udpHandler.begin();
    face_update_by_string("00000000000000c00c30030c00c30030000000000000003f000840012000300000000000", leds, def_color);
    FastLED.show();
}

void loop()
{
    delay(500);// 空转，防止占用CPU
}