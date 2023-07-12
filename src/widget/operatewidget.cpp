//
// Created by zyz on 23-5-12.
//

#include "operatewidget.h"
#include "recvr32datawidget.h"
#include "mcuoperatewidget.h"
#include "operater32widget.h"
#include "mcuctrlstatuswidget.h"

#include "handler32data.h"
#include "handlemcudata.h"
#include "handleanalyserdata.h"

#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateTime>

OperateWidget::OperateWidget(QWidget *parent)
    : QWidget(parent)
    , m_msgLabel(new QLabel(this))
{

    auto *handlerMcuData = new HandleMcuData(this);
    auto *handler32Data = new Handler32data(this);
    auto *handlerAnalyserData = new HandleAnalyserData(this);

    m_operateR32Widget = new OperateR32Widget(handler32Data, this);
    m_recvR32DataWidget = new RecvR32DataWidget(handlerAnalyserData, this);
    m_mcuOperateWidget = new McuOperateWidget(handlerMcuData, this);
    m_mcuCtrlStatusWidget = new McuCtrlStatusWidget(this);

    initUI();

    connect(m_operateR32Widget, &OperateR32Widget::operatedMsg, this, &OperateWidget::showMsg);
    connect(m_recvR32DataWidget, &RecvR32DataWidget::operatedMsg, this, &OperateWidget::showMsg);
    connect(m_mcuOperateWidget, &McuOperateWidget::operatedMsg, this, &OperateWidget::showMsg);

    connect(m_mcuOperateWidget, &McuOperateWidget::fanStatusChanged,
            m_mcuCtrlStatusWidget, &McuCtrlStatusWidget::updateFanStatus);
    connect(m_mcuOperateWidget, &McuOperateWidget::valveStatusChanged,
            m_mcuCtrlStatusWidget, &McuCtrlStatusWidget::updateValveStatus);

    connect(m_mcuOperateWidget, &McuOperateWidget::serialPortClosed,
            m_mcuCtrlStatusWidget, &McuCtrlStatusWidget::reset);
    // connect(m_recvR32DataWidget, &RecvR32DataWidget::r32NDValue, m_operateR32Widget, &OperateR32Widget::setNDValue);
    // 单片机选择通道后同步到模块地址上
    connect(m_mcuOperateWidget, &McuOperateWidget::channelSelectChanged,
            m_operateR32Widget, &OperateR32Widget::setSelectedChannel);
}

OperateWidget::~OperateWidget()
{

}

void OperateWidget::initUI()
{
    auto *mainLayout = new QVBoxLayout();;
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 添加一个groupbox
    auto *groupBox = new QGroupBox(tr("R32传感器"), this);

    auto *vLayout = new QHBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 1);
    vLayout->addWidget(m_operateR32Widget);
    groupBox->setLayout(vLayout);
    mainLayout->addWidget(groupBox);

    // 添加一个groupbox
    auto *r32GroupBox = new QGroupBox(tr("R32分析仪"), this);

    auto *r32VLayout = new QHBoxLayout();
    r32VLayout->setContentsMargins(0, 0, 0, 0);
    r32VLayout->addWidget(m_recvR32DataWidget);
    r32GroupBox->setLayout(r32VLayout);

    // 添加一个groupbox
    auto *mcuGroupBox = new QGroupBox(tr("MCU单片机操作"), this);

    auto *mcuVLayout = new QHBoxLayout();
    mcuVLayout->setContentsMargins(0, 0, 0, 0);
    mcuVLayout->addWidget(m_mcuOperateWidget);
    mcuGroupBox->setLayout(mcuVLayout);

    auto *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(r32GroupBox);
    hLayout->addWidget(mcuGroupBox);

    // 添加一个groupbox
    auto *mcuCtrlGroupBox = new QGroupBox(tr("MCU单片机状态"), this);
    auto *mcuCtrlVLayout = new QHBoxLayout();
    mcuCtrlVLayout->setContentsMargins(0, 0, 0, 0);
    mcuCtrlVLayout->addWidget(m_mcuCtrlStatusWidget);
    mcuCtrlGroupBox->setLayout(mcuCtrlVLayout);
    hLayout->addWidget(mcuCtrlGroupBox);
    mainLayout->addLayout(hLayout);

    mainLayout->addWidget(m_msgLabel);
    setLayout(mainLayout);
}

void OperateWidget::showMsg(const QString &msg)
{
    m_msgLabel->clear();
    m_msgLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " " + msg);
}
