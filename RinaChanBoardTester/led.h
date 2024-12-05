#ifndef LED_H
#define LED_H

#define NUM_LEDS 270
#define LED_OFF "ffffff"
#define DEFAULT_COLOR "f971d4"
#define EMPTY_FACE "000000000000000000000000000000000000000000000000000000000000000000000000"

#include <QLabel>
#include <vector>

class LED
{
public:
    explicit LED(QLabel* label) : label(label) {}
    ~LED() = default;
    void setColor(const std::string& color);
    [[nodiscard]] bool isOn() const { return color != LED_OFF; }
private:
    QLabel* label;
    std::string color = LED_OFF;
};

void init_led(std::vector<LED>& leds,const std::string& color);

void decodeHexString(const std::string& hexString,int cells[16][18]);

std::string encodeBright(int bright);

void updateColor(const std::string& color_code,std::vector<LED> leds);

void face_update(int face[16][18],std::vector<LED> &leds,const std::string& color);

void face_update_by_string(const std::string &hexString, std::vector<LED> &leds, const std::string &color);

std::string get_face(const std::vector<LED> &leds);

#endif //LED_H
