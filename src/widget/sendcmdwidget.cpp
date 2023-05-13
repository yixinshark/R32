//
// Created by zyz on 23-5-13.
//

#include "sendcmdwidget.h"
#include "../serialport/handledata.h"
#include "../serialport/constant.h"
#include "../serialport/serialportcom.h"
#include "delayedbutton.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>

SendCmdWidget::SendCmdWidget(QWidget *parent)
    : OperateBaseWidget(parent)
{
    initUI();

    connect(m_handleData, &HandleData::frameReceived, this, &SendCmdWidget::recvAckData);
}

SendCmdWidget::~SendCmdWidget()
{

}

void SendCmdWidget::initUI()
{
    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(20);

    auto *layout = initSerialPortUI();
    mainLayout->addLayout(layout);

    auto *cmd1Btn = new DelayedButton("cmd1", this);
    cmd1Btn->setObjectName(CMD1_OBJECT_NAME);
    cmd1Btn->setFixedWidth(80);
    connect(cmd1Btn, &DelayedButton::delayedClicked, this, &SendCmdWidget::sendDataBtnClicked);

    auto *label = new QLabel(this);

    auto *cmd1Layout = new QHBoxLayout();
    cmd1Layout->addWidget(cmd1Btn);
    cmd1Layout->addWidget(label);

    mainLayout->addLayout(cmd1Layout);

    setLayout(mainLayout);
}

void SendCmdWidget::recvAckData(int cmd, const QVariantMap &info)
{
    qInfo() << Q_FUNC_INFO << cmd << info;

    // TODO
}

void SendCmdWidget::sendDataBtnClicked()
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
    QByteArray data = m_handleData->getSendData(cmd, QVariantMap());
    qInfo() << "send cmd: " << cmd << " data:" << data;

    // TODO data info
    m_serialPortCom->sendData(data);
}
