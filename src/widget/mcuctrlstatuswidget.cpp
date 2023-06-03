//
// Created by yixin on 2023/6/3.
//

#include "mcuctrlstatuswidget.h"
#include "fanwidget.h"
#include "valvewidget.h"

#include <QLabel>
#include <QGridLayout>

McuCtrlStatusWidget::McuCtrlStatusWidget(QWidget *parent)
    : QWidget(parent)
    , m_gridLayout(new QGridLayout(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    initUi();
}

McuCtrlStatusWidget::~McuCtrlStatusWidget()
{

}

void McuCtrlStatusWidget::initUi()
{
    initFanWidgets();
    initValveWidgets();
}

void McuCtrlStatusWidget::initFanWidgets()
{
    // 4个风扇
    for (int i = 0; i < 4; ++i) {
        auto *fanWidget = new FanWidget(this);
//        fanWidget->setFanOpened(true);
        m_fanWidgets.append(fanWidget);

        auto *vLayout = new QVBoxLayout();
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->addWidget(fanWidget);
        vLayout->addWidget(new QLabel("风扇" + QString::number(i + 1), this));

        m_gridLayout->addLayout(vLayout, i, 0, Qt::AlignCenter);
    }
}

void McuCtrlStatusWidget::initValveWidgets()
{
    // 5个电磁阀
    for (int i = 0; i < 5; ++i) {
        auto *valveWidget = new ValveWidget(this);
//        valveWidget->setValveOpened(true);
        m_valveWidgets.append(valveWidget);

        auto *vLayout = new QVBoxLayout();
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->addWidget(valveWidget);
        vLayout->addWidget(new QLabel("电磁阀" + QString::number(i + 1), this));

        m_gridLayout->addLayout(vLayout, i, 1, Qt::AlignLeft);
    }
}

void McuCtrlStatusWidget::updateFanStatus(const QByteArray &data)
{
    // 更新状态
    for (int i = 0; i < m_fanWidgets.size(); ++i) {
        m_fanWidgets[i]->setFanOpened(data.at(i) == 0x01);
    }
}

void McuCtrlStatusWidget::updateValveStatus(const QByteArray &data)
{
    // 更新状态
    for (int i = 0; i < m_valveWidgets.size(); ++i) {
        m_valveWidgets[i]->setValveOpened(data.at(i) == 0x01);
    }
}
