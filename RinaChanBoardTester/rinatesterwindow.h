//
// Created by jny20 on 24-11-4.
//

#ifndef RINATESTERWINDOW_H
#define RINATESTERWINDOW_H

#include <QUdpSocket>
#include <ui_RinaTesterWindow.h>
#include <vector>

#include "led.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RinaTesterWindow; }
QT_END_NAMESPACE



// ReSharper disable once CppClassCanBeFinal
class RinaTesterWindow : public QWidget {
Q_OBJECT

public:
    explicit RinaTesterWindow(QWidget *parent = nullptr);
    ~RinaTesterWindow() override;

private slots:
    void on_power_btn_clicked();
    void on_checkBox_output_clicked();

private:
    void connectLEDs();
    void processPendingDatagrams();
    void sendCallBack(const std::string& response, const QHostAddress& sender, quint16 senderPort) const;

    Ui::RinaTesterWindow *ui;
    std::vector<LED> leds{};
    std::string color = DEFAULT_COLOR;
    int bright = 16;
    bool power_status = false;
    bool enable_output = false;
    QUdpSocket* udpSocket = nullptr;
};


#endif //RINATESTERWINDOW_H
