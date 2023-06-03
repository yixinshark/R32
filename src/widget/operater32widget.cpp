//
// Created by yixin on 2023/6/1.
//

#include "operater32widget.h"
#include "delayedbutton.h"
#include "r32constant.h"

#include <QDebug>
#include <QGridLayout>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>

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
    m_mainLayout->addLayout(initSerialPortUI());

    auto *widget = new QWidget(this);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widget->setLayout(m_gridLayout);

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

    m_mainLayout->addWidget(widget);
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
    calibrationBtn->installEventFilter(this);
    m_delayBtnList.append(calibrationBtn);
    connect(calibrationBtn, &DelayedButton::clicked, this, [pointEdit, concentrationEdit,this](){
        m_calibrationResultEdit->clear();
        // 校验点位
        if (pointEdit->text().isEmpty()) {
            m_calibrationResultEdit->setText("点位不能为空");
            return;
        }

        // 校验浓度
        if (concentrationEdit->text().isEmpty()) {
            m_calibrationResultEdit->setText("浓度不能为空");
            return;
        }

        QVariantMap info;
        info.insert(SEND_CAL_POINT, pointEdit->text().toInt());
        info.insert(SEND_CAL_CONCENTRATION, concentrationEdit->text().toUShort());

        sendCmdData(CMD_01, info);
    });

    m_gridLayout->addWidget(calibrationBtn, 0, 0, 2, 1);
    m_gridLayout->addWidget(pointEdit, 0, 1, 1, 1, Qt::AlignLeft);
    m_gridLayout->addWidget(concentrationEdit, 0, 2, 1, 1, Qt::AlignLeft);
    m_gridLayout->addWidget(m_calibrationResultEdit, 1, 1, 1, 2);

    // 标定完成按钮
    auto *calibrationFinishBtn = new DelayedButton("标定完成", this);
    calibrationFinishBtn->installEventFilter(this);
    m_delayBtnList.append(calibrationFinishBtn);
    // 结果显示框
    m_calFinishResultEdit->setPlaceholderText("结果或错误信息显示");
    m_calFinishResultEdit->setReadOnly(true);
    connect(calibrationFinishBtn, &DelayedButton::clicked, this, [this](){
        m_calFinishResultEdit->clear();
        sendCmdData(CMD_02);
    });
    // 从第三行开始
    m_gridLayout->addWidget(calibrationFinishBtn, 3, 0, 1, 1);
    m_gridLayout->addWidget(m_calFinishResultEdit, 3, 1, 1, 2);

    // 获取标定状态按钮
    auto *calibrationStatusBtn = new DelayedButton("获取标定状态", this);
    calibrationStatusBtn->installEventFilter(this);
    m_delayBtnList.append(calibrationStatusBtn);
    // 结果显示框
    m_calStatusResultEdit->setPlaceholderText("结果或错误信息显示");
    m_calStatusResultEdit->setReadOnly(true);
    connect(calibrationStatusBtn, &DelayedButton::clicked, this, [this](){
        m_calStatusResultEdit->clear();
        sendCmdData(CMD_ND_STATUS_03);
    });
    // 从第四行开始
    m_gridLayout->addWidget(calibrationStatusBtn, 4, 0, 1, 1);
    m_gridLayout->addWidget(m_calStatusResultEdit, 4, 1, 1, 2);
}

void OperateR32Widget::initReadParamCmdUI()
{
    // 读取零点电阻按钮
    auto *readZeroResistanceBtn = new DelayedButton("读取零点电阻", this);
    readZeroResistanceBtn->installEventFilter(this);
    m_delayBtnList.append(readZeroResistanceBtn);
    m_readZeroResResultEdit->setPlaceholderText("零点电阻值");
    m_readZeroResResultEdit->setReadOnly(true);
    connect(readZeroResistanceBtn, &DelayedButton::clicked, this, [this](){
        m_readZeroResResultEdit->clear();
        sendCmdData(CMD_READ_R0_04);
    });
    m_gridLayout->addWidget(readZeroResistanceBtn, 0, 3, 1, 1);
    m_gridLayout->addWidget(m_readZeroResResultEdit, 0, 4, 1, 1);

    // 读取标定参数p1按钮
    auto *readCalParamP1Btn = new DelayedButton("读取标定参数P1", this);
    readCalParamP1Btn->installEventFilter(this);
    m_delayBtnList.append(readCalParamP1Btn);
    m_readCalP1ResultEdit->setPlaceholderText("标定参数P1");
    m_readCalP1ResultEdit->setReadOnly(true);
    connect(readCalParamP1Btn, &DelayedButton::clicked, this, [this](){
        m_readCalP1ResultEdit->clear();
        sendCmdData(CMD_READ_PARAM1_05);
    });
    m_gridLayout->addWidget(readCalParamP1Btn, 1, 3, 1, 1);
    m_gridLayout->addWidget(m_readCalP1ResultEdit, 1, 4, 1, 1);

    // 读取标定参数p2按钮
    auto *readCalParamP2Btn = new DelayedButton("读取标定参数P2", this);
    readCalParamP2Btn->installEventFilter(this);
    m_delayBtnList.append(readCalParamP2Btn);
    m_readCalP2ResultEdit->setPlaceholderText("标定参数P2");
    m_readCalP2ResultEdit->setReadOnly(true);
    connect(readCalParamP2Btn, &DelayedButton::clicked, this, [this](){
        m_readCalP2ResultEdit->clear();
        sendCmdData(CMD_READ_PARAM2_06);
    });
    m_gridLayout->addWidget(readCalParamP2Btn, 2, 3, 1, 1);
    m_gridLayout->addWidget(m_readCalP2ResultEdit, 2, 4, 1, 1);

    // 读取标定参数p3按钮
    auto *readCalParamP3Btn = new DelayedButton("读取标定参数P3", this);
    readCalParamP3Btn->installEventFilter(this);
    m_delayBtnList.append(readCalParamP3Btn);
    m_readCalP3ResultEdit->setPlaceholderText("1000ppm的Rs/R0");
    m_readCalP3ResultEdit->setReadOnly(true);
    connect(readCalParamP3Btn, &DelayedButton::clicked, this, [this](){
        m_readCalP3ResultEdit->clear();
        sendCmdData(CMD_READ_1000PPM_07);
    });
    m_gridLayout->addWidget(readCalParamP3Btn, 3, 3, 1, 1);
    m_gridLayout->addWidget(m_readCalP3ResultEdit, 3, 4, 1, 1);

    // 读取标定参数p4按钮
    auto *readCalParamP4Btn = new DelayedButton("读取标定参数P4", this);
    readCalParamP4Btn->installEventFilter(this);
    m_delayBtnList.append(readCalParamP4Btn);
    m_readCalP4ResultEdit->setPlaceholderText("5000ppm的Rs/R0");
    m_readCalP4ResultEdit->setReadOnly(true);
    connect(readCalParamP4Btn, &DelayedButton::clicked, this, [this](){
        m_readCalP4ResultEdit->clear();
        sendCmdData(CMD_READ_5000PPM_08);
    });
    m_gridLayout->addWidget(readCalParamP4Btn, 4, 3, 1, 1);
    m_gridLayout->addWidget(m_readCalP4ResultEdit, 4, 4, 1, 1);
}

void OperateR32Widget::initDetectionCmdUI()
{
    // 获取温湿度按钮
    auto *getTempHumBtn = new DelayedButton("获取温湿度", this);
    getTempHumBtn->installEventFilter(this);
    m_delayBtnList.append(getTempHumBtn);
    m_curTempResultEdit->setPlaceholderText("当前温度");
    m_curTempResultEdit->setReadOnly(true);
    m_curHumidityResultEdit->setPlaceholderText("当前湿度");
    m_curHumidityResultEdit->setReadOnly(true);
    connect(getTempHumBtn, &DelayedButton::clicked, this, [this](){
        m_curTempResultEdit->clear();
        m_curHumidityResultEdit->clear();
        sendCmdData(CMD_READ_TEMP_HUM_21);
    });
    m_gridLayout->addWidget(getTempHumBtn, 0, 5, 1, 1);
    m_gridLayout->addWidget(m_curTempResultEdit, 0, 6, 1, 1);
    m_gridLayout->addWidget(m_curHumidityResultEdit, 0, 7, 1, 1);

    // 获取气体ADC值按钮
    auto *getGasAdcBtn = new DelayedButton("获取气体ADC", this);
    getGasAdcBtn->installEventFilter(this);
    m_delayBtnList.append(getGasAdcBtn);
    m_curGasADCResultEdit->setPlaceholderText("当前气体ADC值");
    m_curGasADCResultEdit->setReadOnly(true);
    connect(getGasAdcBtn, &DelayedButton::clicked, this, [this](){
        m_curGasADCResultEdit->clear();
        sendCmdData(CMD_READ_ADC_22);
    });
    m_gridLayout->addWidget(getGasAdcBtn, 1, 5, 1, 1);
    m_gridLayout->addWidget(m_curGasADCResultEdit, 1, 6, 1, 2);

    // 获取气体阻值按钮
    auto *getGasResistanceBtn = new DelayedButton("获取气体阻值", this);
    getGasResistanceBtn->installEventFilter(this);
    m_delayBtnList.append(getGasResistanceBtn);
    m_gasResResultEdit->setPlaceholderText("当前气体探头的阻值");
    m_gasResResultEdit->setReadOnly(true);
    connect(getGasResistanceBtn, &DelayedButton::clicked, this, [this](){
        m_gasResResultEdit->clear();
        sendCmdData(CMD_READ_RESISTANCE_23);
    });
    m_gridLayout->addWidget(getGasResistanceBtn, 2, 5, 1, 1);
    m_gridLayout->addWidget(m_gasResResultEdit, 2, 6, 1, 2);

    // 获取补偿后的阻值按钮
    auto *getCompensatedResistanceBtn = new DelayedButton("获取补偿后的阻值", this);
    getCompensatedResistanceBtn->installEventFilter(this);
    m_delayBtnList.append(getCompensatedResistanceBtn);
    m_compensateResResultEdit->setPlaceholderText("温度补偿后的气体探头阻值");
    m_compensateResResultEdit->setReadOnly(true);
    connect(getCompensatedResistanceBtn, &DelayedButton::clicked, this, [this](){
        m_compensateResResultEdit->clear();
        sendCmdData(CMD_READ_COMPENSATION_RESISTANCE_24);
    });
    m_gridLayout->addWidget(getCompensatedResistanceBtn, 3, 5, 1, 1);
    m_gridLayout->addWidget(m_compensateResResultEdit, 3, 6, 1, 2);

    // 获取浓度和报警按钮
    auto *getConcentrationAndAlarmBtn = new DelayedButton("获取浓度和报警", this);
    getConcentrationAndAlarmBtn->installEventFilter(this);
    m_delayBtnList.append(getConcentrationAndAlarmBtn);
    m_concentrationResultEdit->setPlaceholderText("当前浓度");
    m_concentrationResultEdit->setReadOnly(true);
    m_alarmStatusResultEdit->setPlaceholderText("报警状态");
    m_alarmStatusResultEdit->setReadOnly(true);
    connect(getConcentrationAndAlarmBtn, &DelayedButton::clicked, this, [this](){
        m_concentrationResultEdit->clear();
        m_alarmStatusResultEdit->clear();
        sendCmdData(CMD_READ_GAS_CONCENTRATION_25);
    });
    m_gridLayout->addWidget(getConcentrationAndAlarmBtn, 4, 5, 1, 1);
    m_gridLayout->addWidget(m_concentrationResultEdit, 4, 6, 1, 1);
    m_gridLayout->addWidget(m_alarmStatusResultEdit, 4, 7, 1, 1);
}

void OperateR32Widget::initSystemCmdUI()
{
    // 获取版本号按钮
    auto *getVersionBtn = new DelayedButton("获取版本号", this);
    getVersionBtn->installEventFilter(this);
    m_delayBtnList.append(getVersionBtn);
    m_mainVersionResultEdit->setPlaceholderText("主版本号");
    m_mainVersionResultEdit->setReadOnly(true);
    m_subVersionResultEdit->setPlaceholderText("副版本号");
    m_subVersionResultEdit->setReadOnly(true);
    connect(getVersionBtn, &DelayedButton::clicked, this, [this](){
        m_mainVersionResultEdit->clear();
        m_subVersionResultEdit->clear();
        sendCmdData(CMD_READ_FIRMWARE_VERSION_41);
    });
    m_gridLayout->addWidget(getVersionBtn, 0, 8, 1, 1);
    m_gridLayout->addWidget(m_mainVersionResultEdit, 0, 9, 1, 1);
    m_gridLayout->addWidget(m_subVersionResultEdit, 0, 10, 1, 1);

    // 设置报警阈值按钮
    auto *setAlarmThresholdBtn = new DelayedButton("设置报警阈值", this);
    setAlarmThresholdBtn->installEventFilter(this);
    m_delayBtnList.append(setAlarmThresholdBtn);
    // 输入报警阈值
    auto *alarmThresholdInput = new QLineEdit(this);
    m_setAlarmThresholdResultEdit->setPlaceholderText("设置报警阈值结果");
    m_setAlarmThresholdResultEdit->setReadOnly(true);
    connect(setAlarmThresholdBtn, &DelayedButton::clicked, this, [this, alarmThresholdInput](){
        m_setAlarmThresholdResultEdit->clear();
        if (alarmThresholdInput->text().isEmpty()) {
            m_setAlarmThresholdResultEdit->setText("请输入报警阈值");
            return;
        }

        auto alarmThreshold = alarmThresholdInput->text().toUShort();
        QVariantMap info;
        info.insert(SEND_ALARM_THRESHOLD, alarmThreshold);

        sendCmdData(CMD_SET_ALARM_THRESHOLD_44, info);
    });
    m_gridLayout->addWidget(setAlarmThresholdBtn, 1, 8, 1, 1);
    m_gridLayout->addWidget(alarmThresholdInput, 1, 9, 1, 1);
    m_gridLayout->addWidget(m_setAlarmThresholdResultEdit, 1, 10, 1, 1);

    // 读取报警阈值按钮
    auto *readAlarmThresholdBtn = new DelayedButton("读取报警阈值", this);
    readAlarmThresholdBtn->installEventFilter(this);
    m_delayBtnList.append(readAlarmThresholdBtn);
    m_readAlarmThresholdResultEdit->setPlaceholderText("读取报警阈值结果");
    m_readAlarmThresholdResultEdit->setReadOnly(true);
    connect(readAlarmThresholdBtn, &DelayedButton::clicked, this, [this](){
        m_readAlarmThresholdResultEdit->clear();
        sendCmdData(CMD_READ_ALARM_THRESHOLD_45);
    });
    m_gridLayout->addWidget(readAlarmThresholdBtn, 2, 8, 1, 1);
    m_gridLayout->addWidget(m_readAlarmThresholdResultEdit, 2, 9, 1, 2);

    // 读取故障码按钮
    auto *readFaultCodeBtn = new DelayedButton("读取故障码", this);
    readFaultCodeBtn->installEventFilter(this);
    m_delayBtnList.append(readFaultCodeBtn);
    m_readFaultCodeResultEdit->setPlaceholderText("读取故障码结果");
    m_readFaultCodeResultEdit->setReadOnly(true);
    connect(readFaultCodeBtn, &DelayedButton::clicked, this, [this](){
        m_readFaultCodeResultEdit->clear();
        sendCmdData(CMD_QUERY_FAULT_STATUS_43);
    });
    m_gridLayout->addWidget(readFaultCodeBtn, 3, 8, 1, 1);
    m_gridLayout->addWidget(m_readFaultCodeResultEdit, 3, 9, 1, 2);

    // 打印数据开关
    auto *printDataSwitchBtn = new DelayedButton("打印数据开关", this);
    printDataSwitchBtn->installEventFilter(this);
    m_delayBtnList.append(printDataSwitchBtn);
    m_printDataSwitchResultEdit->setPlaceholderText("打印数据开关结果");
    m_printDataSwitchResultEdit->setReadOnly(true);
    auto *openPrintDataBtn = new QRadioButton("开", this);
    auto *closePrintDataBtn = new QRadioButton("关", this);
    openPrintDataBtn->setChecked(true);
    auto *printDataBtnGroup = new QButtonGroup(this);
    printDataBtnGroup->addButton(closePrintDataBtn, 0);
    printDataBtnGroup->addButton(openPrintDataBtn, 1);
    auto *printDataBtnLayout = new QHBoxLayout;
    printDataBtnLayout->setMargin(10);
    printDataBtnLayout->addWidget(openPrintDataBtn);
    printDataBtnLayout->addWidget(closePrintDataBtn);
    connect(printDataSwitchBtn, &DelayedButton::clicked, this, [this, printDataBtnGroup](){
        m_printDataSwitchResultEdit->clear();
        QVariantMap info;
        info.insert(SEND_PRINT_ENABLE, printDataBtnGroup->checkedId());

        sendCmdData(CMD_OPEN_CLOSE_PRINT_42, info);
    });
    m_gridLayout->addWidget(printDataSwitchBtn, 4, 8, 1, 1);
    m_gridLayout->addLayout(printDataBtnLayout, 4, 9, 1, 1);
    m_gridLayout->addWidget(m_printDataSwitchResultEdit, 4, 10, 1, 1);
}

void OperateR32Widget::initBroadcastCmdUI()
{
    // 设置模块地址按钮
    auto *setModuleAddressBtn = new DelayedButton("设置模块地址", this);
    setModuleAddressBtn->installEventFilter(this);
    m_delayBtnList.append(setModuleAddressBtn);
    // 输入模块地址
    auto *moduleAddressInput = new QLineEdit(this);
    moduleAddressInput->setMaximumWidth(80);
    // 输入模块地址结果
    m_setModuleAddressResultEdit->setPlaceholderText("设置模块地址结果");
    m_setModuleAddressResultEdit->setReadOnly(true);

    connect(setModuleAddressBtn, &DelayedButton::clicked, this, [moduleAddressInput, this](){
        m_setModuleAddressResultEdit->clear();
        QString moduleAddress = moduleAddressInput->text();
        if (moduleAddress.isEmpty()) {
            m_setModuleAddressResultEdit->setText("请输入模块地址");
            return;
        }
        bool ok;
        quint8 address = moduleAddress.toUInt(&ok, 16);
        if (!ok) {
            m_setModuleAddressResultEdit->setText("请输入正确的模块地址");
            return;
        }

        QVariantMap info;
        info.insert(MODULE_ADDRESS, true);
        info.insert(SET_MODULE_ADDRESS, address);

        sendCmdData(CMD_01, info);
    });

    // 读取模块地址按钮
    auto *readModuleAddressBtn = new DelayedButton("读取模块地址", this);
    readModuleAddressBtn->installEventFilter(this);
    m_delayBtnList.append(readModuleAddressBtn);
    m_readModuleAddressResultEdit->setPlaceholderText("读取模块地址结果");
    m_readModuleAddressResultEdit->setReadOnly(true);
    connect(readModuleAddressBtn, &DelayedButton::clicked, this, [this](){
        m_readModuleAddressResultEdit->clear();
        QVariantMap info;
        info.insert(MODULE_ADDRESS, true);

        sendCmdData(CMD_02, info);
    });

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
    qInfo() << "OperateR32Widget::recvAckData" << cmd << info;
    switch (cmd) {
        case CMD_01:
            if (info.value(MODULE_ADDRESS).toBool()) {
                showOperationResult(info, m_setModuleAddressResultEdit);
            } else {
                showOperationResult(info, m_calibrationResultEdit);
            }
            break;
        case CMD_02:
            if (info.value(MODULE_ADDRESS).toBool()) {
                quint8 address = static_cast<quint8>(info.value(READ_MODULE_ADDRESS).toUInt());
                m_readModuleAddressResultEdit->setText("0x" + QString::number(address, 16));
            } else {
                showOperationResult(info, m_calFinishResultEdit);
            }
            break;
        case CMD_ND_STATUS_03:
            showOperationResult(info, m_calStatusResultEdit);
            break;
        case CMD_READ_R0_04:
            showOperationData(cmd, info, m_readZeroResResultEdit);
            break;
        case CMD_READ_PARAM1_05:
            showOperationData(cmd, info, m_readCalP1ResultEdit);
            break;
        case CMD_READ_PARAM2_06:
            showOperationData(cmd, info, m_readCalP2ResultEdit);
            break;
        case CMD_READ_1000PPM_07:
            showOperationData(cmd, info, m_readCalP3ResultEdit);
            break;
        case CMD_READ_5000PPM_08:
            showOperationData(cmd, info, m_readCalP4ResultEdit);
            break;
        case CMD_READ_TEMP_HUM_21:
            m_curTempResultEdit->setText(QString::number(info.value(ACK_TEMPERATURE).toFloat()));
            m_curHumidityResultEdit->setText(QString::number(info.value(ACK_HUMIDITY).toFloat()));
            break;
        case CMD_READ_ADC_22:
            m_curGasADCResultEdit->setText(QString::number(info.value(ACK_ADC_VALUE).toUInt()));
            break;
        case CMD_READ_RESISTANCE_23:
            showOperationData(cmd, info, m_gasResResultEdit);
            break;
        case CMD_READ_COMPENSATION_RESISTANCE_24:
            showOperationData(cmd, info, m_compensateResResultEdit);
            break;
        case CMD_READ_GAS_CONCENTRATION_25:
            m_concentrationResultEdit->setText(QString::number(info.value(ACK_GAS_CONCENTRATION).toUInt()));
            m_alarmStatusResultEdit->setText(info.value(ACK_ALARM_STATUS).toString());
            break;
        case CMD_READ_FIRMWARE_VERSION_41:
            m_mainVersionResultEdit->setText(QString::number(info.value(ACK_FIRMWARE_VERSION).toUInt()));
            m_subVersionResultEdit->setText(QString::number(info.value(ACK_FIRMWARE_SUB_VERSION).toUInt()));
            break;
        case CMD_OPEN_CLOSE_PRINT_42:
            showOperationResult(info, m_printDataSwitchResultEdit);
            break;
        case CMD_QUERY_FAULT_STATUS_43:
            m_readFaultCodeResultEdit->setText(info.value(ACK_ERROR).toString());
            break;
        case CMD_SET_ALARM_THRESHOLD_44:
            showOperationResult(info, m_setAlarmThresholdResultEdit);
            break;
        default:
            qWarning() << "unknown OperateR32Widget::recvAckData" << cmd << info;
            break;
    }
}

void OperateR32Widget::showOperationResult(const QVariantMap &info, QLineEdit *showEdit)
{
    if (info.value(ACK_RESULT).toString() == "成功") {
        showEdit->setText("操作成功");
    } else {
        showEdit->setText(info.value(ACK_ERROR).toString());
    }
}

void OperateR32Widget::showOperationData(char cmd, const QVariantMap &info, QLineEdit *showEdit)
{
    Q_UNUSED(cmd)
    float data = info.value(ACK_FLOAT_VALUE).toFloat();
    showEdit->setText(QString::number(data));
}
