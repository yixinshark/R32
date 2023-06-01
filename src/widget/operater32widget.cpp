//
// Created by yixin on 2023/6/1.
//

#include "operater32widget.h"
#include "delayedbutton.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLineEdit>

OperateR32Widget::OperateR32Widget(HandleDataBase *handleData, QWidget *parent)
    : OperateBaseWidget(handleData, parent)
    , m_gridLayout(new QGridLayout())
    , m_mainLayout(new QVBoxLayout(this))
    , m_calibrationResultEdit(new QLineEdit(this))
    , m_calFinishResultEdit(new QLineEdit(this))
    , m_calStatusResultEdit(new QLineEdit(this))
    , m_readZeroResResultEdit(new QLineEdit(this))
    , m_readCalP1ResultEdit(new QLineEdit(this))
    , m_readCalP2ResultEdit(new QLineEdit(this))
    , m_readCalP3ResultEdit(new QLineEdit(this))
    , m_readCalP4ResultEdit(new QLineEdit(this))
    , m_curTempResultEdit(new QLineEdit(this))
    , m_curHumidityResultEdit(new QLineEdit(this))
    , m_curGasADCResultEdit(new QLineEdit(this))
    , m_gasResResultEdit(new QLineEdit(this))
    , m_compensateResResultEdit(new QLineEdit(this))
    , m_concentrationResultEdit(new QLineEdit(this))
    , m_alarmStatusResultEdit(new QLineEdit(this))
    , m_mainVersionResultEdit(new QLineEdit(this))
    , m_subVersionResultEdit(new QLineEdit(this))
    , m_setAlarmThresholdResultEdit(new QLineEdit(this))
    , m_readAlarmThresholdResultEdit(new QLineEdit(this))
    , m_readFaultCodeResultEdit(new QLineEdit(this))
    , m_printDataSwitchResultEdit(new QLineEdit(this))
    , m_setModuleAddressResultEdit(new QLineEdit(this))
    , m_readModuleAddressResultEdit(new QLineEdit(this))
{
    initUI();
}

OperateR32Widget::~OperateR32Widget()
{

}

void OperateR32Widget::initUI()
{
    m_mainLayout->setSpacing(20);
    m_gridLayout->setSpacing(10);
    m_mainLayout->addLayout(initSerialPortUI());
    // 初始化标定类指令UI
    initCalibrationCmdUI();
    // 初始化读取参数类指令UI
    initReadParamCmdUI();
    // 初始化检测类指令UI
    initDetectionCmdUI();
    // 初始化系统类指令UI
    initSystemCmdUI();

    // 初始化广播类指令UI
    initBroadcastCmdUI();

    m_mainLayout->addLayout(m_gridLayout);
}

void OperateR32Widget::initCalibrationCmdUI()
{
    // 标定按钮
    auto *calibrationBtn = new DelayedButton("标定浓度", this);

    // 点位输入框
    auto *pointEdit = new QLineEdit(this);
    pointEdit->setPlaceholderText("请输入点位");

    // 浓度输入框
    auto *concentrationEdit = new QLineEdit(this);
    concentrationEdit->setPlaceholderText("请输入浓度");

    // 结果显示框
    m_calibrationResultEdit->setPlaceholderText("结果或错误信息显示");
    m_calibrationResultEdit->setReadOnly(true);

    m_gridLayout->addWidget(calibrationBtn, 0, 0, 2, 1);
    m_gridLayout->addWidget(pointEdit, 0, 1, 1, 1, Qt::AlignLeft);
    m_gridLayout->addWidget(concentrationEdit, 0, 2, 1, 1, Qt::AlignLeft);
    m_gridLayout->addWidget(m_calibrationResultEdit, 1, 1, 1, 2);

    // 标定完成按钮
    auto *calibrationFinishBtn = new DelayedButton("标定完成", this);
    // 结果显示框
    m_calFinishResultEdit->setPlaceholderText("结果或错误信息显示");
    m_calFinishResultEdit->setReadOnly(true);

    // 从第三行开始
    m_gridLayout->addWidget(calibrationFinishBtn, 3, 0, 1, 1);
    m_gridLayout->addWidget(m_calFinishResultEdit, 3, 1, 1, 2);

    // 获取标定状态按钮
    auto *calibrationStatusBtn = new DelayedButton("获取标定状态", this);

    // 结果显示框
    m_calStatusResultEdit->setPlaceholderText("结果或错误信息显示");
    m_calStatusResultEdit->setReadOnly(true);

    // 从第四行开始
    m_gridLayout->addWidget(calibrationStatusBtn, 4, 0, 1, 1);
    m_gridLayout->addWidget(m_calStatusResultEdit, 4, 1, 1, 2);
}

void OperateR32Widget::initReadParamCmdUI()
{
    // 读取零点电阻按钮
    auto *readZeroResistanceBtn = new DelayedButton("读取零点电阻", this);
    m_readZeroResResultEdit->setPlaceholderText("零点电阻值");
    m_readZeroResResultEdit->setReadOnly(true);

    m_gridLayout->addWidget(readZeroResistanceBtn, 0, 3, 1, 1);
    m_gridLayout->addWidget(m_readZeroResResultEdit, 0, 4, 1, 1);

    // 读取标定参数p1按钮
    auto *readCalParamP1Btn = new DelayedButton("读取标定参数P1", this);
    m_readCalP1ResultEdit->setPlaceholderText("标定参数P1");
    m_readCalP1ResultEdit->setReadOnly(true);

    m_gridLayout->addWidget(readCalParamP1Btn, 1, 3, 1, 1);
    m_gridLayout->addWidget(m_readCalP1ResultEdit, 1, 4, 1, 1);

    // 读取标定参数p2按钮
    auto *readCalParamP2Btn = new DelayedButton("读取标定参数P2", this);
    m_readCalP2ResultEdit->setPlaceholderText("标定参数P2");
    m_readCalP2ResultEdit->setReadOnly(true);

    m_gridLayout->addWidget(readCalParamP2Btn, 2, 3, 1, 1);
    m_gridLayout->addWidget(m_readCalP2ResultEdit, 2, 4, 1, 1);

    // 读取标定参数p3按钮
    auto *readCalParamP3Btn = new DelayedButton("读取标定参数P3", this);
    m_readCalP3ResultEdit->setPlaceholderText("1000ppm的Rs/R0");
    m_readCalP3ResultEdit->setReadOnly(true);

    m_gridLayout->addWidget(readCalParamP3Btn, 3, 3, 1, 1);
    m_gridLayout->addWidget(m_readCalP3ResultEdit, 3, 4, 1, 1);

    // 读取标定参数p4按钮
    auto *readCalParamP4Btn = new DelayedButton("读取标定参数P4", this);
    m_readCalP4ResultEdit->setPlaceholderText("5000ppm的Rs/R0");
    m_readCalP4ResultEdit->setReadOnly(true);

    m_gridLayout->addWidget(readCalParamP4Btn, 4, 3, 1, 1);
    m_gridLayout->addWidget(m_readCalP4ResultEdit, 4, 4, 1, 1);
}

void OperateR32Widget::initDetectionCmdUI()
{
    // 获取温湿度按钮
    auto *getTempHumBtn = new DelayedButton("获取温湿度", this);
    m_curTempResultEdit->setPlaceholderText("当前温度");
    m_curTempResultEdit->setReadOnly(true);
    m_curHumidityResultEdit->setPlaceholderText("当前湿度");
    m_curHumidityResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(getTempHumBtn, 0, 5, 1, 1);
    m_gridLayout->addWidget(m_curTempResultEdit, 0, 6, 1, 1);
    m_gridLayout->addWidget(m_curHumidityResultEdit, 0, 7, 1, 1);

    // 获取气体ADC值按钮
    auto *getGasAdcBtn = new DelayedButton("获取气体ADC", this);
    m_curGasADCResultEdit->setPlaceholderText("当前气体ADC值");
    m_curGasADCResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(getGasAdcBtn, 1, 5, 1, 1);
    m_gridLayout->addWidget(m_curGasADCResultEdit, 1, 6, 1, 2);

    // 获取气体阻值按钮
    auto *getGasResistanceBtn = new DelayedButton("获取气体阻值", this);
    m_gasResResultEdit->setPlaceholderText("当前气体探头的阻值");
    m_gasResResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(getGasResistanceBtn, 2, 5, 1, 1);
    m_gridLayout->addWidget(m_gasResResultEdit, 2, 6, 1, 2);

    // 获取补偿后的阻值按钮
    auto *getCompensatedResistanceBtn = new DelayedButton("获取补偿后的阻值", this);
    m_compensateResResultEdit->setPlaceholderText("温度补偿后的气体探头阻值");
    m_compensateResResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(getCompensatedResistanceBtn, 3, 5, 1, 1);
    m_gridLayout->addWidget(m_compensateResResultEdit, 3, 6, 1, 2);

    // 获取浓度和报警按钮
    auto *getConcentrationAndAlarmBtn = new DelayedButton("获取浓度和报警", this);
    m_concentrationResultEdit->setPlaceholderText("当前浓度");
    m_concentrationResultEdit->setReadOnly(true);
    m_alarmStatusResultEdit->setPlaceholderText("报警状态");
    m_alarmStatusResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(getConcentrationAndAlarmBtn, 4, 5, 1, 1);
    m_gridLayout->addWidget(m_concentrationResultEdit, 4, 6, 1, 1);
    m_gridLayout->addWidget(m_alarmStatusResultEdit, 4, 7, 1, 1);
}

void OperateR32Widget::initSystemCmdUI()
{
    // 获取版本号按钮
    auto *getVersionBtn = new DelayedButton("获取版本号", this);
    m_mainVersionResultEdit->setPlaceholderText("主版本号");
    m_mainVersionResultEdit->setReadOnly(true);
    m_subVersionResultEdit->setPlaceholderText("副版本号");
    m_subVersionResultEdit->setReadOnly(true);

    m_gridLayout->addWidget(getVersionBtn, 0, 8, 1, 1);
    m_gridLayout->addWidget(m_mainVersionResultEdit, 0, 9, 1, 1);
    m_gridLayout->addWidget(m_subVersionResultEdit, 0, 10, 1, 1);

    // 设置报警阈值按钮
    auto *setAlarmThresholdBtn = new DelayedButton("设置报警阈值", this);
    // 输入报警阈值
    auto *alarmThresholdInput = new QLineEdit(this);
    m_setAlarmThresholdResultEdit->setPlaceholderText("设置报警阈值结果");
    m_setAlarmThresholdResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(setAlarmThresholdBtn, 1, 8, 1, 1);
    m_gridLayout->addWidget(alarmThresholdInput, 1, 9, 1, 1);
    m_gridLayout->addWidget(m_setAlarmThresholdResultEdit, 1, 10, 1, 1);

    // 读取报警阈值按钮
    auto *readAlarmThresholdBtn = new DelayedButton("读取报警阈值", this);
    m_readAlarmThresholdResultEdit->setPlaceholderText("读取报警阈值结果");
    m_readAlarmThresholdResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(readAlarmThresholdBtn, 2, 8, 1, 1);
    m_gridLayout->addWidget(m_readAlarmThresholdResultEdit, 2, 9, 1, 2);

    // 读取故障码按钮
    auto *readFaultCodeBtn = new DelayedButton("读取故障码", this);
    m_readFaultCodeResultEdit->setPlaceholderText("读取故障码结果");
    m_readFaultCodeResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(readFaultCodeBtn, 3, 8, 1, 1);
    m_gridLayout->addWidget(m_readFaultCodeResultEdit, 3, 9, 1, 2);

    // 打印数据开关
    auto *printDataSwitchBtn = new DelayedButton("打印数据开关", this);
    m_printDataSwitchResultEdit->setPlaceholderText("打印数据开关结果");
    m_printDataSwitchResultEdit->setReadOnly(true);
    m_gridLayout->addWidget(printDataSwitchBtn, 4, 8, 1, 1);
    m_gridLayout->addWidget(m_printDataSwitchResultEdit, 4, 9, 1, 2);
}

void OperateR32Widget::initBroadcastCmdUI()
{
    // 设置模块地址按钮
    auto *setModuleAddressBtn = new DelayedButton("设置模块地址", this);
    // 输入模块地址
    auto *moduleAddressInput = new QLineEdit(this);
    moduleAddressInput->setMaximumWidth(80);
    // 输入模块地址结果
    m_setModuleAddressResultEdit->setPlaceholderText("设置模块地址结果");
    m_setModuleAddressResultEdit->setReadOnly(true);

    // 读取模块地址按钮
    auto *readModuleAddressBtn = new DelayedButton("读取模块地址", this);
    m_readModuleAddressResultEdit->setPlaceholderText("读取模块地址结果");
    m_readModuleAddressResultEdit->setReadOnly(true);

    auto *hLayout = new QHBoxLayout;
    hLayout->addWidget(setModuleAddressBtn);
    hLayout->addWidget(moduleAddressInput);
    hLayout->addWidget(m_setModuleAddressResultEdit);
    hLayout->addStretch(1);
    hLayout->addWidget(readModuleAddressBtn);
    hLayout->addWidget(m_readModuleAddressResultEdit);
    hLayout->addStretch(2);

    m_mainLayout->addLayout(hLayout);
}

void OperateR32Widget::recvAckData(quint8 cmd, const QVariantMap &info)
{

}
