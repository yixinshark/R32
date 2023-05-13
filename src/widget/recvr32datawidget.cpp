//
// Created by zyz on 23-5-13.
//

#include "recvr32datawidget.h"
#include "../serialport/handledata.h"

#include <QDebug>
#include <QVBoxLayout>

RecvR32DataWidget::RecvR32DataWidget(QWidget *parent)
    : OperateBaseWidget(parent)
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

    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(layout);

    setLayout(mainLayout);
}

void RecvR32DataWidget::recvAckData(int cmd, const QVariantMap &info)
{
    qInfo() << Q_FUNC_INFO << cmd << info;

}
