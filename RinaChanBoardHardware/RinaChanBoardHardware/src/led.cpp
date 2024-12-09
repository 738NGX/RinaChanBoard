#include <cstdint>

#include "led.h"
#include "emoji_set.hpp"

#define FACE_HEX_DATA_LENGTH 36

emojiSet rina;

const int led_map[LED_MAX_ROW][LED_MAX_COL] = {
    {-1, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1},
    {-1, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, -1},
    {-1, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, -1},
    {-1, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, -1},
    {-1, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, -1},
    {-1, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, -1},
    {-1, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, -1},
    {-1, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, -1},
    {-1, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, -1},
    {-1, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, -1},
    {-1, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, -1},
    {-1, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, -1},
    {-1, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, -1},
    {-1, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, -1},
    {-1, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, -1},
    {-1, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, -1}
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

void decodeFaceHex(const char hexBytes[],
                   const uint8_t offsetRows,
                   uint8_t (&cells)[LED_MAX_ROW][LED_MAX_COL],
                   size_t length)
{
    size_t bitIndex = 0; // 用于定位当前写入到 cells 的 bit 位置
    for (size_t i = 0; i < length; i++)
    {
        uint8_t byte = hexBytes[i]; // 获取当前字节
        for (int bit = 7; bit >= 0; bit--)
        {
            int row = offsetRows + (bitIndex / LED_MAX_COL); // 确定行
            int col = bitIndex % LED_MAX_COL;                // 确定列
            if (row >= LED_MAX_ROW)                          // 防止越界
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
static void decodeHexString(const String hexString,
                            uint8_t (&cells)[LED_MAX_ROW][LED_MAX_COL])
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
        int row = i / LED_MAX_COL;
        int col = i % LED_MAX_COL;
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
            if (led_map[i][j] == -1)
                continue;
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
            if (led_map[i + startRow][j + startCol] < 0)
                continue;
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
            if (led_map[i + startRow][j + startCol] < 0)
                continue;

            leds[led_map[i + startRow][j + startCol]] = emoji[i][width - j - 1] ? color : CRGB::Black;
        }
    }
}

void faceUpdate_litePackage(uint8_t const LEyeCode,
                            uint8_t const REyeCode,
                            uint8_t const MouthCode,
                            uint8_t const CheekCode,
                            CRGB leds[],
                            CRGB color)
{
    if (LEyeCode >= rina.MAX_LEYE_COUNT ||
        REyeCode >= rina.MAX_REYE_COUNT ||
        MouthCode >= rina.MAX_MOUTH_COUNT ||
        CheekCode >= rina.MAX_CHEEK_COUNT)
        return;
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
    updateLedMatrixByEmoji(rina.Cheek[CheekCode].content,
                           rina.R_CHEEK_START_ROW,
                           rina.R_CHEEK_START_COL,
                           rina.CHEEK_HEIGHT_AND_WIDTH,
                           rina.CHEEK_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji_XFlip(rina.Cheek[CheekCode].content,
                                 rina.L_CHEEK_START_ROW,
                                 rina.L_CHEEK_START_COL,
                                 rina.CHEEK_HEIGHT_AND_WIDTH,
                                 rina.CHEEK_HEIGHT_AND_WIDTH,
                                 leds,
                                 color);
}

void getFaceHex(CRGB leds[], uint8_t *hexData)
{
    uint16_t bitIndex = 0;
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
