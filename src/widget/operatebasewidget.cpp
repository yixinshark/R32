//
// Created by zyz on 23-5-13.
//

#include "operatebasewidget.h"
#include "serialportsettingswidget.h"
#include "statuswidget.h"
#include "delayedbutton.h"

#include "handledatabase.h"
#include "serialportcom.h"

#include <QDebug>
#include <QTimer>
#include <QEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSerialPort>

OperateBaseWidget::OperateBaseWidget(HandleDataBase *handleData, QWidget *parent)
    : QWidget(parent)
    , m_connectBtn(new QPushButton("连接", this))
    , m_cntStatusWidget(new StatusWidget(this))
    , m_serialPortSettings(new SerialPortSettingsWidget(this))
    , m_serialPortCom(new SerialPortCom(this))
    , m_handleData(handleData)
{
    m_connectBtn->setObjectName("connect");


    connect(m_serialPortCom, &SerialPortCom::dataReceived,
            m_handleData, &HandleDataBase::processReceivedData);

    connect(m_handleData, &HandleDataBase::recvedFrameData, this, [this](const QByteArray &data){
        operateMsg("接收到数据:" + data.toHex());
    });

    connect(m_connectBtn, &QPushButton::clicked, this, [this]{
        m_connectBtn->setEnabled(false);

        // 准备连接
        if (m_connectBtn->objectName() == "connect") {
            qInfo() << "----------begin connect serial prot-----------";
            if (connectSerialPort()) {
                m_cntStatusWidget->setSelected(true);
                m_connectBtn->setObjectName("disConnect");
                m_connectBtn->setText("断开");
            }
        } else if (m_connectBtn->objectName() == "disConnect") {
            m_serialPortCom->closeSerialPort();
            m_cntStatusWidget->setSelected(false);
            m_connectBtn->setObjectName("connect");
            m_connectBtn->setText("连接");
            operateMsg(m_serialPortSettings->getSelectedPort() + " 连接断开");
        }

        // 1s后使能
        QTimer::singleShot(1000, this, [this]{
            m_connectBtn->setEnabled(true);
        });
    });

    connect(m_handleData, &HandleDataBase::frameReceived, this, &OperateBaseWidget::recvAckData);
}

OperateBaseWidget::~OperateBaseWidget()
{

}

QLayout *OperateBaseWidget::initSerialPortUI()
{
    auto *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(m_serialPortSettings);

#if 1
    auto *vLayout = new QVBoxLayout();
    vLayout->setSpacing(5);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(m_cntStatusWidget);
    vLayout->addWidget(m_connectBtn);

    hLayout->addLayout(vLayout);
#else
    hLayout->addWidget(m_cntStatusWidget);
    hLayout->addWidget(m_connectBtn);
#endif
    return  hLayout;
}

bool OperateBaseWidget::connectSerialPort()
{
    QString portName = m_serialPortSettings->getSelectedPort();
    qint32 baudRate = m_serialPortSettings->getSelectedBaudRate();

    QSerialPort::DataBits sDataBits = QSerialPort::UnknownDataBits;
    int dataBits = m_serialPortSettings->getSelectedDataBits();
    switch (dataBits) {
        case 8:
            sDataBits = QSerialPort::Data8;
            break;
        case 7:
            sDataBits = QSerialPort::Data7;
            break;
        case 6:
            sDataBits = QSerialPort::Data6;
            break;
        case 5:
            sDataBits = QSerialPort::Data5;
            break;
        default:
            break;
    }

    QString parity = m_serialPortSettings->getSelectedParity();
    QSerialPort::Parity sParity = QSerialPort::UnknownParity;
    if (parity == "Even") {
        sParity = QSerialPort::EvenParity;
    } else if (parity == "Odd") {
        sParity = QSerialPort::OddParity;
    }

    QString stopBits = m_serialPortSettings->getSelectedStopBits();
    QSerialPort::StopBits sStopBits = QSerialPort::UnknownStopBits;
    if (stopBits == "1") {
        sStopBits = QSerialPort::OneStop;
    } else if (stopBits == "2") {
        sStopBits = QSerialPort::TwoStop;
    }else if (stopBits == "1.5") {
        sStopBits = QSerialPort::OneAndHalfStop;
    }

    //qInfo() << "cmd serialPort connected:" << portName << " " << baudRate << " parity:" << parity
    //    << " dataBits:" << dataBits << " stopBits:" << stopBits;

    bool ret = m_serialPortCom->openSerialPort(portName, baudRate, sDataBits, sParity, sStopBits);
    QString msg = ret ? "连接成功" : "连接失败";
    msg += " serialPort:" + portName + " 波特率:" + QString::number(baudRate) + " 检验和:" +
            parity + " 数据位:" + QString::number(dataBits) + " 停止位:" + stopBits;

    operateMsg(msg);

   return ret;
}

void OperateBaseWidget::operateMsg(const QString &msg)
{
    Q_EMIT operatedMsg(msg);
}

bool OperateBaseWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress) {
        auto *delayBtn = qobject_cast<DelayedButton *>(watched);
        if (delayBtn) {
            delayBtn->setSelected(true);
            for (auto *btn : m_delayBtnList) {
                if (btn && btn != delayBtn) {
                    btn->setSelected(false);
                }
            }
        }
    }

    return QObject::eventFilter(watched, event);
}

void OperateBaseWidget::sendCmdData(quint8 cmd, const QVariantMap &info)
{
    QByteArray data = m_handleData->getSendData(cmd, info);
    qInfo() << "send cmd: " << cmd << " data:" << data;

    QString msg = QString("%1发送数据:").arg(m_serialPortCom->isSerialPortOpen() ? " " : "串口未打开,未");
    operateMsg(msg + data.toHex());

    m_serialPortCom->sendData(data);
}
