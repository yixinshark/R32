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
{
    initUI();
}

McuOperateWidget::~McuOperateWidget()
{

}

void McuOperateWidget::initUI()
{
    m_gridLayout->setSpacing(10);
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
//    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(initSerialPortUI());
//    mainLayout->addLayout(m_gridLayout);

    auto *hLayout = new QHBoxLayout();
    hLayout->addLayout(m_gridLayout);
    hLayout->addSpacerItem(new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    mainLayout->addLayout(hLayout);
//    mainLayout->addStretch();
}

void McuOperateWidget::initBoxSwitchUI(int rowIndex)
{
    auto *powerButton = new DelayedButton("箱体开关", this);
    auto *powerButtonGroup = new QButtonGroup(this);
    auto *onRadioButton = new QRadioButton("开", this);
    auto *offRadioButton = new QRadioButton("关", this);
    powerButtonGroup->addButton(onRadioButton);
    powerButtonGroup->addButton(offRadioButton);

    m_gridLayout->addWidget(powerButton, rowIndex, 0);
    m_gridLayout->addWidget(onRadioButton, rowIndex, 1);
    m_gridLayout->addWidget(offRadioButton, rowIndex, 2);
    connect(powerButton, &DelayedButton::delayedClicked, this, [this, onRadioButton]() {
        qInfo() << "箱体开关:" << onRadioButton->isChecked();
        QVariantMap info;
        info.insert(MCU_BOX_SWITCH, onRadioButton->isChecked() ? 0 : 1);

        sendCmdData(MCU_CMD_BOX, info);
    });
}

void McuOperateWidget::initValveCtrlUI(int rowIndex)
{
    auto *openValveButton = new DelayedButton("电磁阀控制", this);
    auto *valve1CheckBox = new QCheckBox("电磁阀1", this);
    auto *valve2CheckBox = new QCheckBox("电磁阀2", this);
    auto *valve3CheckBox = new QCheckBox("电磁阀3", this);
    auto *valve4CheckBox = new QCheckBox("电磁阀4", this);
    auto *valve5CheckBox = new QCheckBox("电磁阀5", this);

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
        data.append(valve1CheckBox->isChecked() ? 0 : 1);
        data.append(valve2CheckBox->isChecked() ? 0 : 1);
        data.append(valve3CheckBox->isChecked() ? 0 : 1);
        data.append(valve4CheckBox->isChecked() ? 0 : 1);
        data.append(valve5CheckBox->isChecked() ? 0 : 1);

        QVariantMap info;
        info.insert(MCU_VALVE, data);

        sendCmdData(MCU_CMD_VALVE, info);
    });
}

void McuOperateWidget::initFanCtrlUI(int rowIndex)
{
    auto *fanBtn = new DelayedButton("风扇控制", this);
    auto *fan1CheckBox = new QCheckBox("风扇1", this);
    auto *fan2CheckBox = new QCheckBox("风扇2", this);
    auto *fan3CheckBox = new QCheckBox("风扇3", this);
    auto *fan4CheckBox = new QCheckBox("风扇4", this);

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
        data.append(fan1CheckBox->isChecked() ? 0 : 1);
        data.append(fan2CheckBox->isChecked() ? 0 : 1);
        data.append(fan3CheckBox->isChecked() ? 0 : 1);
        data.append(fan4CheckBox->isChecked() ? 0 : 1);

        QVariantMap info;
        info.insert(MCU_FAN, data);

        sendCmdData(MCU_CMD_FAN, info);
    });
}

void McuOperateWidget::initConcentrationCtrlUI(int rowIndex)
{
    auto *controlButton = new DelayedButton("浓度控制", this);
    auto *concentrationLabel = new QLabel("浓度:", this);
    auto *concentrationLineEdit = new QLineEdit(this);
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(concentrationLabel);
    hLayout->addWidget(concentrationLineEdit);

    m_gridLayout->addWidget(controlButton, rowIndex, 0);
    m_gridLayout->addLayout(hLayout, rowIndex, 1, 1, 5);

    connect(controlButton, &DelayedButton::delayedClicked, this, [this, concentrationLineEdit]() {
        qInfo() << "浓度控制:" << concentrationLineEdit->text();

        QVariantMap info;
        info.insert(MCU_ND, concentrationLineEdit->text().toInt());

        sendCmdData(MCU_CMD_ND, info);
    });
}

void McuOperateWidget::initChannelSelectUI(int rowIndex)
{
    auto *channelBtn = new DelayedButton("通道选择", this);
    auto *channelBox = new QComboBox(this);
    channelBox->setMinimumHeight(30);
    for (int i = 1; i <= 64; ++i) {
        channelBox->addItem(QString::number(i));
    }

    m_gridLayout->addWidget(channelBtn, rowIndex, 0);
    m_gridLayout->addWidget(channelBox, rowIndex, 1, 1, 5);
    connect(channelBtn, &DelayedButton::delayedClicked, this, [this, channelBox]() {
        qInfo() << "通道选择:" << channelBox->currentText();

        QVariantMap info;
        info.insert(MCU_SET_CHANNEL, channelBox->currentText().toInt());

        sendCmdData(MCU_CMD_CHANNEL, info);
    });
}

void McuOperateWidget::initAlarmCtrlUI(int rowIndex)
{
    auto *alarmButton = new DelayedButton("报警灯控制", this);
    auto *alarmLabel = new QLabel("通道:", this);
    auto *alarmLineEdit = new QLineEdit(this);
    auto *alarmOnRadioButton = new QRadioButton("开");
    auto *alarmOffRadioButton = new QRadioButton("关");

    auto *alarmButtonGroup = new QButtonGroup(this);
    alarmButtonGroup->addButton(alarmOnRadioButton);
    alarmButtonGroup->addButton(alarmOffRadioButton);

    auto *hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(alarmLabel);
    hLayout2->addWidget(alarmLineEdit);

    m_gridLayout->addWidget(alarmButton, rowIndex, 0);
    m_gridLayout->addLayout(hLayout2, rowIndex, 1, 1, 2);
    m_gridLayout->addWidget(alarmOnRadioButton, rowIndex, 3);
    m_gridLayout->addWidget(alarmOffRadioButton, rowIndex, 4);

    connect(alarmButton, &DelayedButton::delayedClicked, this, [this, alarmLineEdit,
                                                          alarmOnRadioButton]() {
        qInfo() << "报警灯控制:" << alarmLineEdit->text() << alarmOnRadioButton->isChecked();

        QVariantMap info;
        info.insert(MCU_SET_CHANNEL, alarmLineEdit->text().toInt());
        info.insert(MCU_ALARM_LIGHT, alarmOnRadioButton->isChecked());

        sendCmdData(MCU_CMD_ALARM_LIGHT, info);
    });
}

void McuOperateWidget::recvAckData(quint8 cmd, const QVariantMap &info)
{

}
