//
// Created by zyz on 23-5-13.
//

#include "recvr32datawidget.h"
#include "delayedbutton.h"

#include "handledatabase.h"
#include "constant.h"
#include "serialportcom.h"

#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

RecvR32DataWidget::RecvR32DataWidget(HandleDataBase *handleData, QWidget *parent)
    : OperateBaseWidget(handleData, parent)
    , m_gridLayout(new QGridLayout())
{
    initUI();
//    connect(m_handleData, &HandleDataBase::frameReceived, this, &RecvR32DataWidget::recvAckData);
}

RecvR32DataWidget::~RecvR32DataWidget()
{

}

void RecvR32DataWidget::initUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    auto *layout = initSerialPortUI(SerialPortSettingsWidget::Vertical);
    mainLayout->addLayout(layout);

    m_gridLayout->setSpacing(13);

    //initReadNTCInfoUI();
    initReadR32InfoUI();
    mainLayout->addLayout(m_gridLayout);
    mainLayout->addStretch();
}

void RecvR32DataWidget::initReadR32InfoUI()
{
    auto *timer = new QTimer(this);
    timer->setSingleShot(false);
    // TODO 可配置
    timer->setInterval(1 * 1000); // 1s

    m_showR32ADCValue = new QLineEdit(this);
    m_showR32ADCValue->setReadOnly(true);
    m_showR32ADCValue->setPlaceholderText("显示ADC值");
    m_showR32NDValue = new QLineEdit(this);
    m_showR32NDValue->setReadOnly(true);
    m_showR32NDValue->setPlaceholderText("显示浓度值");

    auto *btn = new DelayedButton("读取R32的ADC和浓度", this);
    //btn->setFixedWidth(190);
    btn->setObjectName(CMD4_OBJECT_NAME);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &DelayedButton::delayedClicked, this, [this, timer]{
        m_showR32ADCValue->clear();
        m_showR32NDValue->clear();

        if (m_serialPortCom->isSerialPortOpen() && !timer->isActive()) {
            timer->start();
        }

        sendCmdData(R32_CMD);
    });

    connect(timer, &QTimer::timeout, this, [btn, timer, this]{
        if (!m_serialPortCom->isSerialPortOpen()) {
            timer->stop();
            m_showR32ADCValue->clear();
            m_showR32NDValue->clear();
            return;
        }

        // 模拟按钮点击，相当于定时获取数据
        emit btn->delayedClicked();
    });

    auto *label = new QLabel("ADC值:", this);
    auto *label1 = new QLabel("浓度值:", this);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addStretch();

    m_gridLayout->addLayout(hLayout, 0, 0, 1, 2);
    m_gridLayout->addWidget(label, 1, 0);
    m_gridLayout->addWidget(m_showR32ADCValue, 1, 1);
    m_gridLayout->addWidget(label1, 2, 0);
    m_gridLayout->addWidget(m_showR32NDValue, 2, 1);
}

void RecvR32DataWidget::recvAckData(quint8 cmd, const QVariantMap &info)
{
    qInfo() << Q_FUNC_INFO << cmd << info;
    if (cmd == R32_CMD) {
        showR32Info(info);
    }
}

void RecvR32DataWidget::showR32Info(const QVariantMap &info)
{
    if (!info.contains(ADC_VALUE) || !info.contains(CONCENTRATION)) {
        qWarning() << "show-read-R32-info error:" << info;
        return;
    }

    Q_EMIT r32NDValue(info[CONCENTRATION].toInt());

    m_showR32ADCValue->setText(info[ADC_VALUE].toString());
    m_showR32NDValue->setText(info[CONCENTRATION].toString());
}
