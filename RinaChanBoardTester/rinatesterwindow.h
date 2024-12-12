//
// Created by jny20 on 24-11-4.
//

#ifndef RINATESTERWINDOW_H
#define RINATESTERWINDOW_H

#include <QUdpSocket>
#include <ui_RinaTesterWindow.h>
#include <vector>

#include "led.h"

#define UDP_INCOME_PACKET_MAXLEN 255
#define LOCAL_UDP_PORT  1234
#define REMOTE_UDP_PORT 4321

QT_BEGIN_NAMESPACE
namespace Ui { class RinaTesterWindow; }
QT_END_NAMESPACE

// ReSharper disable once CppClassCanBeFinal
class RinaTesterWindow : public QWidget {
Q_OBJECT

public:
    enum class PackTypeLen : quint8 {
        FACE_FULL = 36,
        FACE_TEXT_LITE = 16,
        FACE_LITE = 4,
        COLOR     = 3,
        REQUEST   = 2,
        BRIGHT    = 1,
    };
    enum class RequestType : quint16 {
        FACE    = 0x1001,
        COLOR   = 0x1002,
        BRIGHT  = 0x1003,
        VERSION = 0x1004,
        BATTERY  = 0x1005,
    };
    explicit RinaTesterWindow(QWidget *parent = nullptr);
    ~RinaTesterWindow() override;

private slots:
    void on_power_btn_clicked();
    void on_checkBox_output_clicked();

private:
    void connectLEDs();
    void handlePacket();
    void handleRequest(const QHostAddress& sender, const QByteArray &incomingPacket);
    void sendCallBack(const QHostAddress& sender, const QByteArray& responseData) const;
    void sendCallBack(const QHostAddress& sender, const char *buffer) const;
    void sendCallBack(const QHostAddress& sender, const std::string& str) const;
    void sendCallBack(const QHostAddress& sender, int value, quint8 length) const;
    void sendCallBack(const QHostAddress& sender, const quint8 *hexBuffer, quint8 length) const;

    Ui::RinaTesterWindow *ui;
    std::vector<LED> leds{};
    quint8 R      = 249;
    quint8 G      = 113;
    quint8 B      = 212;
    quint8 bright = 16;
    PackTypeLen lastFacePackType = PackTypeLen::FACE_FULL;
    const quint16 localUDPPort  = LOCAL_UDP_PORT;
    const quint16 remoteUDPPort = REMOTE_UDP_PORT;
    std::vector<std::vector<quint8>> faceBuf = std::vector(16, std::vector<quint8>(18, 0));
    std::vector<quint8> faceHexBuffer = std::vector<quint8>(36, 0);
    bool power_status = false;
    bool enable_output = false;
    QUdpSocket* udpSocket = nullptr;
};


#endif //RINATESTERWINDOW_H
