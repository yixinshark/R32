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

    m_mainLayout->setSpacing(13);
    m_mainLayout->setContentsMargins(0, 0, 0, 15);
    m_mainLayout->addLayout(layout);

    //initReadNTCInfoUI();
    m_mainLayout->addLayout(initReadR32InfoUI("读取R32的ADC和浓度"));
}
