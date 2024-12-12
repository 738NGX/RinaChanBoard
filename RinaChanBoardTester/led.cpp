#include <windows.h>
#include <vector>

#include "led.h"
#include "emoji_set.h"

#define LED_MAX_ROW          16
#define LED_MAX_COL          18
#define FACE_HEX_DATA_LENGTH 36

emojiSet rina;

void LED::setColor(const std::string &color)
{
    this->color = color;
    const std::string style=std::format("background-color:#{};", color);
    this->label->setStyleSheet(QString::fromStdString(style));
    this->label->repaint();
    //qDebug()<<this->label->styleSheet();
}

const int led_map[][18] =
{
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

void initLED(std::vector<LED> &leds, const LED &color)
{
    faceUpdate_StringFullPack("0000000007def810a205ef810a205ef800000000117a26505155884d25117a2000000000",
                              leds,
                              color);
    Sleep(1000);
    faceUpdate_StringFullPack("000000000753b91488753b854887723800000000110ec4422910ec44229dcea000000000",
                              leds,
                              color);
    Sleep(1000);
    faceUpdate_StringFullPack("0000000001e3e044201e0804820113e00000000004420191405488133e04488000000000",
                              leds,
                              color);
    Sleep(1000);
    faceUpdate_StringFullPack("00000000000000c00c30030c00c30030000000000000003f000840012000300000000000",
                              leds,
                              color);
}

void updateColor(std::vector<LED>& leds, const quint8 &R, const quint8 &G, const quint8 &B)
{
    const std::string colorString=(QString::number(R, 16) + QString::number(G, 16) + QString::number(B, 16)).toStdString();
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i].setColor(leds[i].isOn() ? LED_OFF : colorString);
    }
}

void decodeFaceHex(const char hexBytes[],
                   const quint8 offsetRows,
                   std::vector<std::vector<quint8>> &cells,
                   const size_t length)
{
    size_t bitIndex = 0; // 用于定位当前写入到 cells 的 bit 位置
    for (size_t i = 0; i < length; i++)
    {
        const quint8 byte = hexBytes[i]; // 获取当前字节
        for (int bit = 7; bit >= 0; bit--)
        {
            const int row = offsetRows + (bitIndex / LED_MAX_COL);  // 确定行
            const int col = bitIndex % LED_MAX_COL;                 // 确定列
            if (row >= LED_MAX_ROW)                                 // 防止越界
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
static void decodeHexString(const std::string& hexString,
                            std::vector<std::vector<quint8>> &cells)
{
    std::string binaryString;
    binaryString.reserve(hexString.length() * 4);

    for (const char hexDigit : hexString)
    {
        const int value = std::isdigit(hexDigit) ? hexDigit - '0' : std::toupper(hexDigit) - 'A' + 10;

        for (int bit = 3; bit >= 0; bit--)
        {
            binaryString += value & 1 << bit ? '1' : '0';
        }
    }

    for (size_t i = 0; i < binaryString.length(); i++)
    {
        const size_t row = i / LED_MAX_COL;
        const size_t col = i % LED_MAX_COL;
        cells[row][col] = binaryString[i] == '1' ? 1 : 0;
    }
}

void faceUpdate_StringFullPack(const std::string &hexString, std::vector<LED> &leds, const LED &color)
{
    auto face = std::vector(16, std::vector<quint8>(18, 0));
    decodeHexString(hexString, face);
    faceUpdate_FullPack(face, leds, color);
}

void faceUpdate_FullPack(const std::vector<std::vector<quint8>> &face, std::vector<LED> &leds, const LED& color)
{
    for (int i = 0; i < LED_MAX_ROW; i++)
    {
        for (int j = 0; j < LED_MAX_COL; j++)
        {
            if (led_map[i][j] < 0) continue;
            leds[led_map[i][j]].setColor(face[i][j] ? color.color : LED_OFF);
        }
    }
}

static void updateLedMatrixByEmoji(const std::array<std::array<quint8, 8>, 8> &emoji,
                                   const quint8 startRow,
                                   const quint8 startCol,
                                   const quint8 height,
                                   const quint8 width,
                                   std::vector<LED> &leds,
                                   const LED& color)
{
    for (quint8 i = 0; i < height; i++)
    {
        for (quint8 j = 0; j < width; j++)
        {
            if (led_map[i + startRow][j + startCol] < 0) continue;
            leds[led_map[i + startRow][j + startCol]].setColor(emoji[i][j] ? color.color : LED_OFF);
        }
    }
}
static void updateLedMatrixByEmoji_XFlip(const std::array<std::array<quint8, 8>, 8> &emoji,
                                         std::vector<LED> leds,
                                         const LED& color)
{
    for (quint8 i = 0; i < emojiSet::CHEEK_HEIGHT_AND_WIDTH; i++)
    {
        for (quint8 j = 0; j < emojiSet::CHEEK_HEIGHT_AND_WIDTH; j++)
        {
            if (led_map[i + emojiSet::L_CHEEK_START_ROW][j + emojiSet::L_CHEEK_START_COL] < 0) continue;

            leds[led_map[i + emojiSet::L_CHEEK_START_ROW][j + emojiSet::L_CHEEK_START_COL]].setColor(emoji[i][emojiSet::CHEEK_HEIGHT_AND_WIDTH - j - 1] ? color.color : LED_OFF);
        }
    }
}

void faceUpdate_litePackage(quint8 const lEyeCode,
                            quint8 const rEyeCode,
                            quint8 const mouthCode,
                            quint8 const cheekCode,
                            std::vector<LED> &leds,
                            const LED &color)
{
    updateLedMatrixByEmoji(rina.LEye[lEyeCode].content,
                           emojiSet::L_EYE_START_ROW,
                           emojiSet::L_EYE_START_COL,
                           emojiSet::EYE_HEIGHT_AND_WIDTH,
                           emojiSet::EYE_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji(rina.REye[rEyeCode].content,
                           emojiSet::R_EYE_START_ROW,
                           emojiSet::R_EYE_START_COL,
                           emojiSet::EYE_HEIGHT_AND_WIDTH,
                           emojiSet::EYE_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji(rina.Mouth[mouthCode].content,
                           emojiSet::MOUTH_START_ROW,
                           emojiSet::MOUTH_START_COL,
                           emojiSet::MOUTH_HEIGHT_AND_WIDTH,
                           emojiSet::MOUTH_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji(rina.Cheek[cheekCode].content,
                           emojiSet::R_CHEEK_START_ROW,
                           emojiSet::R_CHEEK_START_COL,
                           emojiSet::CHEEK_HEIGHT_AND_WIDTH,
                           emojiSet::CHEEK_HEIGHT_AND_WIDTH,
                           leds,
                           color);
    updateLedMatrixByEmoji_XFlip(rina.Cheek[cheekCode].content,
                                 leds,
                                 color);
}

void getFaceHex(const std::vector<LED> &leds, std::vector<quint8> &hexData)
{
    quint16 bitIndex = 0;
    hexData = std::vector<quint8>(36, 0);

    for (const auto & i : led_map)
    {
        for (const int j : i)
        {
            if (leds[j].color != LED_OFF)
                hexData[bitIndex / 8] |= 1 << 7 - bitIndex % 8; // NOLINT(*-narrowing-conversions)
            bitIndex++;
        }
    }
}

