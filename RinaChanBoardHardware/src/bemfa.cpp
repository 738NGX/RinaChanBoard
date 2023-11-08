#include <Arduino.h>
#include <FastLED.h>

#include <ESP8266WiFi.h>

#include <voice.h>
#include <face.h>
#include <bemfa.h>

// tcp客户端相关初始化
WiFiClient TCPclient;
String TcpClient_Buff = "";                 // 初始化字符串,用于接收服务器发来的数据
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long preHeartTick = 0;             // 心跳
unsigned long preTCPStartTick = 0;          // 连接
bool preTCPConnected = false;

void sendtoTCPServer(String p)
{
    if (!TCPclient.connected())
    {
        Serial.println("Client is not readly");
        return;
    }
    TCPclient.print(p);
    preHeartTick=millis();                  // 心跳计时开始,需要每隔60秒发送一次数据
}

void startTCPClient()
{
    if (TCPclient.connect(server_ip,atoi(server_port)))
    {
        Serial.print("\nConnected to server:");
        Serial.printf("%s:%d\r\n",server_ip,atoi(server_port));

        String tcpTemp="cmd=1&uid=";
        tcpTemp+=UID; 
        tcpTemp+="&topic=";
        tcpTemp+=TOPIC;
        tcpTemp+="\r\n";
        sendtoTCPServer(tcpTemp);
        tcpTemp="";

        preTCPConnected = true;
        TCPclient.setNoDelay(true);
    }
    else
    {
        Serial.print("Failed connected to server:");
        Serial.println(server_ip);
        TCPclient.stop();
        preTCPConnected=false;
    }
    preTCPStartTick=millis();
}

void doTCPClientTick(CRGB leds[])
{
    if(WiFi.status()!=WL_CONNECTED) return;
    // 尝试断开重连
    if(!TCPclient.connected())
    { 
        if(preTCPConnected==true)
        {
            preTCPConnected=false;
            preTCPStartTick=millis();
            Serial.println();
            Serial.println("TCP Client disconnected.");
            TCPclient.stop();
        }
        else if(millis()-preTCPStartTick>1*1000)
        {
            startTCPClient();   // 重新连接
        }  
    }
    else
    {
        // 接收数据
        if(TCPclient.available())
        {   
            char c=TCPclient.read();
            TcpClient_Buff+=c;
            TcpClient_BuffIndex++;
            TcpClient_preTick=millis();

            if(TcpClient_BuffIndex>=MAX_PACKETSIZE-1)
            {
                TcpClient_BuffIndex=MAX_PACKETSIZE-2;
                TcpClient_preTick=TcpClient_preTick-200;
            }
        }
        // 保持心跳
        if(millis()-preHeartTick>=KEEPALIVEATIME)
        { 
            preHeartTick=millis();
            Serial.println("--Keep alive:");
            sendtoTCPServer("ping\r\n");        // 发送心跳,指令需\r\n结尾,详见接入文档介绍
        }
    }
    if((TcpClient_Buff.length()>=1)&&(millis()-TcpClient_preTick>=200))
    {
        TCPclient.flush();
        Serial.print("Rev string: ");
        TcpClient_Buff.trim();                  // 去掉首位空格
        Serial.println(TcpClient_Buff);         // 打印接收到的消息
        String getTopic="";
        String getMsg="";
        if (TcpClient_Buff.length() > 15)
        {   
            // 注意TcpClient_Buff只是个字符串，在上面开头做了初始化 String TcpClient_Buff = "";
            // 此时会收到推送的指令，指令大概为 cmd=2&uid=xxx&topic=light002&msg=off
            int topicIndex=TcpClient_Buff.indexOf("&topic=")+7;         // 查找&topic=位置，并移动7位
            int msgIndex=TcpClient_Buff.indexOf("&msg=");               // 查找&msg=位置
            getTopic=TcpClient_Buff.substring(topicIndex, msgIndex);    // 截取到topic
            getMsg=TcpClient_Buff.substring(msgIndex + 5);              // 截取到消息
            Serial.print("topic:------");
            Serial.println(getTopic);                                   // 打印截取到的主题值
            Serial.print("msg:--------");
            Serial.println(getMsg);                                     // 打印截取到的消息值
        }
        if(getMsg.length()==72)
        {
            face_update_by_string(getMsg,leds);
        }
        else if(getMsg.length()==16)
        {
            play_voice_face(leds,getMsg);
        }
        TcpClient_Buff = "";
        TcpClient_BuffIndex = 0;
    }
}

void startSTA()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(DEFAULT_STASSID,DEFAULT_STAPSW);
}

void doWiFiTick()
{
    static bool startSTAFlag = false;
    static bool taskStarted = false;
    static uint32_t lastWiFiCheckTick = 0;

    if(!startSTAFlag)
    {
        startSTAFlag = true;
        startSTA();
    }

    // 未连接1s重连
    if(WiFi.status() != WL_CONNECTED)
    {
        if (millis() - lastWiFiCheckTick > 1000)
        {
            lastWiFiCheckTick = millis();
        }
    }
    // 连接成功建立
    else
    {
        if(taskStarted == false)
        {
            taskStarted = true;
            Serial.print("\r\nGet IP Address: ");
            Serial.println(WiFi.localIP());
            startTCPClient();
        }
    }
}

void turnOnLed()
{
    Serial.println("Turn ON");
    digitalWrite(LED_BUILTIN,LOW);
}

void turnOffLed()
{
    Serial.println("Turn OFF");
    digitalWrite(LED_BUILTIN,HIGH);
}

void decodeHexString(const String hexString,int cells[16][18]) 
{
    String binaryString;
    binaryString.reserve(hexString.length() * 4);

    for(char hexDigit:hexString) 
    {
        int value = (std::isdigit(hexDigit)) ? hexDigit - '0' : std::toupper(hexDigit) - 'A' + 10;

        for (int bit=3;bit>=0;bit--) 
        {
            binaryString+=(value&(1<<bit)) ? '1' : '0';
        }
    }

    for(size_t i=0;i<binaryString.length();i++) 
    {
        int row=i/18;
        int col=i%18;
        cells[row][col]=binaryString[i] == '1' ? 1 : 0;
    }
}