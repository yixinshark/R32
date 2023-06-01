//
// Created by zyz on 23-5-13.
//

#include "sendcmdwidget.h"
#include "handledatabase.h"
#include "constant.h"
#include "serialportcom.h"
#include "delayedbutton.h"
#include "stepswidget.h"
#include "handler32data.h"

#include <QDebug>
#include <QTimer>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>

SendCmdWidget::SendCmdWidget(HandleDataBase *handleData, QWidget *parent)
    : OperateBaseWidget(handleData, parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_stepsWidget(new StepsWidget(this))
    , m_inputSlaveAddress(new QLineEdit(this))
    , m_inputSlaveAddressResult(new QLineEdit(this))
    , m_setProductIDInput(new QLineEdit(this))
    , m_ndInput(new QLineEdit(this))
    , m_temperatureInput(new QLineEdit(this))
    , m_softwareLabel(new QLineEdit(this))
    , m_showSetLDResult(new QLineEdit(this))
    , m_showSetNDResult(new QLineEdit(this))
    , m_showSetProductIDResult(new QLineEdit(this))
    , m_timer(new QTimer(this))
{
//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    initUI();

    connect(m_handleData, &HandleDataBase::frameReceived, this, &SendCmdWidget::recvAckData);
    connect(this, &SendCmdWidget::ntcTemperatureValue, this, &SendCmdWidget::setNTCTemperature);
    connect(this, &SendCmdWidget::cmdCompleted, m_stepsWidget, &StepsWidget::stepComplete);
    connect(this, &SendCmdWidget::cmdCompleted, this, &SendCmdWidget::stepCompleted);
}

SendCmdWidget::~SendCmdWidget()
{

}

void SendCmdWidget::initUI()
{
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(10);

    auto *layout = initSerialPortUI();
    m_mainLayout->addLayout(layout);
    m_mainLayout->addWidget(m_stepsWidget);

    initSlaveAddressUI();
    //initReadSlaveAddressUI();
    initSetProductIDUI();
    //initReadProductIDUI();
    initReadSoftVersionUI();
    initSetLDUI();
    initReadNTCInfoUI();
    initSetNDUI();
    initReadR32InfoUI();
}

void SendCmdWidget::recvAckData(quint8 cmd, const QVariantMap &info)
{
    qInfo() << Q_FUNC_INFO << cmd << info;
    switch (cmd) {
        case NTC_CMD:
            showNTCInfo(cmd, info);
            break;
        case R32_CMD:
            showR32Info(cmd, info);
            break;
    }

    qInfo() << Q_FUNC_INFO << cmd << info;
    switch (cmd) {
        case READ_PRODUCT_ADDR_CMD:
            showProductSlaveAddress(cmd, info);
            break;
        case READ_PRODUCT_ID_CMD:
            showProductID(cmd, info);
            break;
        case VER_CMD:
            showSoftwareVersion(cmd, info);
            break;
        case LD_CMD:
            showOperateResult(cmd, info, m_showSetLDResult);
            break;
        case ND_CMD:
            showOperateResult(cmd, info, m_showSetNDResult);
            break;
        case SET_ID_CMD:
            showOperateResult(cmd, info, m_showSetProductIDResult);
            break;
        case SET_SLAVE_ADDR_CMD:
            showOperateResult(cmd, info, m_inputSlaveAddressResult);
            break;
        default:
            break;
    }
}

void SendCmdWidget::initSlaveAddressUI()
{
    auto *btn = new DelayedButton("设置从机地址", this);
    m_inputSlaveAddressResult->setPlaceholderText("显示操作结果");
    btn->setFixedWidth(120);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &QPushButton::clicked, this, [this]{
        QString slaveAddress = m_inputSlaveAddress->text();
        if (slaveAddress.isEmpty()) {
            return;
        }

        bool ok = false;
        int slaveAddressInt = slaveAddress.toInt(&ok, 16);
        if (!ok) {
            return;
        }

        QVariantMap info;
        info.insert(SLAVE_ADDRESS, slaveAddressInt);
        info.insert(STATIC_ADDRESS, static_address_value);
        qInfo() << "slave address:" << slaveAddressInt;

        sendCmdData(SET_SLAVE_ADDR_CMD, info);
    });

    auto *label = new QLabel("输入地址0X:", this);
    auto *label1 = new QLabel("操作结果:", this);
    m_inputSlaveAddressResult->setReadOnly(true);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(label);
    hLayout->addWidget(m_inputSlaveAddress);
    hLayout->addWidget(label1);
    hLayout->addWidget(m_inputSlaveAddressResult);
    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::initReadSlaveAddressUI()
{
    m_slaveAddressLabel = new QLineEdit(this);
    m_slaveAddressLabel->setReadOnly(true);
    auto *btn = new DelayedButton("读取从机地址", this);
    btn->setFixedWidth(120);
    btn->setObjectName(CMD_FE_OBJECT_NAME);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &QPushButton::clicked, this, [this]{
        m_slaveAddressLabel->clear();

        QVariantMap info;
        info.insert(STATIC_ADDRESS, static_address_value);

        sendCmdData(READ_PRODUCT_ADDR_CMD, info);
    });

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(m_slaveAddressLabel);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showProductSlaveAddress(quint8 cmd, const QVariantMap &info)
{
    if (!info.contains(SLAVE_ADDRESS)) {
        qWarning() << "show-product-slave-address info error:" << info;
        return;
    }

    int value = static_cast<quint8>(info.value(SLAVE_ADDRESS).toInt());
    m_slaveAddressLabel->setText(QString("0x%1").arg(QString::number(value, 16)));
    Q_EMIT cmdCompleted(cmd);
}

void SendCmdWidget::initReadProductIDUI()
{
    m_productIDLabel = new QLineEdit(this);
    m_productIDLabel->setReadOnly(true);
    auto *btn = new DelayedButton("读取产品ID", this);
    btn->setFixedWidth(120);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &QPushButton::clicked, this, [this]{
        m_productIDLabel->clear();
        sendCmdData(READ_PRODUCT_ID_CMD);
    });

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(m_productIDLabel);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showProductID(quint8 cmd, const QVariantMap &info)
{
    if (!info.contains(PRODUCT_ID) || !info.contains(PRODUCT_TYPE)) {
        qWarning() << "show-product-id info error:" << info;
        return;
    }

    unsigned char productType = static_cast<quint8>(info.value(PRODUCT_TYPE).toInt());
    int value = info.value(PRODUCT_ID).toInt();

    QString msg = "产品种类:" + QString::number(productType) + " 产品ID:" + QString::number(value);
    m_productIDLabel->setText(msg);
    Q_EMIT cmdCompleted(cmd);
}

void SendCmdWidget::initReadSoftVersionUI()
{
    m_softwareLabel->setReadOnly(true);
    m_softwareLabel->setPlaceholderText("显示软件版本号");
    auto *btn = new DelayedButton("读取软件版本号", this);
    btn->setFixedWidth(120);
    btn->setObjectName(CMD5_OBJECT_NAME);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &QPushButton::clicked, this, [this]{
        m_softwareLabel->clear();
        sendCmdData(VER_CMD);
    });

    auto *lable1 = new QLabel("软件版本号:", this);
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(lable1);
    hLayout->addWidget(m_softwareLabel);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showSoftwareVersion(quint8 cmd, const QVariantMap &info)
{
    if (!info.contains(SOFTWARE_VERSION)) {
        qWarning() << "show-software-version info error:" << info;
        return;
    }

    QString version = info.value(SOFTWARE_VERSION).toString();
    m_softwareLabel->setText(version);
    Q_EMIT cmdCompleted(cmd);
}

void SendCmdWidget::initSetLDUI()
{
    m_showSetLDResult->setReadOnly(true);
    m_showSetLDResult->setPlaceholderText("显示操作结果");
    auto *btn = new DelayedButton("零点标定", this);
    btn->setFixedWidth(120);
    btn->setObjectName(CMD1_OBJECT_NAME);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &QPushButton::clicked, this, [this]{
        m_showSetLDResult->clear();
        sendCmdData(LD_CMD);
    });

    auto *label = new QLabel("操作结果:", this);
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(label);
    hLayout->addWidget(m_showSetLDResult);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::initSetNDUI()
{
    m_showSetNDResult->setReadOnly(true);
    m_showSetNDResult->setPlaceholderText("显示操作结果");
    auto *btn = new DelayedButton("浓度标定", this);
    btn->setFixedWidth(120);
    btn->installEventFilter(this);
    m_delayBtnList.append(btn);

    auto *label = new QLabel("输入浓度值:", this);
    auto *label1 = new QLabel("输入温度值:", this);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(label);
    hLayout->addWidget(m_ndInput);
    hLayout->addWidget(label1);
    hLayout->addWidget(m_temperatureInput);
    hLayout->addWidget(m_showSetNDResult);

    m_mainLayout->addLayout(hLayout);

    connect(btn, &QPushButton::clicked, this, [this]{
        m_showSetNDResult->clear();

        qInfo() << "nd:" << m_ndInput->text() << " temperature:" << m_temperatureInput->text();

        bool ok = false;
        bool ok1 = false;
        QVariantMap info;
        info.insert(CONCENTRATION, static_cast<quint16>(m_ndInput->text().toInt(&ok)));
        info.insert(TEMPERATURE, m_temperatureInput->text().toDouble(&ok1));

        if (ok & ok1) {
            sendCmdData(ND_CMD, info);
        } else {
         m_showSetNDResult->setText("浓度值或者温度值输入有误,请重新输入!");
        }
    });
}

void SendCmdWidget::initSetProductIDUI()
{
    m_showSetProductIDResult->setReadOnly(true);
    m_showSetProductIDResult->setPlaceholderText("显示操作结果");
    auto *btn = new DelayedButton("设置产品ID号", this);
    btn->setFixedWidth(120);
    btn->setObjectName(CMD6_OBJECT_NAME);
    btn->installEventFilter(this);
    m_delayBtnList.append(btn);

    auto *label = new QLabel("输入ID:", this);
    connect(btn, &QPushButton::clicked, this, [this]{
        m_showSetProductIDResult->clear();

        QVariantMap info;
        // TODO id 的类型
        info.insert(PRODUCT_ID, m_setProductIDInput->text().toInt());
        sendCmdData(SET_ID_CMD, info);
    });

    auto *label1 = new QLabel("操作结果:", this);
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(label);
    hLayout->addWidget(m_setProductIDInput);
    hLayout->addWidget(label1);
    hLayout->addWidget(m_showSetProductIDResult);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showOperateResult(quint8 cmd, const QVariantMap &info, QLineEdit *resultEdit)
{
    if (!info.contains(OPT_RESULT)) {
        qWarning() << "show-operate result info error:" << info;
        return;
    }

    bool result = info.value(OPT_RESULT).toBool();
    QString strRes = result ? "操作成功!" : "操作失败!";
    resultEdit->setText(strRes);

    if (cmd == SET_SLAVE_ADDR_CMD && result) {
        auto handleData = qobject_cast<Handler32data *>(m_handleData);
        handleData->setSlaveAddress(static_cast<quint8>(m_inputSlaveAddress->text().toInt()));
    }

    Q_EMIT cmdCompleted(cmd);
}

void SendCmdWidget::setNDValue(quint16 nd)
{
    m_ndInput->setText(QString::number(nd));
}

void SendCmdWidget::setNTCTemperature(const QString &temperture)
{
    m_temperatureInput->setText(temperture);
}

// 步骤完成后，记录输入的数据
void SendCmdWidget::stepCompleted(quint8 cmd)
{
    switch (cmd) {
        case SET_ID_CMD:
            m_r32RecordValue.sensor_id = QString::number(m_setProductIDInput->text().toInt());
            break;
        case VER_CMD:
            m_r32RecordValue.software_version = m_softwareLabel->text();
            break;
        case ND_CMD:
            m_r32RecordValue.adc_value = m_showADCValue->text().toInt();
            m_r32RecordValue.concentration = m_showR32NDValue->text();
            break;
    }

    // 可以记录数据检测
    if (m_stepsWidget->canRecordData()) {
        // 生产流水号：系统时间+产品ID
        QString serialNumber = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + m_r32RecordValue.sensor_id;
        m_r32RecordValue.serial_number = serialNumber;
        m_r32RecordValue.dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        if (R32RecordValueDao::insertOrUpdate(m_r32RecordValue)) {
            qInfo() << "insert or update r32 record value success!";
            m_stepsWidget->stepComplete(RECORD_DATA);
        } else {
            qWarning() << "insert or update r32 record value failed!:" << m_r32RecordValue.toString();
        }
    }
}

void SendCmdWidget::sendCmdData(quint8 cmd, const QVariantMap &info)
{

}

void SendCmdWidget::initReadNTCInfoUI()
{
    m_showADCValue = new QLineEdit(this);
    m_showADCValue->setReadOnly(true);
    m_showADCValue->setPlaceholderText("显示ADC值");
    m_showTemperatureValue = new QLineEdit(this);
    m_showTemperatureValue->setReadOnly(true);
    m_showTemperatureValue->setPlaceholderText("显示温度值");

    auto *btn = new DelayedButton("读取NTC的ADC和温度", this);
    //btn->setFixedWidth(190);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &DelayedButton::clicked, this, [this]{
        m_showADCValue->clear();
        m_showTemperatureValue->clear();

        sendCmdData(NTC_CMD);
    });

    auto *label = new QLabel("ADC值:", this);
    auto *label1 = new QLabel("温度值:", this);

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(label);
    hLayout->addWidget(m_showADCValue);
    hLayout->addWidget(label1);
    hLayout->addWidget(m_showTemperatureValue);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::initReadR32InfoUI()
{
    m_showR32ADCValue = new QLineEdit(this);
    m_showR32ADCValue->setReadOnly(true);
    m_showR32ADCValue->setPlaceholderText("显示ADC值");
    m_showR32NDValue = new QLineEdit(this);
    m_showR32NDValue->setReadOnly(true);
    m_showR32NDValue->setPlaceholderText("显示浓度值");

    auto *btn = new DelayedButton("R32采样", this);
    //btn->setFixedWidth(190);
    btn->setObjectName(CMD4_OBJECT_NAME);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &DelayedButton::delayedClicked, this, [this]{
        m_showR32ADCValue->clear();
        m_showR32NDValue->clear();

        if (m_serialPortCom->isSerialPortOpen() && !m_timer->isActive()) {
            m_timer->start();
        }

        sendCmdData(R32_CMD);
    });

    m_timer->setSingleShot(false);
    // TODO 可配置
    m_timer->setInterval(1 * 1000); // 1s

    connect(m_timer, &QTimer::timeout, this, [btn, this]{
        if (!m_serialPortCom->isSerialPortOpen()) {
            m_timer->stop();
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
    hLayout->addWidget(label);
    hLayout->addWidget(m_showR32ADCValue);
    hLayout->addWidget(label1);
    hLayout->addWidget(m_showR32NDValue);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showNTCInfo(quint8 cmd, const QVariantMap &info)
{
    if (!info.contains(ADC_VALUE) || !info.contains(TEMPERATURE)) {
        qWarning() << "show-read-NTC-info error:" << info;
        return;
    }

    Q_EMIT ntcTemperatureValue(info[TEMPERATURE].toString());

    m_showADCValue->setText(info[ADC_VALUE].toString());
    m_showTemperatureValue->setText(info[TEMPERATURE].toString());
    Q_EMIT cmdCompleted(cmd);
}

void SendCmdWidget::showR32Info(quint8 cmd, const QVariantMap &info) {
    if (!info.contains(ADC_VALUE) || !info.contains(CONCENTRATION)) {
        qWarning() << "show-read-R32-info error:" << info;
        return;
    }

    Q_EMIT r32NDValue(info[CONCENTRATION].toInt());

    m_showR32ADCValue->setText(info[ADC_VALUE].toString());
    m_showR32NDValue->setText(info[CONCENTRATION].toString());
    Q_EMIT cmdCompleted(cmd);
}
