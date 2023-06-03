//
// Created by yixin on 2023/5/31.
//

#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>

#include "delayedbutton.h"
#include "mcuoperatewidget.h"
#include "mcuconstant.h"

McuOperateWidget::McuOperateWidget(HandleDataBase *handleData, QWidget *parent)
    : OperateBaseWidget(handleData, parent)
    , m_gridLayout(new QGridLayout())
    , m_alarmChannelEdit(new QLineEdit(this))
    , m_concentrationEdit(new QLineEdit(this))
    , m_channelBox(new QComboBox(this))
{
    initUI();
}

McuOperateWidget::~McuOperateWidget()
{

}

void McuOperateWidget::initUI()
{
    // 第一行
    initBoxSwitchUI(0);
    // 第二行
    initValveCtrlUI(1);
    // 第三行
    initFanCtrlUI(2);
    // 第四行
    initConcentrationCtrlUI(3);
    // 第五行
    initChannelSelectUI(4);
    // 第六行
    initAlarmCtrlUI(5);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->addLayout(initSerialPortUI());

    auto *widget = new QWidget(this);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widget->setLayout(m_gridLayout);

    mainLayout->addWidget(widget);
}

void McuOperateWidget::initBoxSwitchUI(int rowIndex)
{
    auto *powerButton = new DelayedButton("箱体开关", this);
    powerButton->installEventFilter(this);
    m_delayBtnList.append(powerButton);
    auto *powerButtonGroup = new QButtonGroup(this);
    auto *onRadioButton = new QRadioButton("开", this);
    auto *offRadioButton = new QRadioButton("关", this);
    powerButtonGroup->addButton(onRadioButton);
    powerButtonGroup->addButton(offRadioButton);
    m_boxSwitchs.append(onRadioButton);
    m_boxSwitchs.append(offRadioButton);

    m_gridLayout->addWidget(powerButton, rowIndex, 0);
    m_gridLayout->addWidget(onRadioButton, rowIndex, 1);
    m_gridLayout->addWidget(offRadioButton, rowIndex, 2);
    connect(powerButton, &DelayedButton::delayedClicked, this, [this, onRadioButton]() {
        qInfo() << "箱体开关:" << onRadioButton->isChecked();
        QVariantMap info;
        info.insert(MCU_BOX_SWITCH, onRadioButton->isChecked() ? 1 : 0);

        sendCmdData(MCU_CMD_BOX, info);
    });
}

void McuOperateWidget::initValveCtrlUI(int rowIndex)
{
    auto *openValveButton = new DelayedButton("电磁阀控制", this);
    openValveButton->installEventFilter(this);
    m_delayBtnList.append(openValveButton);
    auto *valve1CheckBox = new QCheckBox("电磁阀1", this);
    auto *valve2CheckBox = new QCheckBox("电磁阀2", this);
    auto *valve3CheckBox = new QCheckBox("电磁阀3", this);
    auto *valve4CheckBox = new QCheckBox("电磁阀4", this);
    auto *valve5CheckBox = new QCheckBox("电磁阀5", this);
    m_valves.append(valve1CheckBox);
    m_valves.append(valve2CheckBox);
    m_valves.append(valve3CheckBox);
    m_valves.append(valve4CheckBox);
    m_valves.append(valve5CheckBox);

    m_gridLayout->addWidget(openValveButton, rowIndex, 0);
    m_gridLayout->addWidget(valve1CheckBox, rowIndex, 1);
    m_gridLayout->addWidget(valve2CheckBox, rowIndex, 2);
    m_gridLayout->addWidget(valve3CheckBox, rowIndex, 3);
    m_gridLayout->addWidget(valve4CheckBox, rowIndex, 4);
    m_gridLayout->addWidget(valve5CheckBox, rowIndex, 5);

    connect(openValveButton, &DelayedButton::delayedClicked, this, [this, valve1CheckBox,
                                                              valve2CheckBox, valve3CheckBox,
                                                              valve4CheckBox, valve5CheckBox]() {
        qInfo() << "电磁阀控制:" << valve1CheckBox->isChecked() << valve2CheckBox->isChecked() <<
            valve3CheckBox->isChecked() << valve4CheckBox->isChecked() << valve5CheckBox->isChecked();

        QByteArray data;
        data.append(valve1CheckBox->isChecked() ? 1 : 0);
        data.append(valve2CheckBox->isChecked() ? 1 : 0);
        data.append(valve3CheckBox->isChecked() ? 1 : 0);
        data.append(valve4CheckBox->isChecked() ? 1 : 0);
        data.append(valve5CheckBox->isChecked() ? 1 : 0);

        QVariantMap info;
        info.insert(MCU_VALVE, data);

        sendCmdData(MCU_CMD_VALVE, info);
    });
}

void McuOperateWidget::initFanCtrlUI(int rowIndex)
{
    auto *fanBtn = new DelayedButton("风扇控制", this);
    fanBtn->installEventFilter(this);
    m_delayBtnList.append(fanBtn);
    auto *fan1CheckBox = new QCheckBox("风扇1", this);
    auto *fan2CheckBox = new QCheckBox("风扇2", this);
    auto *fan3CheckBox = new QCheckBox("风扇3", this);
    auto *fan4CheckBox = new QCheckBox("风扇4", this);
    m_fans.append(fan1CheckBox);
    m_fans.append(fan2CheckBox);
    m_fans.append(fan3CheckBox);
    m_fans.append(fan4CheckBox);

    m_gridLayout->addWidget(fanBtn, rowIndex, 0);
    m_gridLayout->addWidget(fan1CheckBox, rowIndex, 1);
    m_gridLayout->addWidget(fan2CheckBox, rowIndex, 2);
    m_gridLayout->addWidget(fan3CheckBox, rowIndex, 3);
    m_gridLayout->addWidget(fan4CheckBox, rowIndex, 4);

    connect(fanBtn, &DelayedButton::delayedClicked, this, [this, fan1CheckBox,
                                                     fan2CheckBox, fan3CheckBox,
                                                     fan4CheckBox]() {
        qInfo() << "风扇控制:" << fan1CheckBox->isChecked() << fan2CheckBox->isChecked() <<
            fan3CheckBox->isChecked() << fan4CheckBox->isChecked();

        QByteArray data;
        data.append(fan1CheckBox->isChecked() ? 1 : 0);
        data.append(fan2CheckBox->isChecked() ? 1 : 0);
        data.append(fan3CheckBox->isChecked() ? 1 : 0);
        data.append(fan4CheckBox->isChecked() ? 1 : 0);

        QVariantMap info;
        info.insert(MCU_FAN, data);

        sendCmdData(MCU_CMD_FAN, info);
    });
}

void McuOperateWidget::initConcentrationCtrlUI(int rowIndex)
{
    auto *controlButton = new DelayedButton("浓度控制", this);
    controlButton->installEventFilter(this);
    m_delayBtnList.append(controlButton);
    auto *concentrationLabel = new QLabel("浓度:", this);
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(concentrationLabel);
    hLayout->addWidget(m_concentrationEdit);

    m_gridLayout->addWidget(controlButton, rowIndex, 0);
    m_gridLayout->addLayout(hLayout, rowIndex, 1, 1, 5);

    connect(controlButton, &DelayedButton::delayedClicked, this, [this]() {
        qInfo() << "浓度控制:" << m_concentrationEdit->text();

        QVariantMap info;
        info.insert(MCU_ND, m_concentrationEdit->text().toUInt());

        sendCmdData(MCU_CMD_ND, info);
    });
}

void McuOperateWidget::initChannelSelectUI(int rowIndex)
{
    auto *channelBtn = new DelayedButton("通道选择", this);
    channelBtn->installEventFilter(this);
    m_delayBtnList.append(channelBtn);
    m_channelBox->setMinimumHeight(30);
    for (int i = 1; i <= 64; ++i) {
        m_channelBox->addItem(QString::number(i));
    }

    m_gridLayout->addWidget(channelBtn, rowIndex, 0);
    m_gridLayout->addWidget(m_channelBox, rowIndex, 1, 1, 5);
    connect(channelBtn, &DelayedButton::delayedClicked, this, [this]() {
        qInfo() << "通道选择:" << m_channelBox->currentText();
        m_alarmChannelEdit->setText(m_channelBox->currentText());
        QVariantMap info;
        info.insert(MCU_SET_CHANNEL, m_channelBox->currentText().toInt());

        sendCmdData(MCU_CMD_CHANNEL, info);
    });
}

void McuOperateWidget::initAlarmCtrlUI(int rowIndex)
{
    auto *alarmButton = new DelayedButton("报警灯控制", this);
    alarmButton->installEventFilter(this);
    m_delayBtnList.append(alarmButton);
    auto *alarmLabel = new QLabel("通道:", this);
    auto *alarmOnRadioButton = new QRadioButton("开");
    auto *alarmOffRadioButton = new QRadioButton("关");
    m_alarmSwitchs.append(alarmOnRadioButton);
    m_alarmSwitchs.append(alarmOffRadioButton);

    auto *alarmButtonGroup = new QButtonGroup(this);
    alarmButtonGroup->addButton(alarmOnRadioButton);
    alarmButtonGroup->addButton(alarmOffRadioButton);

    auto *hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(alarmLabel);
    hLayout2->addWidget(m_alarmChannelEdit);

    m_gridLayout->addWidget(alarmButton, rowIndex, 0);
    m_gridLayout->addLayout(hLayout2, rowIndex, 1, 1, 2);
    m_gridLayout->addWidget(alarmOnRadioButton, rowIndex, 3);
    m_gridLayout->addWidget(alarmOffRadioButton, rowIndex, 4);

    connect(alarmButton, &DelayedButton::delayedClicked, this, [this,
                                                          alarmOnRadioButton]() {
        qInfo() << "报警灯控制:" << m_alarmChannelEdit->text() << alarmOnRadioButton->isChecked();

        QVariantMap info;
        info.insert(MCU_SET_CHANNEL, m_alarmChannelEdit->text().toInt());
        info.insert(MCU_ALARM_LIGHT, alarmOnRadioButton->isChecked());

        sendCmdData(MCU_CMD_ALARM_LIGHT, info);
    });
}

void McuOperateWidget::recvAckData(quint8 cmd, const QVariantMap &info)
{
    qInfo() << "mcuOperate widget recvAckData:" << cmd << info;
    QByteArray data = info.value(MCU_RECV_DATA).toByteArray();
    if (data.size() < MCU_CONTENT_LEN) {
        qWarning() << "mcuOperate widget showBoxSwitchStatus data error:" << data;
        return;
    }

    switch (cmd) {
        case MCU_CMD_VALVE:
            showValveCtrlStatus(data);
            break;
        case MCU_CMD_FAN:
            showFanCtrlStatus(data);
            break;
        case MCU_CMD_ND:
            showConcentrationValue(data);
            break;
        case MCU_CMD_CHANNEL:
            showChannelSelectValue(data);
            break;
        case MCU_CMD_ALARM_LIGHT:
            showAlarmCtrlStatus(data);
            break;
        case MCU_CMD_BOX:
            showBoxSwitchStatus(data);
            break;
        default:
            qWarning() << "mcuOperate widget recvAckData cmd error:" << cmd;
            break;
    }
}

void McuOperateWidget::showBoxSwitchStatus(const QByteArray &info)
{
    char openStatus = info.at(0);
    m_boxSwitchs.at(0)->setChecked(openStatus & OPEN);
    m_boxSwitchs.at(1)->setChecked(openStatus & OPEN);
}

void McuOperateWidget::showValveCtrlStatus(const QByteArray &info)
{
    for (int i = 0; i < m_valves.size(); ++i) {
        m_valves.at(i)->setChecked(info.at(i) & OPEN);
    }
}

void McuOperateWidget::showFanCtrlStatus(const QByteArray &info)
{
    for (int i = 0; i < m_fans.size(); ++i) {
        m_fans.at(i)->setChecked(info.at(i) & OPEN);
    }
}

void McuOperateWidget::showConcentrationValue(const QByteArray &info)
{
    // 两个字节代表浓度值
    quint16 concentration = info.at(0) << 8 | info.at(1);
    m_concentrationEdit->setText(QString::number(concentration));
}

void McuOperateWidget::showChannelSelectValue(const QByteArray &info)
{
    m_channelBox->setCurrentText(QString::number(info.at(0)));
}

void McuOperateWidget::showAlarmCtrlStatus(const QByteArray &info)
{
    m_alarmChannelEdit->setText(QString::number(info.at(0)));
    m_alarmSwitchs.at(0)->setChecked(info.at(1) & OPEN);
    m_alarmSwitchs.at(1)->setChecked(info.at(1) & OPEN);
}
