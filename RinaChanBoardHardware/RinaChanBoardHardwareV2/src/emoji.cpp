#include <cstdint>
#include <array>

class emoji
{
public:
    uint8_t id;
    std::array<std::array<uint8_t, 8>, 8> facePart;

private:
};

class emojiSet
{
public:
    emoji LEye[27 + 1];
    emoji REye[27 + 1];
    emoji Mouth[32 + 1];
    emoji Face[5 + 1];
    emojiSet();
};

emojiSet::emojiSet()
{
    LEye[1].id       = 1;
    LEye[1].facePart = {{{0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0}}};
    LEye[2].id       = 2;
    LEye[2].facePart = {{{0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 1, 0, 0, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0, 0, 0},
                         {0, 0, 1, 1, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0}}};
}