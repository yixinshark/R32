//
// Created by zyz on 23-5-11.
//

#ifndef R32_HANDLEDATA_H
#define R32_HANDLEDATA_H

#include <QObject>
#include <QVariantMap>
#include <QMap>

class SerialPortCom;

class HandleData : public QObject
{
    Q_OBJECT
public:
    explicit HandleData(QObject *parent = nullptr);
    ~HandleData() override;

    void setSlaveAddress(quint8 slaveAddress);
    QByteArray getSendData(int cmd, const QVariantMap &info);

signals:
    void frameReceived(quint8 cmd, const QVariantMap &info);
    void recvedFrameData(const QByteArray &frameData);

public slots:
    void processReceivedData(const QByteArray &data);

// handle send data
private:
    void addContent(int cmd, const QVariantMap &info, QByteArray &data);
    void addCheckSum(QByteArray &data);

    // 添加浓度标定命令的数据
    bool addCmd_nd(const QVariantMap &info, QByteArray &data);
    // 添加设置产品ID号的命令数据
    bool addCmd_set_id(const QVariantMap &info, QByteArray &data);
    bool addCmd_set_slave_address(const QVariantMap &info, QByteArray &data);

// handle recv data
private:
    bool frameIsValid(const QByteArray &frameData);

    // 读取含有错误Ack
    bool readErrorAck(const QByteArray &data, QVariantMap &value);
    // 读取NTC的ADC值和温度
    bool read_ntc(const QByteArray &data, QVariantMap &value);
    // 读取R32的ADC值和浓度
    bool read_r32(const QByteArray &data, QVariantMap &value);
    // 读取软件版本号
    bool readSoftwareVersion(const QByteArray &data, QVariantMap &value);
    // 读取产品ID号
    bool readProductInfo(const QByteArray &data, QVariantMap &value);
    // 读取产品从机地址
    bool readSlaveProductAddress(const QByteArray &data, QVariantMap &value);
    // 读取设置零点标定结果;读取设置零点标定结果
    bool readOperateResult(const QByteArray &data, QVariantMap &value);

private:
    unsigned char m_address = 0x00;
    QByteArray m_receivedData;

    typedef bool (HandleData::*readFunc)(const QByteArray &data, QVariantMap &value);
    QMap<quint8 , readFunc> m_readFuncMap;
};

#endif //R32_HANDLEDATA_H
