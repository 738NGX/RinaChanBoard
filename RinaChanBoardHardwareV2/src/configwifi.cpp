#include <Arduino.h>

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <Wifi.h>

#include <configwifi.h>

bool shouldSaveConfig = false;

void init_wifi_manager()
{
    WiFiManager wifiManager;

    wifiManager.resetSettings();

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
        ESP.restart();
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
}

void saveConfigCallback()
{
    Serial.println("Should save config");
    shouldSaveConfig = true;
}
