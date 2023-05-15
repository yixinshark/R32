//
// Created by zyz on 23-5-13.
//

#include "recvr32datawidget.h"
#include "delayedbutton.h"

#include "../serialport/handledata.h"
#include "../serialport/constant.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

RecvR32DataWidget::RecvR32DataWidget(QWidget *parent)
    : OperateBaseWidget(parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_showADCValue(new QLineEdit(this))
    , m_showTemperatureValue(new QLineEdit(this))
    , m_showR32ADCValue(new QLineEdit(this))
    , m_showR32NDValue(new QLineEdit(this))
{
    initUI();
    connect(m_handleData, &HandleData::frameReceived, this, &RecvR32DataWidget::recvAckData);
}

RecvR32DataWidget::~RecvR32DataWidget()
{

}

void RecvR32DataWidget::initUI()
{
    auto *layout = initSerialPortUI();

    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addLayout(layout);

    initReadNTCInfoUI();
    initReadR32InfoUI();
}

void RecvR32DataWidget::recvAckData(int cmd, const QVariantMap &info)
{
    qInfo() << Q_FUNC_INFO << cmd << info;
    switch (cmd) {
        case NCT_CMD:
            showNTCInfo(info);
            break;
        case R32_CMD:
            showR32Info(info);
            break;
    }
}

void RecvR32DataWidget::initReadNTCInfoUI()
{
    auto *btn = new DelayedButton("读取NTC的ADC和温度", this);
    btn->setFixedWidth(190);
    btn->setObjectName(CMD3_OBJECT_NAME);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);

    auto *adLabel = new QLabel("输入地址0x:", this);
    auto *inputAddress = new QLineEdit(this);
    inputAddress->setFixedWidth(60);
    connect(btn, &DelayedButton::clicked, this, [this, inputAddress]{
        m_showADCValue->clear();
        m_showTemperatureValue->clear();

        char sendAddress = static_cast<quint8>(inputAddress->text().toInt());
        QVariantMap info;
        info.insert(SEND_ADDRESS, sendAddress);
        sendDataBtnClicked(info);
    });

    auto *label = new QLabel("ADC值:", this);
    auto *label1 = new QLabel("温度值:", this);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(adLabel);
    hLayout->addWidget(inputAddress);
    hLayout->addWidget(label);
    hLayout->addWidget(m_showADCValue);
    hLayout->addWidget(label1);
    hLayout->addWidget(m_showTemperatureValue);

    m_mainLayout->addLayout(hLayout);
}

void RecvR32DataWidget::initReadR32InfoUI()
{
    auto *btn = new DelayedButton("读取R32的ADC和浓度", this);
    btn->setFixedWidth(190);
    btn->setObjectName(CMD4_OBJECT_NAME);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);

    auto *adLabel = new QLabel("输入地址0x:", this);
    auto *inputAddress = new QLineEdit(this);
    inputAddress->setFixedWidth(60);

    connect(btn, &DelayedButton::delayedClicked, this, [this, inputAddress]{
        m_showR32ADCValue->clear();
        m_showR32NDValue->clear();

        char sendAddress = static_cast<quint8>(inputAddress->text().toInt());
        QVariantMap info;
        info.insert(SEND_ADDRESS, sendAddress);
        sendDataBtnClicked(info);
    });

    auto *label = new QLabel("ADC值:", this);
    auto *label1 = new QLabel("浓度值:", this);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(adLabel);
    hLayout->addWidget(inputAddress);
    hLayout->addWidget(label);
    hLayout->addWidget(m_showR32ADCValue);
    hLayout->addWidget(label1);
    hLayout->addWidget(m_showR32NDValue);

    m_mainLayout->addLayout(hLayout);
}

void RecvR32DataWidget::showNTCInfo(const QVariantMap &info)
{
    if (!info.contains(ADC_VALUE) || !info.contains(TEMPERATURE)) {
        qWarning() << "show-read-NTC-info error:" << info;
        return;
    }

    m_showADCValue->setText(info[ADC_VALUE].toString());
    m_showTemperatureValue->setText(info[TEMPERATURE].toString());
}

void RecvR32DataWidget::showR32Info(const QVariantMap &info)
{
    if (!info.contains(ADC_VALUE) || !info.contains(CONCENTRATION)) {
        qWarning() << "show-read-R32-info error:" << info;
        return;
    }

    m_showR32ADCValue->setText(info[ADC_VALUE].toString());
    m_showR32NDValue->setText(info[TEMPERATURE].toString());
}
