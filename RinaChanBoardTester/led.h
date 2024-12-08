#ifndef LED_H
#define LED_H

#define NUM_LEDS 270
#define LED_OFF "ffffff"
#define DEFAULT_COLOR "f971d4"
#define EMPTY_FACE "000000000000000000000000000000000000000000000000000000000000000000000000"

#include <QLabel>
#include <utility>
#include <vector>

class LED
{
public:
    explicit LED(QLabel* label) : color(LED_OFF), label(label) {}
    explicit LED(std::string color) : color(std::move(color)), label(nullptr) {}
    ~LED() = default;
    std::string color;
    void setColor(const std::string &color);
    [[nodiscard]] bool isOn() const { return color != LED_OFF; }
private:
    QLabel* label;
};

/**
 * @brief led灯初始化
 *
 * @param leds
 * @param color
 */
void initLED(std::vector<LED> &leds, const LED &color);

/**
 * @brief 表情Hex字节流解码
 *
 * @param[in]   hexBytes  Hex字节流
 * @param[out]  cells     解码后的表情矩阵
 * @param[in]   length    字节流长度

 */
void decodeFaceHex(const char hexBytes[],
                   const quint8 offsetRows,
                   std::vector<std::vector<quint8>> &cells,
                   size_t length);

/**
 * @brief 通过RGB值直接更新led颜色
 *
 * @param leds
 * @param R
 * @param G
 * @param B
 */
void updateColor(std::vector<LED>& leds,
                 const quint8 &R,
                 const quint8 &G,
                 const quint8 &B);

/**
 * @brief 从数组更新璃奈板表情
 *
 * @param[in]  face 16*18的数组
 * @param[out] leds 目标led
 * @param[in]  color 目标颜色
 */
void faceUpdate_FullPack(const std::vector<std::vector<quint8>> &face,
                         std::vector<LED> &leds,
                         const LED& color);

/**
 * @brief
 *
 * @param[in]   lEyeCode   左眼表情编号
 * @param[in]   rEyeCode   右眼表情编号
 * @param[in]   mouthCode  嘴巴表情编号
 * @param[in]   cheekCode  脸颊表情编号
 * @param[out]  leds       目标led矩阵
 * @param[in]   color      目标颜色
 */
void faceUpdate_litePackage(quint8 lEyeCode,
                            quint8 rEyeCode,
                            quint8 mouthCode,
                            quint8 cheekCode,
                            std::vector<LED> &leds,
                            const LED &color);

/**
 * @brief 从状态字符串更新璃奈板表情
 * @warning 注意：不要用于UDP传输
 * @param[in]   hexString   72位长的十六进制状态字符串,可以被译码位16*18的数组
 * @param[out]  leds        目标led
 * @param[in]   color       目标颜色
 */
void faceUpdate_StringFullPack(const std::string &hexString,
                               std::vector<LED> &leds,
                               const LED &color);

/**
 * @brief 从led开闭状态获取状态字符串
 *
 * @param leds 源led
 * @param hexData
 * @return String 状态字符串
 */
void getFaceHex(const std::vector<LED> &leds, std::vector<quint8> &hexData);

#endif //LED_H
