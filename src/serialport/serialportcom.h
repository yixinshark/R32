//
// Created by zyz on 23-5-11.
//

#ifndef R32_SERIALPORTCOM_H
#define R32_SERIALPORTCOM_H

#include <QObject>
#include <QSerialPort>

class SerialPortCom : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortCom(QObject *parent = nullptr);
    ~SerialPortCom() override;

    bool openSerialPort(const QString &portName, qint32 baudRate, QSerialPort::DataBits dataBits,
                        QSerialPort::Parity parity, QSerialPort::StopBits stopBits);
    void closeSerialPort();
    bool isSerialPortOpen() const;

signals:
    void serialPortClosed();
    void dataReceived(const QByteArray &data);

public slots:
    void sendData(const QByteArray &data);
    void sendData(const char *data, int size);

private slots:
    void readData();

private:
    QSerialPort *m_serialPort = nullptr;
};


#endif //R32_SERIALPORTCOM_H
