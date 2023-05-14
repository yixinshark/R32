//
// Created by zyz on 23-5-13.
//

#include "sendcmdwidget.h"
#include "../serialport/handledata.h"
#include "../serialport/constant.h"
#include "../serialport/serialportcom.h"
#include "delayedbutton.h"

#include <QDebug>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>

SendCmdWidget::SendCmdWidget(QWidget *parent)
    : OperateBaseWidget(parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_inputSlaveAddress(new QLineEdit(this))
    , m_slaveAddressLabel(new QLabel(this))
{
    initUI();

    connect(m_handleData, &HandleData::frameReceived, this, &SendCmdWidget::recvAckData);
}

SendCmdWidget::~SendCmdWidget()
{

}

void SendCmdWidget::initUI()
{
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(20);

    auto *layout = initSerialPortUI();
    m_mainLayout->addLayout(layout);

    auto *cmd1Btn = new DelayedButton("cmd1", this);
    cmd1Btn->setObjectName(CMD1_OBJECT_NAME);
    cmd1Btn->setFixedWidth(80);
    connect(cmd1Btn, &DelayedButton::delayedClicked, this, [this]{
        sendDataBtnClicked();
    });

    auto *label = new QLabel(this);

    auto *cmd1Layout = new QHBoxLayout();
    cmd1Layout->addWidget(cmd1Btn);
    cmd1Layout->addWidget(label);

    m_mainLayout->addLayout(cmd1Layout);

    initSlaveAddressUI();
}

void SendCmdWidget::recvAckData(int cmd, const QVariantMap &info)
{
    qInfo() << Q_FUNC_INFO << cmd << info;
    switch (cmd) {
        case READ_PRODUCT_ADDR_CMD:
            showProductSlaveAddress(info);
            break;
    }

    // TODO
}

void SendCmdWidget::sendDataBtnClicked(const QVariantMap &info)
{
    auto *btn = qobject_cast<DelayedButton *>(sender());
    if (!btn) {
        return;
    }

    QString objectName = btn->objectName();
    if (!ObjectNameCmdMap.contains(objectName)) {
        qWarning() << "send data btn objectName error:" << objectName;
        return;
    }

    int cmd = ObjectNameCmdMap.value(objectName);
    QByteArray data = m_handleData->getSendData(cmd, info);
    qInfo() << "send cmd: " << cmd << " data:" << data;

    operateMsg("发送数据:" + data.toHex());

    m_serialPortCom->sendData(data);
}

void SendCmdWidget::initSlaveAddressUI()
{
    auto *btn = new DelayedButton("设置从机地址", this);
    btn->setObjectName(CMD_FD_OBJECT_NAME);
    connect(btn, &QPushButton::clicked, this, [this]{
        QString slaveAddress = m_inputSlaveAddress->text();
        if (slaveAddress.isEmpty()) {
            return;
        }

        bool ok = false;
        int slaveAddressInt = slaveAddress.toInt(&ok, 16);
        if (!ok) {
            return;
        }

        QVariantMap info;
        info.insert(SLAVE_ADDRESS, slaveAddressInt);
        info.insert(STATIC_ADDRESS, static_address_value);
        qInfo() << "slave address:" << slaveAddressInt;

        sendDataBtnClicked(info);
    });

    auto *label = new QLabel("输入地址0x:", this);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(label);
    hLayout->addWidget(m_inputSlaveAddress);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::initReadSlaveAddressUI()
{
    auto *btn = new DelayedButton("读取从机地址", this);
    btn->setObjectName(CMD_FE_OBJECT_NAME);
    connect(btn, &QPushButton::clicked, this, [this]{
        m_slaveAddressLabel->clear();

        QVariantMap info;
        info.insert(STATIC_ADDRESS, static_address_value);

        sendDataBtnClicked(info);
    });

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(m_slaveAddressLabel);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showProductSlaveAddress(const QVariantMap &info)
{
    if (!info.contains(SLAVE_ADDRESS)) {
        qWarning() << "show-product-slave-address info error:" << info;
        return;
    }

    int value = info.value(SLAVE_ADDRESS).toInt();
    m_slaveAddressLabel->setText(QString("Hex: 0x%1").arg(QString::number(value, 16)));
}
