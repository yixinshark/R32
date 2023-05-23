//
// Created by zyz on 23-5-11.
//

#include "handledata.h"
#include "constant.h"

#include <QDebug>
#include <QVariantMap>

HandleData::HandleData(QObject *parent)
    : QObject(parent)
{
     m_readFuncMap = {
            {READ_ERROR_CMD, &HandleData::readErrorAck},
            {NTC_CMD, &HandleData::read_ntc},
            {R32_CMD, &HandleData::read_r32},
            {VER_CMD, &HandleData::readSoftwareVersion},
            {READ_PRODUCT_ID_CMD, &HandleData::readProductInfo},
            {READ_PRODUCT_ADDR_CMD, &HandleData::readSlaveProductAddress},
            {LD_CMD, &HandleData::readOperateResult},
            {ND_CMD, &HandleData::readOperateResult},
            {SET_ID_CMD, &HandleData::readOperateResult},
            {SET_SLAVE_ADDR_CMD, &HandleData::readOperateResult}
    };
}

HandleData::~HandleData()
{

}

QByteArray HandleData::getSendData(int cmd, const QVariantMap &info)
{
    QByteArray data;
    addContent(cmd, info, data);

    return data;
}

void HandleData::processReceivedData(const QByteArray &data)
{
    m_receivedData.append(data);

    while (m_receivedData.length() >= 6) // 至少包含一个完整帧的数据
    {
        // 检查数据头是否匹配
        if (m_receivedData.at(0) != header0 || m_receivedData.at(1) != header1) {
            // 数据头不匹配，丢弃第一个字节
            m_receivedData.remove(0, 1);
            continue;
        }

        // 提取长度
        int length = static_cast<unsigned char>(m_receivedData.at(2));

        if (m_receivedData.length() < length) // 数据不完整，等待更多数据
            break;

        // 提取完整帧的数据
        QByteArray frameData = m_receivedData.mid(0, length);
        m_receivedData.remove(0, length);

        // 提取命令号和其他内容
        quint8 command = static_cast<unsigned char>(frameData.at(3));
        // 4：头部，2：长度，1：命令号1； 6：4+校验和2
        QByteArray otherData = frameData.mid(4, length - 6);

        if (!frameIsValid(frameData)) {
            qWarning() << "recv frame is valid: " << frameData.toHex();
            // 校验和错误，丢弃该帧数据
            continue;
        }

        Q_EMIT recvedFrameData(frameData);

        // 根据命令号调用相应的处理函数
        if (m_readFuncMap.contains(command)) {
            QVariantMap value;
            if ((this->*m_readFuncMap[command])(otherData, value)) {
                // 发送信号，通知解析完成的帧数据
                Q_EMIT frameReceived(command, value);
            } else {
                qWarning() << "cmd:" << command << " read data error:" << otherData;
            }
        }
        else {
            qWarning() << "unknown command:" << command;
        }
    }
}

bool HandleData::frameIsValid(const QByteArray &frameData) {
    // 提取校验和
    QByteArray checksumBytes = frameData.right(2);
    quint16 checksum = (static_cast<quint8>(checksumBytes.at(0)) << 8) | static_cast<quint8>(checksumBytes.at(1));

    // 校验校验和
    quint16 calculatedChecksum = 0;
    for (int i = 0; i < frameData.length() - 2; ++i) {
        calculatedChecksum += static_cast<quint8>(frameData.at(i));
    }

    if (calculatedChecksum != checksum) {
        // 校验和错误，丢弃该帧数据
        return false;
    }

    return true;
}

void HandleData::addContent(int cmd, const QVariantMap &info, QByteArray &data)
{
    data.append(header0);
    data.append(header1);
    data.append(static_cast<quint8>(0)); // 先用0占位，后面再填充
    data.append(m_address);
    data.append(static_cast<quint8>(cmd));

    switch (cmd) {
        case ND_CMD:
            if (!addCmd_nd(info, data))
                return;
            break;
        case SET_ID_CMD:
            if (!addCmd_set_id(info, data))
                return;
            break;
        case SET_SLAVE_ADDR_CMD:
            if (!addCmd_set_slave_address(info, data))
                return;
            break;
        default:
            break;
    }

    // 填充长度，+2为校验和2字节
    data[2] = static_cast<quint8>(data.length() + 2);

    addCheckSum(data);
}

void HandleData::addCheckSum(QByteArray &data)
{
    quint16 checksum = 0;
    for (int i = 0; i < data.length(); ++i)
        checksum += static_cast<quint8>(data.at(i));

    data.append(static_cast<char>((checksum >> 8) & 0xFF));
    data.append(static_cast<char>(checksum & 0xFF));
}

bool HandleData::addCmd_nd(const QVariantMap &info, QByteArray &data)
{
    // 浓度
    if (info.contains(CONCENTRATION)) {
        int concentration = info.value(CONCENTRATION).toInt();
        // 添加浓度的高8位和低8位
        data.append(static_cast<char>((concentration >> 8) & 0xFF));
        data.append(static_cast<char>(concentration & 0xFF));
    } else {
        qWarning() << "concentration not found";
        return false;
    }

    // 温度
    if (info.contains(TEMPERATURE)) {
        int temperature = info.value(TEMPERATURE).toDouble() * 10;

        // 添加符号位
        data.append(temperature >= 0 ? 0x00 : 0x01);

        // 添加温度的高8位和低8位
        data.append(static_cast<char>((temperature >> 8) & 0xFF));
        data.append(static_cast<char>(temperature & 0xFF));
    } else {
        qWarning() << "temperature not found";
        return false;
    }

    return true;
}

bool HandleData::addCmd_set_id(const QVariantMap &info, QByteArray &data)
{
    // 添加产品种类
    if (info.contains(PRODUCT_TYPE)) {
        //int type = info.value(PRODUCT_TYPE).toInt();
        data.append(static_cast<unsigned char>(info.value(PRODUCT_TYPE).toInt()));
    } else {
        qWarning() << "product_type not found";
        return false;
    }

    // TODO
    if (info.contains(PRODUCT_ID)) {
        int id = info.value(PRODUCT_ID).toInt();
        // 分别添加id的第一个字节，第二个字节，第三个字节，第四个字节
        data.append(static_cast<char>((id >> 24) & 0xFF));
        data.append(static_cast<char>((id >> 16) & 0xFF));
        data.append(static_cast<char>((id >> 8) & 0xFF));
        data.append(static_cast<char>(id & 0xFF));
    } else {
        qWarning() << "product_id not found";
        return false;
    }

    return true;
}

bool HandleData::read_ntc(const QByteArray &data, QVariantMap &value)
{
    // NTC的adc值和温度一共五个字节；
    if (data.length() != 5) {
        qWarning() << "ntc data length error";
        return false;
    }

    // 前两个字节分别为adc的高8位和低8位，
    int adc = (static_cast<quint8>(data.at(0)) << 8) | static_cast<quint8>(data.at(1));
    value.insert(ADC_VALUE, adc);

    // 后三个字节分别为温度的符号位(0x00:代表正数0x01:代表负数)、温度乘以10的高8位和温度乘以10的低8位
    int temp = (static_cast<quint8>(data.at(2)) << 16) |
            (static_cast<quint8>(data.at(3)) << 8) |
            static_cast<quint8>(data.at(4));
    double temperature = temp / 10.0;

    switch (data[2]) {
        case 0x00:
            value.insert(TEMPERATURE, QString::number(temperature));
            break;
        case 0x01:
            value.insert(TEMPERATURE, QString::number(-temperature));
            break;
        case 0x02:
            value.insert(TEMPERATURE, NTC_STATUS_2);
            break;
        case 0x03:
            value.insert(TEMPERATURE, NTC_STATUS_3);
            break;
    }

    return true;
}

bool HandleData::read_r32(const QByteArray &data, QVariantMap &value)
{
    // R32的adc值和浓度一共4个字节；
    if (data.length() != 4) {
        qWarning() << "r32 data length error";
        return false;
    }

    // 前两个字节分别为adc的高8位和低8位
    int adc = (static_cast<quint8>(data.at(0)) << 8) | static_cast<quint8>(data.at(1));
    value.insert(ADC_VALUE, adc);

    // 后两个字节分别为浓度的高8位和低8位
    int concentration = (static_cast<quint8>(data.at(2)) << 8) | static_cast<quint8>(data.at(3));
    value.insert(CONCENTRATION, concentration);

    return true;
}

bool HandleData::readSoftwareVersion(const QByteArray &data, QVariantMap &value)
{
    // 软件版本号一共4个字节；
    if (data.length() != 4) {
        qWarning() << "software version data length error";
        return false;
    }

    // TODO 确认格式
    // 4个字节分别为软件版本号的第一个字节，第二个字节，第三个字节，第四个字节
    QString version = QString::number(static_cast<quint8>(data.at(0))) + "." +
                      QString::number(static_cast<quint8>(data.at(1))) + "." +
                      QString::number(static_cast<quint8>(data.at(2))) + "." +
                      QString::number(static_cast<quint8>(data.at(3)));
    value.insert(SOFTWARE_VERSION, version);

    return true;
}

bool HandleData::readProductInfo(const QByteArray &data, QVariantMap &value)
{
    // 一共5个字节，第一个字节为产品种类，后四个字节为产品id
    if (data.length() != 5) {
        qWarning() << "product info data length error";
        return false;
    }

    // 第一个字节为产品种类
    int type = static_cast<quint8>(data.at(0));
    value.insert(PRODUCT_TYPE, type);

    // TODO 产品id是整形还是字符串
    // 后四个字节为产品id
    int id = (static_cast<quint8>(data.at(1)) << 24) | (static_cast<quint8>(data.at(2)) << 16) |
            (static_cast<quint8>(data.at(3)) << 8) | static_cast<quint8>(data.at(4));
    value.insert(PRODUCT_ID, id);

    return true;
}

bool HandleData::readErrorAck(const QByteArray &data, QVariantMap &value)
{
    // 只有一个字节，代表错误码
    if (data.length() != 1) {
        qWarning() << "error ack data length error";
        return false;
    }

    int error = static_cast<quint8>(data.at(0));
    value.insert(ERROR_MSG, errorMsg.value(error, ERROR_MSG_UNKNOWN));

    return true;
}

bool HandleData::addCmd_set_slave_address(const QVariantMap &info, QByteArray &data)
{
    if (!info.contains(SLAVE_ADDRESS)|| !info.contains(STATIC_ADDRESS)) {
        qWarning() << "set product slave address info error:" << info;
        return false;
    }

    data.append(static_cast<unsigned char>(info.value(SLAVE_ADDRESS).toInt()));

    // 固定地址
    data[3] = (static_cast<unsigned char>(info.value(STATIC_ADDRESS).toInt()));

    return true;
}

bool HandleData::readSlaveProductAddress(const QByteArray &data, QVariantMap &value)
{
    // 只有一个字节，代表错误码
    if (data.length() != 1) {
        qWarning() << "error read-product-slave-address data length error";
        return false;
    }

    value.insert(SLAVE_ADDRESS, data[0]);

    return true;
}

bool HandleData::readOperateResult(const QByteArray &data, QVariantMap &value)
{
    // 没有有效字节
    value.insert(OPT_RESULT, true);
    return true;
}

void HandleData::setSlaveAddress(quint8 slaveAddress)
{
    m_address = slaveAddress;
}
