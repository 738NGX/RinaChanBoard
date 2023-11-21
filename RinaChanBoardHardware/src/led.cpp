#include <Arduino.h>
#include <FastLED.h>

#include <led.h>

void init_led(CRGB leds[],CRGB color)
{
    FastLED.setBrightness(64);
    face_update_by_string("0000000007def810a205ef810a205ef800000000117a26505155884d25117a2000000000",leds,color);
    FastLED.show();
    delay(1000);
    face_update_by_string("0000000001e3e044201e0804820113e00000000004420191405488133e04488000000000",leds,color);
    FastLED.show();
    delay(1000);
    face_update_by_string("00000000000000601818060601818060000000000000003f000840012000300000000000",leds,color);
    FastLED.show();
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

String encodeColor(unsigned R,unsigned int G,unsigned int B)
{
    String s="#";
    String hex[16]={"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
    s+=hex[R/16]+hex[R%16]+hex[G/16]+hex[G%16]+hex[B/16]+hex[B%16];
    return s;
}

String encodeBright(u_int8_t bright)
{
    String hex[16]={"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
    String s=hex[bright/100]+hex[bright%100/10]+hex[bright%10];
    return s;
}

void updateColor(String color_code,CRGB leds[],unsigned int &R,unsigned int &G,unsigned int &B)
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

void face_update(int face[16][18],CRGB leds[],CRGB color)
{
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<16;j++)
        {
            if(i%2) leds[16*i+j]=face[15-i][16-j] ? color : CRGB::Black;
            else leds[16*i+j]= face[15-i][j+1] ? color : CRGB::Black;
        }
    }
}

void face_update_by_string(const String hexString,CRGB leds[],CRGB color)
{
    int face[16][18];
    decodeHexString(hexString,face);
    face_update(face,leds,color);
    FastLED.show();
}

String get_face(CRGB leds[]) {
    int face[16][18]={0};

    for(int i=0;i<16;i++) 
    {
        for(int j=0;j<16;j++)
        {
            if(i%2==0) 
            {
                face[15-i][j+1]=leds[16*i + j]==CRGB::Black ? 0 : 1;
            }
            else
            {
                face[15-i][16-j]=leds[16 * i + j]==CRGB::Black ? 0 : 1;
            }
        }
    }

    String binaryString;
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<18;j++)
        {
            binaryString+=face[i][j] ? '1' : '0';
        }
    }

    String hexString;
    for(size_t i=0;i<binaryString.length();i+=4)
    {
        int value = 0;
        for(int j=0;j<4; j++)
        {
            value = (value << 1) | (binaryString[i + j] - '0');
        }
        hexString += String(value, HEX);
    }

    return hexString;
}