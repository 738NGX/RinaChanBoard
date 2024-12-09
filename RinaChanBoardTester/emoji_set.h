#pragma once
/*-------------------------------------------
This file is auto generated by gen_emojiSet_to_cpp_Class.py,
DO NOT MODIFY IT MANUALLY.
Generated on 2024-12-07 15:16:54
-------------------------------------------*/
#include <qtypes.h>
#include <cstdint>
#include <array>

class emoji
{
public:
    quint16 id;
    std::array<std::array<quint8, 8>, 8> content;
};

class emojiSet
{
public:
    static constexpr quint8 MAX_LEYE_COUNT  = 27;
    static constexpr quint8 MAX_REYE_COUNT  = 27;
    static constexpr quint8 MAX_MOUTH_COUNT = 32;
    static constexpr quint8 MAX_CHEEK_COUNT = 5;

    static constexpr quint8 L_EYE_START_ROW = 0;
    static constexpr quint8 L_EYE_START_COL = 0;
    static constexpr quint8 R_EYE_START_ROW = 0;
    static constexpr quint8 R_EYE_START_COL = 10;
    static constexpr quint8 EYE_HEIGHT_AND_WIDTH = 8;

    static constexpr quint8 MOUTH_START_ROW        = 8;
    static constexpr quint8 MOUTH_START_COL        = 5;
    static constexpr quint8 MOUTH_HEIGHT_AND_WIDTH = 8;

    static constexpr quint8 L_CHEEK_START_ROW = 8;
    static constexpr quint8 L_CHEEK_START_COL = 0;
    static constexpr quint8 R_CHEEK_START_ROW = 8;
    static constexpr quint8 R_CHEEK_START_COL = 14;
    static constexpr quint8 CHEEK_HEIGHT_AND_WIDTH = 4;

    emoji LEye[MAX_LEYE_COUNT + 1]{};
    emoji REye[MAX_REYE_COUNT + 1]{};
    emoji Mouth[MAX_MOUTH_COUNT + 1]{};
    emoji Cheek[MAX_CHEEK_COUNT + 1]{};
    emoji EmptyPart{};
    emojiSet();
};