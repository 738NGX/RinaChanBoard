#include <ESP8266WiFi.h>
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
    digitalWrite(LED_BUILTIN,HIGH);
    pinMode(DATA_PIN, OUTPUT);
    LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    initLED(leds, def_color);
    debugSerial.println("LED初始化完成,WIFI连接程序开始.");
    WiFi.setSleep(false);
    // esp_wifi_set_ps(WIFI_PS_NONE);

    faceUpdate_StringFullPack("0000000001c0408810040402010000002010000000000033001320040801320033000000", leds, def_color);
    FastLED.show();
    init_wifi_manager();
    faceUpdate_StringFullPack("00000000000000c00630060c02030078000000001400a01e00084002100048000c000000", leds, def_color);
    FastLED.show();
    udpHandler.begin();
    faceUpdate_StringFullPack("00000000000000c00c30030c00c30030000000000000003f000840012000300000000000", leds, def_color);
    FastLED.show();
    end_wifi_manager();
}

void loop()
{
    delay(500); // 空转，防止占用CPU
}