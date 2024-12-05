#include <Arduino.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Ticker.h>

#include <wifi.h>

bool shouldSaveConfig = false;

Ticker ticker;

void init_led_builtin()
{
    pinMode(LED_BUILTIN, OUTPUT);
    ticker.attach(0.6, tick);
}

void init_wifi_manager()
{
    WiFiManager wifiManager;

    wifiManager.resetSettings();
    ESP.eraseConfig(); 
    wifiManager.setConnectTimeout(0);
    wifiManager.setDebugOutput(true);
    wifiManager.setMinimumSignalQuality(30);

    IPAddress _ip=IPAddress(192,168, 11, 13);
    IPAddress _gw=IPAddress(192,168,  4,  1);
    IPAddress _sn=IPAddress(255,255,255,  0);

    wifiManager.setAPStaticIPConfig(_ip,_gw,_sn);
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.setBreakAfterConfig(true);
    wifiManager.setRemoveDuplicateAPs(true);

    if(!wifiManager.autoConnect("RinaChanBoard"))
    {
        Serial.println("failed to connect and hit timeout");
        ESP.reset();
        delay(1000);
    }

    Serial.println("local ip");
    Serial.println(WiFi.localIP());
}

void configModeCallback(WiFiManager *myWiFiManager)
{
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    Serial.println(myWiFiManager->getConfigPortalSSID());
    ticker.attach(0.2, tick);
}

void saveConfigCallback()
{
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

void tick()
{
    int state = digitalRead(LED_BUILTIN);
    digitalWrite(LED_BUILTIN, !state);
}

void end_wifi_manager()
{
    ticker.detach();
    digitalWrite(LED_BUILTIN, LOW);
}
