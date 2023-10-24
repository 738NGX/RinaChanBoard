#include <Arduino.h>
#include <ESP8266WiFi.h>

#define server_ip "bemfa.com" // 巴法云服务器地址默认即可
#define server_port "8344"    // 服务器端口，tcp创客云端口8344

//********************需要修改的部分*******************//

// WIFI名称，区分大小写，不要写错
#define DEFAULT_STASSID "New_Media"
// WIFI密码
#define DEFAULT_STAPSW "sufexmt123*"
// 用户私钥，可在控制台获取,修改为自己的UID
#define UID "a8a83e1f0a4c4e42b031e1c323dd9159"
// 主题名字，可在控制台新建
#define TOPIC "RinaChanBoard"
// 需要控制的单片机LED引脚值
//#define LED_BUILTIN 2

//**************************************************//
// 最大字节数
#define MAX_PACKETSIZE 512
// 设置心跳值60s
#define KEEPALIVEATIME 60 * 1000
// tcp客户端相关初始化，默认即可
WiFiClient TCPclient;
String TcpClient_Buff = ""; // 初始化字符串，用于接收服务器发来的数据
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long preHeartTick = 0;    // 心跳
unsigned long preTCPStartTick = 0; // 连接
bool preTCPConnected = false;
// 相关函数初始化
// 连接WIFI
void doWiFiTick();
void startSTA();

// TCP初始化连接
void doTCPClientTick();
void startTCPClient();
void sendtoTCPServer(String p);

// led控制函数，具体函数内容见下方
void turnOnLed();
void turnOffLed();

/*
 *发送数据到TCP服务器
 */
void sendtoTCPServer(String p)
{
    if (!TCPclient.connected())
    {
        Serial.println("Client is not readly");
        return;
    }
    TCPclient.print(p);
    preHeartTick = millis(); // 心跳计时开始，需要每隔60秒发送一次数据
}

/*
 *初始化和服务器建立连接
 */
void startTCPClient()
{
    if (TCPclient.connect(server_ip, atoi(server_port)))
    {
        Serial.print("\nConnected to server:");
        Serial.printf("%s:%d\r\n", server_ip, atoi(server_port));

        String tcpTemp = "cmd=1&uid=";
        tcpTemp += UID; 
        tcpTemp += "&topic=";
        tcpTemp += TOPIC;
        tcpTemp += "\r\n";
        sendtoTCPServer(tcpTemp);
        tcpTemp = "";

        preTCPConnected = true;
        TCPclient.setNoDelay(true);
    }
    else
    {
        Serial.print("Failed connected to server:");
        Serial.println(server_ip);
        TCPclient.stop();
        preTCPConnected = false;
    }
    preTCPStartTick = millis();
}

/*
 *检查数据，发送心跳
 */
void doTCPClientTick()
{
    // 检查是否断开，断开后重连
    if (WiFi.status() != WL_CONNECTED)
        return;
    if (!TCPclient.connected())
    { // 断开重连
        if (preTCPConnected == true)
        {
            preTCPConnected = false;
            preTCPStartTick = millis();
            Serial.println();
            Serial.println("TCP Client disconnected.");
            TCPclient.stop();
        }
        else if (millis() - preTCPStartTick > 1 * 1000) // 重新连接
            startTCPClient();
    }
    else
    {
        if (TCPclient.available())
        { // 收数据
            char c = TCPclient.read();
            TcpClient_Buff += c;
            TcpClient_BuffIndex++;
            TcpClient_preTick = millis();

            if (TcpClient_BuffIndex >= MAX_PACKETSIZE - 1)
            {
                TcpClient_BuffIndex = MAX_PACKETSIZE - 2;
                TcpClient_preTick = TcpClient_preTick - 200;
            }
        }
        if (millis() - preHeartTick >= KEEPALIVEATIME)
        { // 保持心跳
            preHeartTick = millis();
            Serial.println("--Keep alive:");
            sendtoTCPServer("ping\r\n"); // 发送心跳，指令需\r\n结尾，详见接入文档介绍
        }
    }
    if ((TcpClient_Buff.length() >= 1) && (millis() - TcpClient_preTick >= 200))
    {
        TCPclient.flush();
        Serial.print("Rev string: ");
        TcpClient_Buff.trim();          // 去掉首位空格
        Serial.println(TcpClient_Buff); // 打印接收到的消息
        String getTopic = "";
        String getMsg = "";
        if (TcpClient_Buff.length() > 15)
        { // 注意TcpClient_Buff只是个字符串，在上面开头做了初始化 String TcpClient_Buff = "";
            // 此时会收到推送的指令，指令大概为 cmd=2&uid=xxx&topic=light002&msg=off
            int topicIndex = TcpClient_Buff.indexOf("&topic=") + 7;    // c语言字符串查找，查找&topic=位置，并移动7位，不懂的可百度c语言字符串查找
            int msgIndex = TcpClient_Buff.indexOf("&msg=");            // c语言字符串查找，查找&msg=位置
            getTopic = TcpClient_Buff.substring(topicIndex, msgIndex); // c语言字符串截取，截取到topic,不懂的可百度c语言字符串截取
            getMsg = TcpClient_Buff.substring(msgIndex + 5);           // c语言字符串截取，截取到消息
            Serial.print("topic:------");
            Serial.println(getTopic); // 打印截取到的主题值
            Serial.print("msg:--------");
            Serial.println(getMsg); // 打印截取到的消息值
        }
        if (getMsg == "on")
        { // 如果是消息==打开
            turnOnLed();
        }
        else if (getMsg == "off")
        { // 如果是消息==关闭
            turnOffLed();
        }

        TcpClient_Buff = "";
        TcpClient_BuffIndex = 0;
    }
}
/*
 *初始化wifi连接
 */
void startSTA()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(DEFAULT_STASSID, DEFAULT_STAPSW);
}

/**************************************************************************
                                 WIFI
***************************************************************************/
/*
  WiFiTick
  检查是否需要初始化WiFi
  检查WiFi是否连接上，若连接成功启动TCP Client
  控制指示灯
*/
void doWiFiTick()
{
    static bool startSTAFlag = false;
    static bool taskStarted = false;
    static uint32_t lastWiFiCheckTick = 0;

    if (!startSTAFlag)
    {
        startSTAFlag = true;
        startSTA();
    }

    // 未连接1s重连
    if (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - lastWiFiCheckTick > 1000)
        {
            lastWiFiCheckTick = millis();
        }
    }
    // 连接成功建立
    else
    {
        if (taskStarted == false)
        {
            taskStarted = true;
            Serial.print("\r\nGet IP Address: ");
            Serial.println(WiFi.localIP());
            startTCPClient();
        }
    }
}
// 打开灯泡
void turnOnLed()
{
    Serial.println("Turn ON");
    digitalWrite(LED_BUILTIN, LOW);
}
// 关闭灯泡
void turnOffLed()
{
    Serial.println("Turn OFF");
    digitalWrite(LED_BUILTIN, HIGH);
}

// 初始化，相当于main 函数
void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Beginning...");
}

// 循环
void loop()
{
    doWiFiTick();
    doTCPClientTick();
}