//
// Created by zyz on 23-5-12.
//

#include "operatewidget.h"
#include "serialportsettingswidget.h"
#include "sendcmdwidget.h"
#include "recvr32datawidget.h"

#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateTime>

OperateWidget::OperateWidget(QWidget *parent)
    : QWidget(parent)
    , m_sendCmdWidget(new SendCmdWidget(this))
    , m_recvR32DataWidget(new RecvR32DataWidget(this))
    , m_msgLabel(new QLabel(this))
{
    initUI();

    connect(m_sendCmdWidget, &SendCmdWidget::operatedMsg, this, &OperateWidget::showMsg);
    connect(m_recvR32DataWidget, &RecvR32DataWidget::operatedMsg, this, &OperateWidget::showMsg);
}

OperateWidget::~OperateWidget()
{

}

void OperateWidget::initUI()
{
    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 添加一个groupbox
    auto *groupBox = new QGroupBox(tr("指令操作"), this);

    auto *vLayout = new QHBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(m_sendCmdWidget);
    groupBox->setLayout(vLayout);
    mainLayout->addWidget(groupBox);

    // 添加一个groupbox
    auto *r32GroupBox = new QGroupBox(tr("R32数据"), this);

    auto *r32VLayout = new QHBoxLayout();
    r32VLayout->setContentsMargins(0, 0, 0, 0);
    r32VLayout->addWidget(m_recvR32DataWidget);
    r32GroupBox->setLayout(r32VLayout);
    mainLayout->addWidget(r32GroupBox);


    mainLayout->addStretch();
    mainLayout->addWidget(m_msgLabel);

    setLayout(mainLayout);
}

void OperateWidget::showMsg(const QString &msg)
{
    m_msgLabel->clear();
    m_msgLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " " + msg);
}
