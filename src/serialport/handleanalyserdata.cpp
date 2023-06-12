//
// Created by yixin on 2023/6/12.
//

#include "handleanalyserdata.h"
#include "analyserconstant.h"

#include <QDebug>
#include <QVariantMap>

HandleAnalyserData::HandleAnalyserData(QObject *parent)
    : HandleDataBase(parent)
{

}

HandleAnalyserData::~HandleAnalyserData()
{

}

void HandleAnalyserData::setAddress(char address)
{
    m_address = address;
}

void HandleAnalyserData::processReceivedData(const QByteArray &data)
{
    m_receivedData.append(data);

    while (m_receivedData.length() > 3) {
        // 1. 判断帧头，第一位地址，第二位命令
        if (m_receivedData.at(0) != m_address && m_receivedData.at(1) != ANALYSER_CMD) {
            m_receivedData.remove(0, 1);
            continue;
        }

        // 2. 第三个字节代表数据长度，5=3+2（前三位和后两位CRC）
        int dataLen = m_receivedData.at(2);
        if (m_receivedData.length() < dataLen + 5) {
            break;
        }

        // 3. 提取帧数据
        QByteArray frameData = m_receivedData.left(dataLen + 5);
        m_receivedData.remove(0, dataLen);

        // 4. 判断帧数据是否有效
        if (!frameIsValid(frameData)) {
            qWarning() << "frame data is invalid:" << frameData;
            continue;
        }

        Q_EMIT recvedFrameData(frameData);

        // 5. 提取帧内容
        char cmd = frameData.at(1);
        QByteArray content = frameData.mid(3, dataLen - 5);
        handleContent(content);
    }
}

void HandleAnalyserData::handleContent(const QByteArray &content)
{
    // 只处理0x06和0x1C长度的数据
    if (content.length() != 0x06 && content.length() != 0x1C) {
        qWarning() << "content length is invalid:" << content;
        return;
    }

    auto getBaseData = [&content](QVariantMap &info) {
        // 前4位气体浓度，后两位报警状态，都是高字节在前，低字节在后
        int gasConcentration = static_cast<quint8>(content.at(0)) << 24 |
                               static_cast<quint8>(content.at(1)) << 16 |
                               static_cast<quint8>(content.at(2)) << 8 |
                               static_cast<quint8>(content.at(3));

        int alarmStatus = static_cast<quint8>(content.at(4)) << 8 | static_cast<quint8>(content.at(5));


        info.insert(ANALYSER_GAS_CONCENTRATION, gasConcentration);
        info.insert(ANALYSER_ALARM_STATUS, alarmStatus);
    };

    // 1. 提取数据
    if (content.length() == 0x06) {

        QVariantMap info;
        getBaseData(info);

        Q_EMIT frameReceived(ANALYSER_CMD, info);
    } else if (content.length() == 0x1C) {
        QVariantMap info;
        getBaseData(info);

        info.insert(ANALYSER_GAS_NAME, "CO");

        QByteArray otherConent = content.mid(12, 12);
        // 前两二字节为放大倍数
        int magnification = static_cast<quint8>(otherConent.at(0)) << 8 | static_cast<quint8>(otherConent.at(1));
        // 两个字节气体单位
        int gasUnit = static_cast<quint8>(otherConent.at(2)) << 8 | static_cast<quint8>(otherConent.at(3));
        // 4个字节低报值
        int lowAlarmValue = static_cast<quint8>(otherConent.at(4)) << 24 |
                            static_cast<quint8>(otherConent.at(5)) << 16 |
                            static_cast<quint8>(otherConent.at(6)) << 8 |
                            static_cast<quint8>(otherConent.at(7));
        // 4个字节高报值
        int highAlarmValue = static_cast<quint8>(otherConent.at(8)) << 24 |
                             static_cast<quint8>(otherConent.at(9)) << 16 |
                             static_cast<quint8>(otherConent.at(10)) << 8 |
                             static_cast<quint8>(otherConent.at(11));

        info.insert(ANALYSER_MAGNIFICATION, magnification);
        info.insert(ANALYSER_GAS_UNIT, gasUnit);
        info.insert(ANALYSER_GAS_LOW_ALARM, lowAlarmValue);
        info.insert(ANALYSER_GAS_HIGH_ALARM, highAlarmValue);

        Q_EMIT frameReceived(ANALYSER_CMD, info);
    }
}

bool HandleAnalyserData::frameIsValid(const QByteArray &frameData)
{
    quint8 uchCRCHi = 0xFF; /* 高CRC字节初始化*/
    quint8 uchCRCLo = 0xFF; /* 低CRC 字节初始化*/
    quint32 uIndex; /* CRC循环中的索引*/
    int usDataLen = frameData.length() - 2;
    for (int i = 0; i < usDataLen; i++) /* 传输消息缓冲区*/
    {
        uIndex = uchCRCHi ^ frameData.at(i); /* 计算CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }

    if (uchCRCHi == static_cast<quint8>(frameData.at(usDataLen)) &&
        uchCRCLo == static_cast<quint8>(frameData.at(usDataLen + 1))) {
        return true;
    }

    return false;
}

void HandleAnalyserData::addContent(char cmd, const QVariantMap &info, QByteArray &data)
{
    // 1. 添加地址
    char address = static_cast<char>(info.value(ADDRESS, 0x01).toInt());
    m_address = address;

    data.append(address);
    data.append(cmd);
    // 寄存器起始地址
    data.append((char)0x00);
    data.append((char)0x05);

    bool moreInfo = info.value(MORE_INFO, false).toBool();
    quint16 dataLen = moreInfo ? 0x0E : 0x03;
    data.append((char)(dataLen >> 8));
    data.append((char)(dataLen & 0xFF));

    // 再添加两个字节的CRC
    quint8 uchCRCHi = 0xFF; /* 高CRC字节初始化*/
    quint8 uchCRCLo = 0xFF; /* 低CRC 字节初始化*/
    quint32 uIndex; /* CRC循环中的索引*/
    int usDataLen = data.length();
    for (int i = 0; i < usDataLen; i++) /* 传输消息缓冲区*/
    {
        uIndex = uchCRCHi ^ data.at(i); /* 计算CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }

    data.append(uchCRCHi);
    data.append(uchCRCLo);
}


