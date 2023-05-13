//
// Created by zyz on 23-5-13.
//

#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSerialPortInfo>

#include "serialportsettingswidget.h"

SerialPortSettingsWidget::SerialPortSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , m_portComboBox(new QComboBox(this))
    , m_baudRateComboBox(new QComboBox(this))
    , m_parityComboBox(new QComboBox(this))
    , m_dataBitsComboBox(new QComboBox(this))
    , m_stopBitsComboBox(new QComboBox(this))
{
    initUI();
}

SerialPortSettingsWidget::~SerialPortSettingsWidget()
{

}

void SerialPortSettingsWidget::initUI()
{
    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto *titleLabel = new QLabel("串口设置", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    auto *hLayout = new QHBoxLayout();

    // 串口号选择
    auto *portLabel = new QLabel("串口:", this);
    m_portComboBox->addItems(getAvailablePorts());
    hLayout->addWidget(portLabel);
    hLayout->addWidget(m_portComboBox);
//    hLayout->addStretch();

    // 波特率选择
    auto *baudRateLabel = new QLabel("波特率:", this);
    m_baudRateComboBox->addItem("9600");
    m_baudRateComboBox->addItem("115200");
    hLayout->addWidget(baudRateLabel);
    hLayout->addWidget(m_baudRateComboBox);

    // 校验位选择
    auto *parityLabel = new QLabel("校验位:");
    m_parityComboBox->addItem("None");
    m_parityComboBox->addItem("Even");
    m_parityComboBox->addItem("Odd");
    hLayout->addWidget(parityLabel);
    hLayout->addWidget(m_parityComboBox);

    // 数据位选择
    auto *dataBitsLabel = new QLabel("数据位:", this);
    m_dataBitsComboBox->addItem("8");
    m_dataBitsComboBox->addItem("7");
    m_dataBitsComboBox->addItem("6");
    hLayout->addWidget(dataBitsLabel);
    hLayout->addWidget(m_dataBitsComboBox);

    // 停止位选择
    auto *stopBitsLabel = new QLabel("停止位:", this);
    m_stopBitsComboBox->addItem("1");
    m_stopBitsComboBox->addItem("1.5");
    m_stopBitsComboBox->addItem("2");
    hLayout->addWidget(stopBitsLabel);
    hLayout->addWidget(m_stopBitsComboBox);

    mainLayout->addLayout(hLayout);
//    mainLayout->addStretch();

    setLayout(mainLayout);
}

QStringList SerialPortSettingsWidget::getAvailablePorts() const
{
    QStringList ports;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ports.append(info.portName());
    }

    return ports;
}

QString SerialPortSettingsWidget::getSelectedPort() const
{
    return m_portComboBox->currentText();
}

qint32 SerialPortSettingsWidget::getSelectedBaudRate() const
{
    return m_baudRateComboBox->currentText().toInt();
}

QString SerialPortSettingsWidget::getSelectedParity() const
{
    return m_parityComboBox->currentText();
}

int SerialPortSettingsWidget::getSelectedDataBits() const
{
    return m_dataBitsComboBox->currentText().toInt();
}

QString SerialPortSettingsWidget::getSelectedStopBits() const
{
    return m_stopBitsComboBox->currentText();
}
