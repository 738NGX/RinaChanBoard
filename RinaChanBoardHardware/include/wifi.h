#pragma once
#include <Arduino.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <Ticker.h>

void init_led_builtin();

void init_wifi_manager();

void configModeCallback(WiFiManager *myWiFiManager);

void saveConfigCallback();

void tick();

void end_wifi_manager();