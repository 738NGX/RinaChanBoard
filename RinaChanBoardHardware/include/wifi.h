#pragma once
#include <Arduino.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Ticker.h>

void init_led_builtin();

void init_wifi_manager();

/**
 * @brief 配置进入AP模式通知回调
 * 
 * @param myWiFiManager 
 */
void configModeCallback(WiFiManager *myWiFiManager);

/**
 * @brief 设置点击保存的回调
 * 
 */
void saveConfigCallback();

/**
 * @brief 设置LED灯闪烁,提示用户进入配置模式
 * 
 */
void tick();

void end_wifi_manager();