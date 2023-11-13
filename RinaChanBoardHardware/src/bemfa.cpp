#include <Arduino.h>
#include <FastLED.h>

#include <ESP8266WiFi.h>
#include <map>

#include <voice.h>
#include <music.h>
#include <face.h>
#include <bemfa.h>

// tcp客户端相关初始化
WiFiClient TCPclient;
String TcpClient_Buff= "";                 // 初始化字符串,用于接收服务器发来的数据
unsigned int TcpClient_BuffIndex=0;
unsigned long TcpClient_preTick=0;
unsigned long preHeartTick=0;             // 心跳
unsigned long preTCPStartTick=0;          // 连接
bool preTCPConnected=false;

unsigned int R=249;
unsigned int G=113;
unsigned int B=212;
uint8_t bright=64;

void updateColor(String color_code,CRGB leds[])
{
    unsigned int new_R=0,new_G=0,new_B=0;
    for(unsigned int i=0;i<color_code.length();i++)
    {
        int value=(std::isdigit(color_code[i])) ? color_code[i]-'0' : std::toupper(color_code[i])-'A'+10;
        switch(i)
        {
            case 0: continue;        break;
            case 1: new_R+=16*value; break;
            case 2: new_R+=value;    break;
            case 3: new_G+=16*value; break;
            case 4: new_G+=value;    break;
            case 5: new_B+=16*value; break;
            case 6: new_B+=value;    break;
            default: continue;       break;
        }
    }
    R=new_R; G=new_G; B=new_B;
    for(int i=0;i<NUM_LEDS;i++)
    {
        leds[i]=leds[i]==CRGB::Black ? CRGB::Black : CRGB(R,G,B);
    }
    FastLED.show();
}

void sendColor()
{
    String s="#";
    String hex[16]={"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
    s+=hex[R/16]+hex[R%16]+hex[G/16]+hex[G%16]+hex[B/16]+hex[B%16];
    String tcpTemp="cmd=2&uid=";
    tcpTemp+=UID;
    tcpTemp+="&topic=";
    tcpTemp+=TOPIC;
    tcpTemp+="&msg=";
    tcpTemp+=s;
    tcpTemp+="\r\n";
    sendtoTCPServer(tcpTemp);
}

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
            // 此时会收到推送的指令，指令大概为 cmd=2&uid=xxx&topic=xxx&msg=xxx
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
            // 自定义表情消息
            face_update_by_string(getMsg,leds,CRGB(R,G,B));
        }
        else if(getMsg=="requestFace")
        {
            send_face(get_face(leds));
        }
        else if(getMsg=="requestColor")
        {
            sendColor();
        }
        else if(getMsg=="requestBright")
        {
            String hex[16]={"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
            String s=hex[bright/100]+hex[bright%100/10]+hex[bright%10];
            String tcpTemp="cmd=2&uid=";
            tcpTemp+=UID; 
            tcpTemp+="&topic=";
            tcpTemp+=TOPIC;
            tcpTemp+="&msg=";
            tcpTemp+=s;
            tcpTemp+="\r\n";
            sendtoTCPServer(tcpTemp);
        }
        else if(getMsg[0]=='B')
        {
            bright=(getMsg[1]-'0')*100+(getMsg[2]-'0')*10+(getMsg[3]-'0');
            FastLED.setBrightness(bright);
            FastLED.show();
        }
        else if(getMsg.length()==7)
        {
            updateColor(getMsg,leds);
            FastLED.show();
        }
        else if(getMsg.length()==16)
        {
            // 语音播放消息
            play_voice_face(leds,CRGB(R,G,B),getMsg);
        }
        else if(getMsg.length()==19)
        {
            // 歌曲播放消息
            play_music_face(leds,CRGB(R,G,B),getMsg);
        }
        TcpClient_Buff="";
        TcpClient_BuffIndex=0;
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

void decodeHexString(const String hexString,int cells[16][18]) 
{
    String binaryString;
    binaryString.reserve(hexString.length() * 4);

    for(char hexDigit:hexString) 
    {
        int value=(std::isdigit(hexDigit)) ? hexDigit-'0' : std::toupper(hexDigit)-'A'+10;

        for(int bit=3;bit>=0;bit--) 
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

void send_face(String s)
{
    String tcpTemp="cmd=2&uid=";
    tcpTemp+=UID; 
    tcpTemp+="&topic=";
    tcpTemp+=TOPIC;
    tcpTemp+="&msg=";
    tcpTemp+=s;
    tcpTemp+="\r\n";
    sendtoTCPServer(tcpTemp);
}