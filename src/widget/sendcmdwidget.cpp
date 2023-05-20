//
// Created by zyz on 23-5-13.
//

#include "sendcmdwidget.h"
#include "../serialport/handledata.h"
#include "../serialport/constant.h"
#include "../serialport/serialportcom.h"
#include "delayedbutton.h"

#include <QDebug>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>

SendCmdWidget::SendCmdWidget(QWidget *parent)
    : OperateBaseWidget(parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_inputSlaveAddress(new QLineEdit(this))
    , m_inputSlaveAddressResult(new QLineEdit(this))
    , m_ndInput(new QLineEdit(this))
    , m_temperatureInput(new QLineEdit(this))
    , m_softwareLabel(new QLineEdit(this))
    , m_showSetLDResult(new QLineEdit(this))
    , m_showSetNDResult(new QLineEdit(this))
    , m_showSetProductIDResult(new QLineEdit(this))
{
    initUI();

    connect(m_handleData, &HandleData::frameReceived, this, &SendCmdWidget::recvAckData);
    connect(this, &SendCmdWidget::ntcTemperatureValue, this, &SendCmdWidget::setNTCTemperature);
}

SendCmdWidget::~SendCmdWidget()
{

}

void SendCmdWidget::initUI()
{
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(20);

    auto *layout = initSerialPortUI();
    m_mainLayout->addLayout(layout);

    initSlaveAddressUI();
    //initReadSlaveAddressUI();
    initSetProductIDUI();
    //initReadProductIDUI();
    initReadSoftVersionUI();
    initSetLDUI();
    initSetNDUI();
    m_mainLayout->addLayout(initReadNTCInfoUI());
    m_mainLayout->addLayout(initReadR32InfoUI("R32采样"));
}

void SendCmdWidget::recvAckData(int cmd, const QVariantMap &info)
{
    OperateBaseWidget::recvAckData(cmd, info);

    qInfo() << Q_FUNC_INFO << cmd << info;
    switch (cmd) {
        case READ_PRODUCT_ADDR_CMD:
            showProductSlaveAddress(info);
            break;
        case READ_PRODUCT_ID_CMD:
            showProductID(info);
            break;
        case VER_CMD:
            showSoftwareVersion(info);
            break;
        case LD_CMD:
            showOperateResult(info, m_showSetLDResult);
            break;
        case ND_CMD:
            showOperateResult(info, m_showSetNDResult);
            break;
        case SET_ID_CMD:
            showOperateResult(info, m_showSetProductIDResult);
            break;
        default:
            break;
    }
}

void SendCmdWidget::initSlaveAddressUI()
{
    auto *btn = new DelayedButton("设置从机地址", this);
    m_inputSlaveAddressResult->setPlaceholderText("显示操作结果");
    btn->setObjectName(CMD_FD_OBJECT_NAME);
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

        sendDataBtnClicked(info);
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

        sendDataBtnClicked(info);
    });

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(m_slaveAddressLabel);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showProductSlaveAddress(const QVariantMap &info)
{
    if (!info.contains(SLAVE_ADDRESS)) {
        qWarning() << "show-product-slave-address info error:" << info;
        return;
    }

    int value = static_cast<quint8>(info.value(SLAVE_ADDRESS).toInt());
    m_slaveAddressLabel->setText(QString("0x%1").arg(QString::number(value, 16)));
}

void SendCmdWidget::initReadProductIDUI()
{
    m_productIDLabel = new QLineEdit(this);
    m_productIDLabel->setReadOnly(true);
    auto *btn = new DelayedButton("读取产品ID", this);
    btn->setFixedWidth(120);
    btn->setObjectName(CMD7_OBJECT_NAME);
    m_delayBtnList.append(btn);
    btn->installEventFilter(this);
    connect(btn, &QPushButton::clicked, this, [this]{
        m_productIDLabel->clear();
        sendDataBtnClicked();
    });

    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(m_productIDLabel);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showProductID(const QVariantMap &info)
{
    if (!info.contains(PRODUCT_ID) || !info.contains(PRODUCT_TYPE)) {
        qWarning() << "show-product-id info error:" << info;
        return;
    }

    unsigned char productType = static_cast<quint8>(info.value(PRODUCT_TYPE).toInt());
    int value = info.value(PRODUCT_ID).toInt();

    QString msg = "产品种类:" + QString::number(productType) + " 产品ID:" + QString::number(value);
    m_productIDLabel->setText(msg);
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
        sendDataBtnClicked();
    });

    auto *lable1 = new QLabel("软件版本号:", this);
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(lable1);
    hLayout->addWidget(m_softwareLabel);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showSoftwareVersion(const QVariantMap &info)
{
    if (!info.contains(SOFTWARE_VERSION)) {
        qWarning() << "show-software-version info error:" << info;
        return;
    }

    QString version = info.value(SOFTWARE_VERSION).toString();
    m_softwareLabel->setText(version);
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
        sendDataBtnClicked();
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
    btn->setObjectName(CMD2_OBJECT_NAME);
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
            sendDataBtnClicked(info);
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
    auto *idInput = new QLineEdit(this);
    connect(btn, &QPushButton::clicked, this, [this, idInput]{
        m_showSetProductIDResult->clear();

        QVariantMap info;
        // TODO id 的类型
        info.insert(PRODUCT_ID, idInput->text().toInt());
        sendDataBtnClicked(info);
    });

    auto *label1 = new QLabel("操作结果:", this);
    auto *hLayout = new QHBoxLayout();
    hLayout->addWidget(btn);
    hLayout->addWidget(label);
    hLayout->addWidget(idInput);
    hLayout->addWidget(label1);
    hLayout->addWidget(m_showSetProductIDResult);

    m_mainLayout->addLayout(hLayout);
}

void SendCmdWidget::showOperateResult(const QVariantMap &info, QLineEdit *resultEdit)
{
    if (!info.contains(OPT_RESULT)) {
        qWarning() << "show-operate result info error:" << info;
        return;
    }

    bool result = info.value(OPT_RESULT).toBool();
    QString strRes = result ? "操作成功!" : "操作失败!";
    resultEdit->setText(strRes);
}

void SendCmdWidget::setNDValue(quint16 nd)
{
    m_ndInput->setText(QString::number(nd));
}

void SendCmdWidget::setNTCTemperature(const QString &temperture)
{
    m_temperatureInput->setText(temperture);
}
