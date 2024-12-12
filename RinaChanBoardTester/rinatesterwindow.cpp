//
// Created by jny20 on 24-11-4.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RinaTesterWindow.h" resolved

#include "rinatesterwindow.h"
#include "ui_RinaTesterWindow.h"

#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <windows.h>

#define TEXT_CENTER_ALIGN_OFFSET_ROWS 4

RinaTesterWindow::RinaTesterWindow(QWidget *parent) : QWidget(parent), ui(new Ui::RinaTesterWindow)
{
    ui->setupUi(this);
    connectLEDs();
}

RinaTesterWindow::~RinaTesterWindow() { delete ui; }

QHostAddress getLocalIPAddress()
{
    for (const QList<QHostAddress> &allAddresses = QNetworkInterface::allAddresses(); const QHostAddress &address:
         allAddresses)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != QHostAddress(QHostAddress::LocalHost)) { return address; }
    }
    return {QHostAddress::LocalHost};
}

void RinaTesterWindow::on_power_btn_clicked()
{
    if (power_status)
    {
        power_status = false;
        faceUpdate_StringFullPack(EMPTY_FACE, leds, LED(DEFAULT_COLOR));

        if (udpSocket)
        {
            udpSocket->close();
            delete udpSocket;
            udpSocket = nullptr;
            ui->udpaddr_val->setText("UDP服务未启动");
        }
    }
    else
    {
        power_status = true;
        initLED(leds, LED(DEFAULT_COLOR));

        udpSocket = new QUdpSocket(this);
        if (!connect(udpSocket, &QUdpSocket::readyRead, this, &RinaTesterWindow::handlePacket))
        {
            qDebug() << "UDP绑定失败";
            return;
        }

        const QHostAddress address = getLocalIPAddress();
        if (!udpSocket->bind(address, localUDPPort))
        {
            qDebug() << "UDP绑定失败";
            return;
        }
        ui->udpaddr_val->setText(address.toString());
        if (enable_output) { qDebug() << "ip:" << address << ",port:" << localUDPPort << ",等待接收..."; }
    }
}

void RinaTesterWindow::on_checkBox_output_clicked()
{
    if(ui->checkBox_output->isChecked())
    {
        enable_output = true;
    }
    else
    {
        enable_output = false;
    }
}

std::string getColorString(const quint8 &R, const quint8 &G, const quint8 &B)
{
    return (QString::number(R, 16) + QString::number(G, 16) + QString::number(B, 16)).toStdString();
}

void RinaTesterWindow::handlePacket()
{
    constexpr size_t BufMaxSize = UDP_INCOME_PACKET_MAXLEN;
    char buf[BufMaxSize] = {};

    while (udpSocket->hasPendingDatagrams())
    {
        QHostAddress sender;
        quint16 senderPort;

        // ReSharper disable once CppLocalVariableMayBeConst
        // ReSharper disable once CppTooWideScopeInitStatement
        qint64 datagramSize = udpSocket->readDatagram(buf, BufMaxSize, &sender, &senderPort);
        if (datagramSize != -1)
        {
            if (enable_output) { qDebug() << "接收到数据：" << QByteArray(buf, datagramSize).toHex(); }
            switch (const auto incomingPacket = QByteArray(buf, datagramSize); incomingPacket.length()) // 通过长度判断包类型
            {
                case static_cast<quint8>(PackTypeLen::FACE_FULL): {
                    // 从上位机接受全脸状态更新
                    if (lastFacePackType != PackTypeLen::FACE_FULL)
                    {
                        lastFacePackType = PackTypeLen::FACE_FULL;
                    }
                    decodeFaceHex(incomingPacket, 0,
                                  faceBuf, static_cast<quint8>(PackTypeLen::FACE_FULL));
                    faceUpdate_FullPack(faceBuf, this->leds,
                                        LED(getColorString(R, G, B)));
                    break;
                }
                case static_cast<uint8_t>(PackTypeLen::FACE_TEXT_LITE): {
                    // 从上位机接受7行文字的小包更新
                    if (lastFacePackType != PackTypeLen::FACE_TEXT_LITE)
                    {
                        lastFacePackType = PackTypeLen::FACE_TEXT_LITE;
                        faceBuf = std::vector(16, std::vector<quint8>(18, 0));
                    }
                    decodeFaceHex(incomingPacket,
                                  TEXT_CENTER_ALIGN_OFFSET_ROWS,
                                  faceBuf,
                                  static_cast<uint8_t>(PackTypeLen::FACE_TEXT_LITE));
                    faceUpdate_FullPack(faceBuf,
                                        this->leds,
                                        LED(getColorString(R, G, B)));
                    break;
                }
                case static_cast<quint8>(PackTypeLen::FACE_LITE): {
                    // 从上位机接受表情部件状态更新
                    if (lastFacePackType != PackTypeLen::FACE_LITE)
                    {
                        lastFacePackType = PackTypeLen::FACE_LITE;
                        faceUpdate_StringFullPack(EMPTY_FACE, leds, LED(getColorString(R, G, B)));
                    }
                    const quint8 lEyeCode  = incomingPacket[0];
                    const quint8 rEyeCode  = incomingPacket[1];
                    const quint8 mouthCode = incomingPacket[2];
                    const quint8 cheekCode = incomingPacket[3];

                    faceUpdate_litePackage(lEyeCode,
                                           rEyeCode,
                                           mouthCode,
                                           cheekCode,
                                           this->leds,
                                           LED(getColorString(R, G, B)));
                    break;
                }
                case static_cast<quint8>(PackTypeLen::COLOR): {
                    // 从上位机接受颜色更新
                    R = static_cast<quint8>(incomingPacket[0]);
                    G = static_cast<quint8>(incomingPacket[1]);
                    B = static_cast<quint8>(incomingPacket[2]);
                    updateColor(this->leds, R, G, B);
                    ui->color_val->setText(QString::fromStdString("#"+getColorString(R, G, B)));
                    break;
                }
                case static_cast<quint8>(PackTypeLen::REQUEST): {
                    // 从上位机接受请求包
                    handleRequest(sender, incomingPacket);
                    break;
                }
                break;
                case static_cast<quint8>(PackTypeLen::BRIGHT): {
                    // 从上位机接受亮度更新
                    bright = incomingPacket[0];
                    ui->bright_val->setText(QString::fromStdString(std::to_string(bright)));
                    break;
                }
                default:
                    sendCallBack(sender, "Command Error!");
                break;
            }
            memset(buf, 0, sizeof(buf));
        } else { qDebug() << "接收失败或发生错误！"; }
    }
}

void RinaTesterWindow::handleRequest(const QHostAddress& sender, const QByteArray &incomingPacket)
{
    switch (incomingPacket[0] << 8 | incomingPacket[1])
    {
        case static_cast<quint16>(RequestType::FACE): {
            // 发送状态hex字节流到上位机
            getFaceHex(this->leds, faceHexBuffer);
            sendCallBack(sender, QByteArray(faceHexBuffer));
            break;
        }
        case static_cast<quint16>(RequestType::COLOR): {
            // 发送颜色hex字节流到上位机
            sendCallBack(sender,
                         B << 16 | G << 8 | R,
                         static_cast<quint8>(PackTypeLen::COLOR));
            break;
        }
        case static_cast<quint16>(RequestType::BRIGHT): {
            // 发送亮度字节流到上位机
            sendCallBack(sender,
                         bright,
                         static_cast<quint8>(PackTypeLen::BRIGHT));
            break;
        }
        case static_cast<uint16_t>(RequestType::VERSION): {
            sendCallBack(sender, "1.0.0-tester");
            break;
        }
        case static_cast<uint16_t>(RequestType::BATTERY): {
            // TODO:发送ADC数据到上位机
            break;
        }
        default: {
            sendCallBack(sender, "Command Error!");
            break;
        }
    }
}

void RinaTesterWindow::sendCallBack(const QHostAddress& sender, const QByteArray& responseData) const
{
    udpSocket->writeDatagram(responseData, sender, remoteUDPPort);
    if (enable_output)
    {
        qDebug() << "发送回调消息到" << sender.toString() << ":" << remoteUDPPort << " - " << responseData.toHex();
    };
}

void RinaTesterWindow::sendCallBack(const QHostAddress& sender, const char *buffer) const
{
    const auto responseData = QByteArray(buffer, strlen(buffer)); // NOLINT(*-narrowing-conversions)
    sendCallBack(sender, responseData);
}

void RinaTesterWindow::sendCallBack(const QHostAddress& sender, const std::string& str) const
{
    const auto responseData = QByteArray(str.c_str());
    sendCallBack(sender, responseData);
}

void RinaTesterWindow::sendCallBack(const QHostAddress& sender, const int value, const quint8 length) const
{
    // ReSharper disable once CppCStyleCast
    const auto responseData = QByteArray(reinterpret_cast<const char*>(&value), length);
    sendCallBack(sender, responseData);
}

void RinaTesterWindow::sendCallBack(const QHostAddress& sender, const quint8 *hexBuffer, quint8 length) const
{
    // ReSharper disable once CppCStyleCast
    const auto responseData = QByteArray(reinterpret_cast<const char*>(hexBuffer), length);
    sendCallBack(sender, responseData);
}

void RinaTesterWindow::connectLEDs()
{
    leds = {
        LED(ui->label_00), LED(ui->label_01), LED(ui->label_02), LED(ui->label_03), LED(ui->label_04),
        LED(ui->label_05), LED(ui->label_06), LED(ui->label_07), LED(ui->label_08), LED(ui->label_09),
        LED(ui->label_10), LED(ui->label_11), LED(ui->label_12), LED(ui->label_13), LED(ui->label_14),
        LED(ui->label_15), LED(ui->label_16), LED(ui->label_17), LED(ui->label_18), LED(ui->label_19),
        LED(ui->label_20), LED(ui->label_21), LED(ui->label_22), LED(ui->label_23), LED(ui->label_24),
        LED(ui->label_25), LED(ui->label_26), LED(ui->label_27), LED(ui->label_28), LED(ui->label_29),
        LED(ui->label_30), LED(ui->label_31), LED(ui->label_32), LED(ui->label_33), LED(ui->label_34),
        LED(ui->label_35), LED(ui->label_36), LED(ui->label_37), LED(ui->label_38), LED(ui->label_39),
        LED(ui->label_40), LED(ui->label_41), LED(ui->label_42), LED(ui->label_43), LED(ui->label_44),
        LED(ui->label_45), LED(ui->label_46), LED(ui->label_47), LED(ui->label_48), LED(ui->label_49),
        LED(ui->label_50), LED(ui->label_51), LED(ui->label_52), LED(ui->label_53), LED(ui->label_54),
        LED(ui->label_55), LED(ui->label_56), LED(ui->label_57), LED(ui->label_58), LED(ui->label_59),
        LED(ui->label_60), LED(ui->label_61), LED(ui->label_62), LED(ui->label_63), LED(ui->label_64),
        LED(ui->label_65), LED(ui->label_66), LED(ui->label_67), LED(ui->label_68), LED(ui->label_69),
        LED(ui->label_70), LED(ui->label_71), LED(ui->label_72), LED(ui->label_73), LED(ui->label_74),
        LED(ui->label_75), LED(ui->label_76), LED(ui->label_77), LED(ui->label_78), LED(ui->label_79),
        LED(ui->label_80), LED(ui->label_81), LED(ui->label_82), LED(ui->label_83), LED(ui->label_84),
        LED(ui->label_85), LED(ui->label_86), LED(ui->label_87), LED(ui->label_88), LED(ui->label_89),
        LED(ui->label_90), LED(ui->label_91), LED(ui->label_92), LED(ui->label_93), LED(ui->label_94),
        LED(ui->label_95), LED(ui->label_96), LED(ui->label_97), LED(ui->label_98), LED(ui->label_99),
        LED(ui->label_100), LED(ui->label_101), LED(ui->label_102), LED(ui->label_103), LED(ui->label_104),
        LED(ui->label_105), LED(ui->label_106), LED(ui->label_107), LED(ui->label_108), LED(ui->label_109),
        LED(ui->label_110), LED(ui->label_111), LED(ui->label_112), LED(ui->label_113), LED(ui->label_114),
        LED(ui->label_115), LED(ui->label_116), LED(ui->label_117), LED(ui->label_118), LED(ui->label_119),
        LED(ui->label_120), LED(ui->label_121), LED(ui->label_122), LED(ui->label_123), LED(ui->label_124),
        LED(ui->label_125), LED(ui->label_126), LED(ui->label_127), LED(ui->label_128), LED(ui->label_129),
        LED(ui->label_130), LED(ui->label_131), LED(ui->label_132), LED(ui->label_133), LED(ui->label_134),
        LED(ui->label_135), LED(ui->label_136), LED(ui->label_137), LED(ui->label_138), LED(ui->label_139),
        LED(ui->label_140), LED(ui->label_141), LED(ui->label_142), LED(ui->label_143), LED(ui->label_144),
        LED(ui->label_145), LED(ui->label_146), LED(ui->label_147), LED(ui->label_148), LED(ui->label_149),
        LED(ui->label_150), LED(ui->label_151), LED(ui->label_152), LED(ui->label_153), LED(ui->label_154),
        LED(ui->label_155), LED(ui->label_156), LED(ui->label_157), LED(ui->label_158), LED(ui->label_159),
        LED(ui->label_160), LED(ui->label_161), LED(ui->label_162), LED(ui->label_163), LED(ui->label_164),
        LED(ui->label_165), LED(ui->label_166), LED(ui->label_167), LED(ui->label_168), LED(ui->label_169),
        LED(ui->label_170), LED(ui->label_171), LED(ui->label_172), LED(ui->label_173), LED(ui->label_174),
        LED(ui->label_175), LED(ui->label_176), LED(ui->label_177), LED(ui->label_178), LED(ui->label_179),
        LED(ui->label_180), LED(ui->label_181), LED(ui->label_182), LED(ui->label_183), LED(ui->label_184),
        LED(ui->label_185), LED(ui->label_186), LED(ui->label_187), LED(ui->label_188), LED(ui->label_189),
        LED(ui->label_190), LED(ui->label_191), LED(ui->label_192), LED(ui->label_193), LED(ui->label_194),
        LED(ui->label_195), LED(ui->label_196), LED(ui->label_197), LED(ui->label_198), LED(ui->label_199),
        LED(ui->label_200), LED(ui->label_201), LED(ui->label_202), LED(ui->label_203), LED(ui->label_204),
        LED(ui->label_205), LED(ui->label_206), LED(ui->label_207), LED(ui->label_208), LED(ui->label_209),
        LED(ui->label_210), LED(ui->label_211), LED(ui->label_212), LED(ui->label_213), LED(ui->label_214),
        LED(ui->label_215), LED(ui->label_216), LED(ui->label_217), LED(ui->label_218), LED(ui->label_219),
        LED(ui->label_220), LED(ui->label_221), LED(ui->label_222), LED(ui->label_223), LED(ui->label_224),
        LED(ui->label_225), LED(ui->label_226), LED(ui->label_227), LED(ui->label_228), LED(ui->label_229),
        LED(ui->label_230), LED(ui->label_231), LED(ui->label_232), LED(ui->label_233), LED(ui->label_234),
        LED(ui->label_235), LED(ui->label_236), LED(ui->label_237), LED(ui->label_238), LED(ui->label_239),
        LED(ui->label_240), LED(ui->label_241), LED(ui->label_242), LED(ui->label_243), LED(ui->label_244),
        LED(ui->label_245), LED(ui->label_246), LED(ui->label_247), LED(ui->label_248), LED(ui->label_249),
        LED(ui->label_250), LED(ui->label_251), LED(ui->label_252), LED(ui->label_253), LED(ui->label_254),
        LED(ui->label_255), LED(ui->label_256), LED(ui->label_257), LED(ui->label_258), LED(ui->label_259),
        LED(ui->label_260), LED(ui->label_261), LED(ui->label_262), LED(ui->label_263), LED(ui->label_264),
        LED(ui->label_265), LED(ui->label_266), LED(ui->label_267), LED(ui->label_268), LED(ui->label_269),
    };
}
