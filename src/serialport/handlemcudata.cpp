//
// Created by zyz on 2023/5/31.
//

#include "handlemcudata.h"
#include "mcuconstant.h"

#include <QDebug>

HandleMcuData::HandleMcuData(QObject *parent)
    : HandleDataBase(parent)
{
}

HandleMcuData::~HandleMcuData()
{

}

void HandleMcuData::processReceivedData(const QByteArray &data)
{
    m_receivedData.append(data);

    // 1. 判断帧数据是否有效
    while (m_receivedData.length() >= MCU_FRAME_LEN) {
        // 2. 判断帧头是否正确
        if (m_receivedData.at(0) != MCU_RECV_HEADER) {
            m_receivedData.remove(0, 1);
            continue;
        }

        // 3. 提取帧数据
        QByteArray frameData = m_receivedData.left(MCU_FRAME_LEN);
        m_receivedData.remove(0, MCU_FRAME_LEN);

        // 4. 判断帧数据是否有效
        if (!frameIsValid(frameData)) {
            qWarning() << "frame data is invalid:" << frameData;
            continue;
        }

        Q_EMIT recvedFrameData(frameData);

        // 5. 提取帧内容
        char cmd = frameData.at(1);
        QByteArray content = frameData.mid(2, MCU_CONTENT_LEN);

        QVariantMap info;
        info.insert(MCU_RECV_DATA, content);

        Q_EMIT frameReceived(cmd, info);
    }
}

void HandleMcuData::addContent(char cmd, const QVariantMap &info, QByteArray &data)
{
    data.append(MCU_SEND_HEADER);
    data.append(cmd);

    switch ((quint8)cmd) {
        case MCU_CMD_VALVE:
            addCmd_valve_Content(info, data);
            break;
        case MCU_CMD_FAN:
            addCmd_fan_Content(info, data);
            break;
        case MCU_CMD_ND:
            addCmd_nd_Content(info, data);
            break;
        case MCU_CMD_CHANNEL:
            addCmd_set_channel_Content(info, data);
            break;
        case MCU_CMD_BOX:
            addCmd_box_switch_Content(info, data);
            break;
        case MCU_CMD_ALARM_LIGHT:
            addCmd_alarm_light_Content(info, data);
            break;
    }

    addCheckSum(data);
}

bool HandleMcuData::addCmd_valve_Content(const QVariantMap &info, QByteArray &data)
{
    if (info.contains(MCU_VALVE)) {
        data.append(info[MCU_VALVE].toByteArray());
        return true;
    }

    qWarning() << "valve not exist:" << info;
    return false;
}

bool HandleMcuData::addCmd_fan_Content(const QVariantMap &info, QByteArray &data)
{
    if (info.contains(MCU_FAN)) {
        data.append(info[MCU_FAN].toByteArray());
        data.append((char)0x00); // 填充一个字节
        return true;
    }

    qWarning() << "fan not exist:" << info;
    return false;
}

bool HandleMcuData::addCmd_nd_Content(const QVariantMap &info, QByteArray &data)
{
    if (info.contains(MCU_ND)) {
        quint16 nd = static_cast<quint16>(info[MCU_ND].toUInt());
        data.append((char)(nd >> 8));
        data.append((char)(nd & 0xFF));
        data.append((char)0x00); // 填充一个字节
        data.append((char)0x00); // 填充一个字节
        data.append((char)0x00); // 填充一个字节
        return true;
    }

    qWarning() << "mcu nd not exist:" << info;
    return false;
}

bool HandleMcuData::addCmd_set_channel_Content(const QVariantMap &info, QByteArray &data)
{
    if (info.contains(MCU_SET_CHANNEL)) {
        char channel = static_cast<char>(info[MCU_SET_CHANNEL].toUInt());
        data.append(channel);
        data.append((char)0x00); // 填充一个字节
        data.append((char)0x00); // 填充一个字节
        data.append((char)0x00); // 填充一个字节
        data.append((char)0x00); // 填充一个字节

        return true;
    }

    qWarning() << "mcu set channel not exist:" << info;
    return false;
}

bool HandleMcuData::addCmd_box_switch_Content(const QVariantMap &info, QByteArray &data)
{
    if (info.contains(MCU_BOX_SWITCH)) {
        char box_switch = static_cast<char>(info[MCU_BOX_SWITCH].toUInt());
        data.append(box_switch);
        data.append((char)0x00); // 填充一个字节
        data.append((char)0x00); // 填充一个字节
        data.append((char)0x00); // 填充一个字节
        data.append((char)0x00); // 填充一个字节

        return true;
    }

    return false;
}

bool HandleMcuData::addCmd_alarm_light_Content(const QVariantMap &info, QByteArray &data)
{
    if (!info.contains(MCU_SET_CHANNEL) || !info.contains(MCU_ALARM_LIGHT)) {
        qWarning() << "mcu alarm light not exist:" << info;
        return false;
    }

    char channel = static_cast<char>(info[MCU_SET_CHANNEL].toUInt());
    char alarm_light = static_cast<char>(info[MCU_ALARM_LIGHT].toUInt());
    data.append(channel);
    data.append(alarm_light);

    data.append((char)0x00); // 填充一个字节
    data.append((char)0x00); // 填充一个字节
    data.append((char)0x00); // 填充一个字节

    return true;
}

bool HandleMcuData::frameIsValid(const QByteArray &frameData)
{
    // 最后两个字节和前面字节和比较
    int sum = 0;
    for (int i = 0; i < MCU_FRAME_LEN - 2; ++i) {
        sum += (quint8)frameData.at(i);
    }

    // 最后两个字节是校验和，高位在前，低位在后
    int sum2 = (quint8)frameData.at(MCU_FRAME_LEN - 2) << 8 | (quint8)frameData.at(MCU_FRAME_LEN - 1);
    if (sum != sum2) {
        qWarning() << "frame data is invalid:" << frameData;
        return false;
    }

    return true;
}


