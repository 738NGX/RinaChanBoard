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
    // set led pin as output
    pinMode(LED_BUILTIN, OUTPUT);
    // start ticker with 0.5 because we start in AP mode and try to connect
    ticker.attach(0.6, tick);
}

void init_wifi_manager()
{
    /***  步骤一：创建 wifimanager对象 **/
    WiFiManager wifiManager;
    /*************************************/
    /*** 步骤二：进行一系列配置，参考配置类方法 **/
    // 重置保存的修改 目标是为了每次进来都是去掉配置页面
    wifiManager.resetSettings();
    ESP.eraseConfig(); 
    // 配置连接超时
    wifiManager.setConnectTimeout(0);
    // 打印调试内容
    wifiManager.setDebugOutput(true);
    // 设置最小信号强度
    wifiManager.setMinimumSignalQuality(30);
    // 设置固定AP信息
    IPAddress _ip=IPAddress(192,168, 11, 13);
    IPAddress _gw=IPAddress(192,168,  4,  1);
    IPAddress _sn=IPAddress(255,255,255,  0);
    wifiManager.setAPStaticIPConfig(_ip,_gw,_sn);
    // 设置进入AP模式的回调
    wifiManager.setAPCallback(configModeCallback);
    // 设置点击保存的回调
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    // 设置 如果配置错误的ssid或者密码 退出配置模式
    wifiManager.setBreakAfterConfig(true);
    // 设置过滤重复的AP 默认可以不用调用 这里只是示范
    wifiManager.setRemoveDuplicateAPs(true);

    /*************************************/
    /*** 步骤三：尝试连接网络，失败去到配置页面 **/
    if(!wifiManager.autoConnect("RinaChanBoard"))
    {
        Serial.println("failed to connect and hit timeout");
        // reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(1000);
    }

    Serial.println("local ip");
    Serial.println(WiFi.localIP());
}

/**
 * 功能描述：配置进入AP模式通知回调
 */
void configModeCallback(WiFiManager *myWiFiManager)
{
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    // if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
    // entered config mode, make led toggle faster
    ticker.attach(0.2, tick);
}

/**
 * 功能描述：设置点击保存的回调
 */
void saveConfigCallback()
{
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

/**
 * 功能描述：设置LED灯闪烁，提示用户进入配置模式
 */
void tick()
{
    // toggle state
    int state = digitalRead(LED_BUILTIN); // get the current state of GPIO1 pin
    digitalWrite(LED_BUILTIN, !state);    // set pin to the opposite state
}

void end_wifi_manager()
{
    ticker.detach();
    // keep LED on
    digitalWrite(LED_BUILTIN, LOW);
}
