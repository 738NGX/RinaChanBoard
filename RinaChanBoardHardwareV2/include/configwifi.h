#pragma once
#include <Arduino.h>

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

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
