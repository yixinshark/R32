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

}

SerialPortCom::~SerialPortCom()
{
    closeSerialPort();
}

bool SerialPortCom::openSerialPort(const QString &portName, qint32 baudRate)
{
    if (m_serialPort->isOpen())
        return true;

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    if (m_serialPort->open(QIODevice::ReadWrite))
    {
        connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortCom::readData);
        return true;
    }

    return false;
    qWarning() << "open serial port failed:" << portName;
}

void SerialPortCom::closeSerialPort()
{
    m_serialPort->close();
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
    if (m_serialPort->isOpen()) {
        QByteArray data = m_serialPort->readAll();
        qInfo() << "read data count:" << data.size() << "data:" << data.toHex();
        emit dataReceived(data);
    }
}