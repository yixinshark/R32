//
// Created by zyz on 23-5-13.
//

#include "recvr32datawidget.h"
#include "delayedbutton.h"

#include "handledatabase.h"
#include "serialportcom.h"
#include "analyserconstant.h"

#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QCheckBox>

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

    m_showAlarmValue = new QLineEdit(this);
    m_showAlarmValue->setReadOnly(true);
    m_showAlarmValue->setPlaceholderText("显示ADC值");
    m_showR32NDValue = new QLineEdit(this);
    m_showR32NDValue->setReadOnly(true);
    m_showR32NDValue->setPlaceholderText("显示浓度值");

    auto *label3 = new QLabel("输入分析仪地址:", this);
    auto *inputAddr = new QLineEdit(this);
    auto *checkMore = new QCheckBox("获取更多信息", this);
    checkMore->setToolTip("勾选，则可获取更多信息，如低报值，高报值等");

    auto *btn = new DelayedButton("读取分析仪数据", this);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &DelayedButton::delayedClicked, this, [this, timer, inputAddr, checkMore]{
        m_showAlarmValue->clear();
        m_showR32NDValue->clear();

        if (m_serialPortCom->isSerialPortOpen() && !timer->isActive()) {
            timer->start();
        }

        QVariantMap info;

        QString address = inputAddr->text();
        // 判断是否为空和为一个字节的数字
        if (!address.isEmpty() && address.toInt() != 0) {
            info.insert(ADDRESS, inputAddr->text().toInt());
        }

        info.insert(MORE_INFO, checkMore->isChecked());

        sendCmdData(ANALYSER_CMD, info);
    });

    connect(timer, &QTimer::timeout, this, [btn, timer, this, checkMore]{
        if (!m_serialPortCom->isSerialPortOpen()) {
            timer->stop();
            m_showAlarmValue->clear();
            m_showR32NDValue->clear();
            checkMore->setChecked(false);
            return;
        }

        // 模拟按钮点击，相当于定时获取数据
        emit btn->delayedClicked();
    });

    auto *label = new QLabel("实时显示结果:", this);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addStretch();

    m_gridLayout->addWidget(label3, 0, 0);
    m_gridLayout->addWidget(inputAddr, 0, 1);
    m_gridLayout->addWidget(checkMore, 0, 2);

    m_gridLayout->addLayout(hLayout, 1, 0, 1, 3);
    m_gridLayout->addWidget(label, 2, 0);
    m_gridLayout->addWidget(m_showAlarmValue, 2, 1);
    m_gridLayout->addWidget(m_showR32NDValue, 2, 2);
}

void RecvR32DataWidget::recvAckData(quint8 cmd, const QVariantMap &info)
{
    qInfo() << Q_FUNC_INFO << cmd << info;
    if (cmd == ANALYSER_CMD) {
        showR32Info(info);
    }
}

void RecvR32DataWidget::showR32Info(const QVariantMap &info)
{
    if (!info.contains(ANALYSER_GAS_CONCENTRATION) ||
        !info.contains(ANALYSER_ALARM_STATUS)) {
        qWarning() << "show-read-R32-info error:" << info;
        return;
    }

    Q_EMIT r32NDValue(info[ANALYSER_GAS_CONCENTRATION].toInt());

    m_showR32NDValue->setText(info[ANALYSER_GAS_CONCENTRATION].toString());
    m_showAlarmValue->setText(info[ANALYSER_GAS_CONCENTRATION].toString());
}
