#include "led.h"

#include <windows.h>
#include <vector>

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

void LED::setColor(const std::string &color)
{
    this->color = color;
    const std::string style=std::format("background-color:#{};", color);
    this->label->setStyleSheet(QString::fromStdString(style));
    this->label->repaint();
    //qDebug()<<this->label->styleSheet();
}

void init_led(std::vector<LED> &leds, const std::string &color)
{
    face_update_by_string("0000000007def810a205ef810a205ef800000000117a26505155884d25117a2000000000", leds, color);
    Sleep(1000);
    face_update_by_string("000000000753b91488753b854887723800000000110ec4422910ec44229dcea000000000", leds, color);
    Sleep(1000);
    face_update_by_string("0000000001e3e044201e0804820113e00000000004420191405488133e04488000000000", leds, color);
    Sleep(1000);
    face_update_by_string("00000000000000c00c30030c00c30030000000000000003f000840012000300000000000", leds, color);
}

void decodeHexString(const std::string &hexString, int cells[16][18])
{
    std::string binaryString;
    binaryString.reserve(hexString.length() * 4);

    for (const char hexDigit: hexString)
    {
        const int value = (std::isdigit(hexDigit)) ? hexDigit - '0' : std::toupper(hexDigit) - 'A' + 10;

        for (int bit = 3; bit >= 0; bit--) { binaryString += (value & (1 << bit)) ? '1' : '0'; }
    }

    for (size_t i = 0; i < binaryString.length(); i++)
    {
        const size_t row = i / 18;
        const size_t col = i % 18;
        cells[row][col] = binaryString[i] == '1' ? 1 : 0;
    }
}

std::string encodeBright(const int bright)
{
    const std::string hex[16]={"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
    std::string s=hex[bright/100]+hex[bright%100/10]+hex[bright%10];
    return s;
}

void updateColor(const std::string &color_code, std::vector<LED> leds)
{
    for (int i = 0; i < NUM_LEDS; i++) { if (leds[i].isOn()) { leds[i].setColor(color_code); } }
}

void face_update(int face[16][18],std::vector<LED> &leds, const std::string &color)
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 18; j++)
        {
            if (led_map[i][j] < 0) continue;
            leds[led_map[i][j]].setColor(face[i][j] ? color : LED_OFF);
        }
    }
}

void face_update_by_string(const std::string &hexString, std::vector<LED> &leds, const std::string &color)
{
    int face[16][18];
    decodeHexString(hexString, face);
    face_update(face, leds, color);
}

std::string int2Hex(const int val)
{
    std::stringstream ss;
    ss << std::hex << val;
    return ss.str();
}

std::string get_face(const std::vector<LED> &leds)
{
    int face[16][18] = {0};

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 18; j++)
        {
            if (led_map[i][j] == -1) continue;
            face[i][j] = !leds[led_map[i][j]].isOn() ? 0 : 1;
        }
    }

    std::string binaryString;
    for (auto &i: face) { for (const int j: i) { binaryString += j ? '1' : '0'; } }

    std::string hexString;
    for (size_t i = 0; i < binaryString.length(); i += 4)
    {
        int value = 0;
        for (int j = 0; j < 4; j++) { value = value << 1 | binaryString[i + j] - '0'; }
        hexString += int2Hex(value);
    }

    return hexString;
}
