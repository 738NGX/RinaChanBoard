#include <Arduino.h>
#include <FastLED.h>

#include <cstdint>
#include <led.h>
#define LED_MAX_ROW          16
#define LED_MAX_COL          18
#define FACE_HEX_DATA_LENGTH 36

const int led_map[][18] = {
    {-1, -1, 38, 39, 70, 71, 102, 103, 134, 135, 166, 167, 198, 199, 230, 231, -1, -1},
    {-1, 10, 37, 40, 69, 72, 101, 104, 133, 136, 165, 168, 197, 200, 229, 232, 259, -1},
    {9, 11, 36, 41, 68, 73, 100, 105, 132, 137, 164, 169, 196, 201, 228, 233, 258, 260},
    {8, 12, 35, 42, 67, 74, 99, 106, 131, 138, 163, 170, 195, 202, 227, 234, 257, 261},
    {7, 13, 34, 43, 66, 75, 98, 107, 130, 139, 162, 171, 194, 203, 226, 235, 256, 262},
    {6, 14, 33, 44, 65, 76, 97, 108, 129, 140, 161, 172, 193, 204, 225, 236, 255, 263},
    {5, 15, 32, 45, 64, 77, 96, 109, 128, 141, 160, 173, 192, 205, 224, 237, 254, 264},
    {4, 16, 31, 46, 63, 78, 95, 110, 127, 142, 159, 174, 191, 206, 223, 238, 253, 265},
    {3, 17, 30, 47, 62, 79, 94, 111, 126, 143, 158, 175, 190, 207, 222, 239, 252, 266},
    {2, 18, 29, 48, 61, 80, 93, 112, 125, 144, 157, 176, 189, 208, 221, 240, 251, 267},
    {1, 19, 28, 49, 60, 81, 92, 113, 124, 145, 156, 177, 188, 209, 220, 241, 250, 268},
    {0, 20, 27, 50, 59, 82, 91, 114, 123, 146, 155, 178, 187, 210, 219, 242, 249, 269},
    {-1, 21, 26, 51, 58, 83, 90, 115, 122, 147, 154, 179, 186, 211, 218, 243, 248, -1},
    {-1, 22, 25, 52, 57, 84, 89, 116, 121, 148, 153, 180, 185, 212, 217, 244, 247, -1},
    {-1, 23, 24, 53, 56, 85, 88, 117, 120, 149, 152, 181, 184, 213, 216, 245, 246, -1},
    {-1, -1, -1, 54, 55, 86, 87, 118, 119, 150, 151, 182, 183, 214, 215, -1, -1, -1},
};

void init_led(CRGB leds[], CRGB color)
{
    FastLED.setBrightness(16);
    face_update_by_string("0000000007def810a205ef810a205ef800000000117a26505155884d25117a2000000000",
                          leds,
                          color);
    FastLED.show();
    delay(1000);
    face_update_by_string("000000000753b91488753b854887723800000000110ec4422910ec44229dcea000000000",
                          leds,
                          color);
    FastLED.show();
    delay(1000);
    face_update_by_string("0000000001e3e044201e0804820113e00000000004420191405488133e04488000000000",
                          leds,
                          color);
    FastLED.show();
    delay(1000);
    face_update_by_string("00000000000000c00c30030c00c30030000000000000003f000840012000300000000000",
                          leds,
                          color);
    FastLED.show();
}

void updateColor(CRGB leds[], const uint8_t &R, const uint8_t &G, const uint8_t &B)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = ((leds[i] == CRGB::Black) ? CRGB::Black : CRGB(R, G, B));
    }
    FastLED.show();
}

void decodeFaceHex(const char hexBytes[], int (&cells)[16][18], size_t length)
{
    size_t bitIndex = 0; // 用于定位当前写入到 cells 的 bit 位置
    for (size_t i = 0; i < length; i++)
    {
        uint8_t byte = hexBytes[i]; // 获取当前字节

        // 遍历该字节的 8 位二进制
        for (int bit = 7; bit >= 0; bit--)
        {
            int row = bitIndex / LED_MAX_COL; // 确定行
            int col = bitIndex % LED_MAX_COL; // 确定列
            if (row >= LED_MAX_ROW)           // 防止越界
                return;

            cells[row][col] = (byte & (1 << bit)) ? 1 : 0; // 提取二进制位并存入 cells
            bitIndex++;
        }
    }
}

/**
 * @brief 内部函数，用于解码HexString到表情矩阵，请不要直接调用
 *
 * @param hexString
 * @param cells
 */
static void decodeHexString(const String hexString, int cells[16][18])
{
    String binaryString;
    binaryString.reserve(hexString.length() * 4);

    for (char hexDigit : hexString)
    {
        int value = (std::isdigit(hexDigit)) ? hexDigit - '0' : std::toupper(hexDigit) - 'A' + 10;

        for (int bit = 3; bit >= 0; bit--)
        {
            binaryString += (value & (1 << bit)) ? '1' : '0';
        }
    }

    for (size_t i = 0; i < binaryString.length(); i++)
    {
        int row         = i / 18;
        int col         = i % 18;
        cells[row][col] = binaryString[i] == '1' ? 1 : 0;
    }
}
void face_update_by_string(const String hexString, CRGB leds[], CRGB color)
{
    int face[16][18];
    decodeHexString(hexString, face);
    face_update(face, leds, color);
    FastLED.show();
}

void face_update(int face[16][18], CRGB leds[], CRGB color)
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 18; j++)
        {
            if (led_map[i][j] < 0) continue;
            leds[led_map[i][j]] = face[i][j] ? color : CRGB::Black;
        }
    }
}

void get_face_hex(CRGB leds[], uint8_t *hexData)
{
    int bitIndex = 0; // 位索引

    uint8_t face[16][18] = {0};
    memset(hexData, 0, FACE_HEX_DATA_LENGTH);

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 18; j++)
        {
            if (led_map[i][j] == -1) continue;
            uint8_t currentBit = (leds[led_map[i][j]] == CRGB::Black) ? 0 : 1; // 判断颜色是否为黑色

            // 计算目标字节数组的下标和位位置
            uint8_t byteIndex = bitIndex / 8;
            uint8_t bitInByte = bitIndex % 8;

            // 设置该字节的相应位
            if (currentBit)
                hexData[byteIndex] |= (1 << (7 - bitInByte)); // 设置该位为 1

            bitIndex++;
        }
    }
}