//
// Created by zyz on 23-5-11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_HandleData.h" resolved

#include "handledata.h"
#include "constant.h"

#include <QDebug>
#include <QVariantMap>

HandleData::HandleData(QObject *parent)
    : QObject(parent)
{

}

HandleData::~HandleData()
{

}

void HandleData::sendData(int cmd, const QVariantMap &info)
{
    QByteArray data;
    addContent(cmd, info, data);
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
        int length = static_cast<int>(m_receivedData.at(2));

        if (m_receivedData.length() < length) // 数据不完整，等待更多数据
            break;

        // 提取完整帧的数据
        QByteArray frameData = m_receivedData.mid(0, length);
        m_receivedData.remove(0, length);

        // 提取命令号和其他内容
        int command = static_cast<int>(frameData.at(3));
        // 4：头部，2：长度，1：命令号1； 6：4+校验和2
        QByteArray otherData = frameData.mid(4, length - 6);

        if (!frameIsValid(frameData)) {
            // 校验和错误，丢弃该帧数据
            continue;
        }

        // 发送信号，通知解析完成的帧数据
        emit frameReceived(command, otherData);
    }
}

bool HandleData::frameIsValid(const QByteArray &frameData) {
    // 提取校验和
    QByteArray checksumBytes = frameData.right(2);
    quint16 checksum = (static_cast<quint8>(checksumBytes.at(1)) << 8) | static_cast<quint8>(checksumBytes.at(0));

    // 校验校验和
    quint16 calculatedChecksum = 0;
    for (int i = 0; i < frameData.length() - 2; ++i)
        calculatedChecksum += static_cast<quint8>(frameData.at(i));

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
    data.append(static_cast<char>(0)); // 先用0占位，后面再填充
    data.append(m_address);
    data.append(static_cast<char>(cmd));

    switch (cmd) {
        case cmd_nd:
            if (!addCmd_nd(info, data))
                return;
            break;
        case cmd_set_id:
            if (!addCmd_set_id(info, data))
                return;
            break;
        default:
            break;
    }

    // 填充长度，+2为校验和2字节
    data[2] = static_cast<char>(data.length() + 2);

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
    if (info.contains("concentration")) {
        int concentration = info.value("concentration").toInt();
        // 添加浓度的高8位和低8位
        data.append(static_cast<char>((concentration >> 8) & 0xFF));
        data.append(static_cast<char>(concentration & 0xFF));
    } else {
        qWarning() << "concentration not found";
        return false;
    }

    if (info.contains("temperature")) {
        int temperature = info.value("temperature").toDouble() * 10;

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
    if (info.contains("type")) {
        int type = info.value("type").toInt();
        data.append(static_cast<char>(type));
    }

    // TODO
    if (info.contains("id")) {
        int id = info.value("id").toInt();
        // 分别添加id的第一个字节，第二个字节，第三个字节，第四个字节

    } else {
        qWarning() << "id not found";
        return false;
    }

    return true;
}
