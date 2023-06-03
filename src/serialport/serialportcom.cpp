//
// Created by zyz on 23-5-11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SerialPortcom.h" resolved

#include "serialportcom.h"

#include <QDebug>
#include <QSerialPort>

SerialPortCom::SerialPortCom(QObject *parent)
    : QObject(parent)
    , m_serialPort(new QSerialPort(this))
{
    connect(m_serialPort, &QSerialPort::aboutToClose, this, &SerialPortCom::serialPortClosed);
}

SerialPortCom::~SerialPortCom()
{
    if (!m_serialPort->portName().isEmpty())
        closeSerialPort();
}

bool SerialPortCom::openSerialPort(const QString &portName, qint32 baudRate, QSerialPort::DataBits dataBits,
                                   QSerialPort::Parity parity, QSerialPort::StopBits stopBits)
{
    if (m_serialPort->isOpen())
        return true;

    m_serialPort->setPortName(portName);
    bool b = m_serialPort->setBaudRate(baudRate);
    b &= m_serialPort->setDataBits(dataBits);
    b &= m_serialPort->setParity(parity);
    b &= m_serialPort->setStopBits(stopBits);

    if (b && m_serialPort->open(QIODevice::ReadWrite)) {
        connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortCom::readData);
        qInfo() << "serialPort opened successfully:" << portName;

        connect(m_serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error){
            qWarning() << "serialPort:" << m_serialPort->portName() << " has error:" << error;
        });

        return true;
    }

    qWarning() << "open serialPort failed:" << portName << " error:" << m_serialPort->errorString() << " setAttr:" << b;
    return false;
}

void SerialPortCom::closeSerialPort()
{
    m_serialPort->close();
    qInfo() << "close serialPort :" << m_serialPort->portName();
}

bool SerialPortCom::isSerialPortOpen() const
{
    return m_serialPort->isOpen();
}

void SerialPortCom::sendData(const QByteArray &data)
{
    qint64 sendCount = m_serialPort->write(data);
    qInfo() << "send data count:" << sendCount << "data:" << data.toHex();
}

void SerialPortCom::sendData(const char *data, int size)
{
    qint64 sendCount = m_serialPort->write(data, size);
    qInfo() << "send data count:" << sendCount << "data:" << QByteArray(data, size).toHex();
}

void SerialPortCom::readData()
{
    if (m_serialPort->isOpen() && m_serialPort->bytesAvailable() > 0) {
        qInfo() << "-----readData-----:" << m_serialPort->bytesAvailable();
        QByteArray data = m_serialPort->readAll();
        qInfo() << "read data count:" << data.size() << "data:" << data.toHex();
        emit dataReceived(data);
    }
}