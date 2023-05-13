//
// Created by zyz on 23-5-13.
//

#include "operatebasewidget.h"
#include "serialportsettingswidget.h"

#include "../serialport/handledata.h"
#include "../serialport/serialportcom.h"

#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSerialPort>

OperateBaseWidget::OperateBaseWidget(QWidget *parent)
    : QWidget(parent)
    , m_connectBtn(new QPushButton("连接", this))
    , m_serialPortSettings(new SerialPortSettingsWidget(this))
    , m_serialPortCom(new SerialPortCom(this))
    , m_handleData(new HandleData(this))
{
    m_connectBtn->setObjectName("connect");

    connect(m_serialPortCom, &SerialPortCom::dataReceived,
            m_handleData, &HandleData::processReceivedData);

    connect(m_connectBtn, &QPushButton::clicked, this, [this]{
        qInfo() << "----------begin connect serial prot-----------";
        m_connectBtn->setEnabled(false);

        // 准备连接
        if (m_connectBtn->objectName() == "connect") {
            if (connectSerialPort()) {
                m_connectBtn->setObjectName("disConnect");
                m_connectBtn->setText("断开");
            }
        } else if (m_connectBtn->objectName() == "disConnect") {
            m_serialPortCom->closeSerialPort();
            m_connectBtn->setObjectName("connect");
            m_connectBtn->setText("连接");
        }

        // 1s后使能
        QTimer::singleShot(1000, this, [this]{
            m_connectBtn->setEnabled(true);
        });
    });
}

OperateBaseWidget::~OperateBaseWidget()
{

}

QLayout *OperateBaseWidget::initSerialPortUI()
{
    auto *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(m_serialPortSettings);
    hLayout->addWidget(m_connectBtn);

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

    qInfo() << "cmd serialPort connected:" << portName << " " << baudRate << " parity:" << parity
        << " dataBits:" << dataBits << " stopBits:" << stopBits;

   return  m_serialPortCom->openSerialPort(portName, baudRate, sDataBits, sParity, sStopBits);
}
