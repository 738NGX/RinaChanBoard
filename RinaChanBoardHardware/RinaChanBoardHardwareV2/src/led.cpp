#include <Arduino.h>
#include <FastLED.h>

#include <cstdint>
#include <led.h>
#include <cassert>

#include "emoji_set.hpp"

#define LED_MAX_ROW          16
#define LED_MAX_COL          18
#define FACE_HEX_DATA_LENGTH 36

emojiSet rina;

const int led_map[][LED_MAX_COL] = {
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

void initLED(CRGB leds[], CRGB color)
{
    FastLED.setBrightness(16);
    faceUpdate_StringFullPack("0000000007def810a205ef810a205ef800000000117a26505155884d25117a2000000000",
                              leds,
                              color);
    FastLED.show();
    delay(1000);
    faceUpdate_StringFullPack("000000000753b91488753b854887723800000000110ec4422910ec44229dcea000000000",
                              leds,
                              color);
    FastLED.show();
    delay(1000);
    faceUpdate_StringFullPack("0000000001e3e044201e0804820113e00000000004420191405488133e04488000000000",
                              leds,
                              color);
    FastLED.show();
    delay(1000);
    faceUpdate_StringFullPack("00000000000000c00c30030c00c30030000000000000003f000840012000300000000000",
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

void decodeFaceHex(const char hexBytes[], uint8_t (&cells)[LED_MAX_ROW][LED_MAX_COL], size_t length)
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
static void decodeHexString(const String hexString, uint8_t (&cells)[LED_MAX_ROW][LED_MAX_COL])
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
        int row         = i / LED_MAX_COL;
        int col         = i % LED_MAX_COL;
        cells[row][col] = binaryString[i] == '1' ? 1 : 0;
    }
}
void faceUpdate_StringFullPack(const String hexString, CRGB leds[], CRGB color)
{
    uint8_t face[LED_MAX_ROW][LED_MAX_COL];
    decodeHexString(hexString, face);
    faceUpdate_FullPack(face, leds, color);
    FastLED.show();
}

void faceUpdate_FullPack(uint8_t face[LED_MAX_ROW][LED_MAX_COL], CRGB leds[], CRGB color)
{
    for (int i = 0; i < LED_MAX_ROW; i++)
    {
        for (int j = 0; j < LED_MAX_COL; j++)
        {
            if (led_map[i][j] < 0) continue;
            leds[led_map[i][j]] = face[i][j] ? color : CRGB::Black;
        }
    }
}

static void updateLedMatrixByEmoji(const std::array<std::array<uint8_t, 8>, 8> &emoji,
                                   const uint8_t startRow,
                                   const uint8_t startCol,
                                   const uint8_t height,
                                   const uint8_t width,
                                   CRGB leds[],
                                   const CRGB color)
{

    for (uint8_t i = 0; i < height; i++)
    {
        for (uint8_t j = 0; j < width; j++)
        {
            if (led_map[i + startRow][j + startCol] < 0) continue;
            leds[led_map[i + startRow][j + startCol]] = emoji[i][j] ? color : CRGB::Black;
        }
    }
}
static void updateLedMatrixByEmoji_XFlip(const std::array<std::array<uint8_t, 8>, 8> &emoji,
                                         const uint8_t startRow,
                                         const uint8_t startCol,
                                         const uint8_t height,
                                         const uint8_t width,
                                         CRGB leds[],
                                         const CRGB color)
{

    for (uint8_t i = 0; i < height; i++)
    {
        for (uint8_t j = 0; j < width; j++)
        {
            if (led_map[i + startRow][j + startCol] < 0) continue;
            leds[led_map[i + startRow][j + startCol]] = emoji[i][width - j - 1] ? color : CRGB::Black;
        }
    }
}

void faceUpdate_litePackage(uint8_t const LEyeCode,
                            uint8_t const REyeCode,
                            uint8_t const MouthCode,
                            uint8_t const cheekCode,
                            CRGB leds[],
                            CRGB color)
{
    memset(leds, 0, NUM_LEDS * sizeof(CRGB));
    updateLedMatrixByEmoji(rina.LEye[LEyeCode].content,
                           rina.L_EYE_START_ROW,
                           rina.L_EYE_START_COL,
                           rina.EYE_HEIGHT_AND_WIDTH,
                           rina.EYE_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji(rina.REye[REyeCode].content,
                           rina.R_EYE_START_ROW,
                           rina.R_EYE_START_COL,
                           rina.EYE_HEIGHT_AND_WIDTH,
                           rina.EYE_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji(rina.Mouth[MouthCode].content,
                           rina.MOUTH_START_ROW,
                           rina.MOUTH_START_COL,
                           rina.MOUTH_HEIGHT_AND_WIDTH,
                           rina.MOUTH_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji(rina.Cheek[cheekCode].content,
                           rina.R_CHEEK_START_ROW,
                           rina.R_CHEEK_START_COL,
                           rina.CHEEK_HEIGHT_AND_WIDTH,
                           rina.CHEEK_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji_XFlip(rina.Cheek[cheekCode].content,
                                 rina.L_CHEEK_START_ROW,
                                 rina.L_CHEEK_START_COL,
                                 rina.CHEEK_HEIGHT_AND_WIDTH,
                                 rina.CHEEK_HEIGHT_AND_WIDTH,
                                 leds,
                                 color);
    FastLED.show();
}

void getFaceHex(CRGB leds[], uint8_t *hexData)
{
    int bitIndex = 0; // 位索引

    uint8_t face[LED_MAX_ROW][LED_MAX_COL] = {0};
    memset(hexData, 0, FACE_HEX_DATA_LENGTH);

    for (int i = 0; i < LED_MAX_ROW; i++)
    {
        for (int j = 0; j < LED_MAX_COL; j++)
        {
            if ((leds[led_map[i][j]] != CRGB::Black))
                hexData[bitIndex / 8] |= (1 << (7 - bitIndex % 8));
            bitIndex++;
        }
    }
}
