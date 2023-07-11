//
// Created by zyz on 2023/5/28.
//

#include "handler32data.h"
#include "r32constant.h"

#include <QDebug>
#include <QByteArray>

Handler32data::Handler32data(QObject *parent)
    : HandleDataBase(parent)
{
    m_readFuncMap = {
            {CMD_01, &Handler32data::readOperateResult},
            {CMD_02, &Handler32data::readOperateResult},
            {CMD_ND_STATUS_03, &Handler32data::readOperateResult},
            {CMD_READ_R0_04, &Handler32data::readOperateData},
            {CMD_READ_PARAM1_05, &Handler32data::readOperateData},
            {CMD_READ_PARAM2_06, &Handler32data::readOperateData},
            {CMD_READ_1000PPM_07, &Handler32data::readOperateData},
            {CMD_READ_5000PPM_08, &Handler32data::readOperateData},
            {CMD_READ_TEMP_HUM_21, &Handler32data::readTemperatureHumidity},
            {CMD_READ_ADC_22, &Handler32data::readGasProbeADC},
            {CMD_READ_RESISTANCE_23, &Handler32data::readGasProbeResistance},
            {CMD_READ_COMPENSATION_RESISTANCE_24, &Handler32data::readGasProbeResistance},
            {CMD_READ_GAS_CONCENTRATION_25, &Handler32data::readGasConcentration},
            {CMD_READ_FIRMWARE_VERSION_41, &Handler32data::readFirmwareVersion},
            {CMD_OPEN_CLOSE_PRINT_42, &Handler32data::readOperateResult},
            {CMD_QUERY_FAULT_STATUS_43, &Handler32data::readProductFaultStatus},
            {CMD_SET_ALARM_THRESHOLD_44, &Handler32data::readOperateResult},
            {CMD_READ_ALARM_THRESHOLD_45, &Handler32data::readAlarmThreshold},
    };

}

Handler32data::~Handler32data()
{

}

void Handler32data::processReceivedData(const QByteArray &data)
{
    m_receivedData.append(data);

    // 1. 判断数据是否有效
    while (m_receivedData.length() >= RECV_FRAME_LENGTH) {
        // 2. 判断帧头是否正确
        if (m_receivedData.at(0) != RECV_HEADER) {
            m_receivedData.remove(0, 1);
            continue;
        }

        // 3.提取完整的一帧数据
        QByteArray frameData = m_receivedData.left(RECV_FRAME_LENGTH);
        m_receivedData.remove(0, RECV_FRAME_LENGTH);

        // 4. 判断校验和是否正确
        if (!frameIsValid(frameData)) {
            qWarning() << "recv frame is valid:" << frameData.toHex();
            continue;
        }

        Q_EMIT recvedFrameData(frameData);

        // 5. 提取命令和数据
        quint8 cmd = static_cast<quint8>(frameData.at(1));
        QByteArray content = frameData.mid(2, 4);

        // 6. 根据命令解析数据
        if (m_readFuncMap.contains(cmd)) {
            QVariantMap value;
            bool ret = (this->*m_readFuncMap.value(cmd))(cmd, content, value);
            if (ret) {
                Q_EMIT frameReceived(cmd, value);
            } else {
                qWarning() << "cmd:"<< cmd << "parse frame failed:" << frameData.toHex();
            }
        } else {
            qWarning() << "unknown command:"<< cmd << "not support:" << frameData.toHex();
        }
    }
}

// 判断帧数据的累加和是否正确
bool Handler32data::frameIsValid(const QByteArray &frameData) {
    int sumLow8 = static_cast<quint8>(frameData.at(frameData.length() - 1));

    int sum = 0;
    for (int i = 0; i < frameData.length() - 1; ++i) {
        sum += (quint8)frameData.at(i);
    }

    // 取sum的低8字节和sumLow8比较
    if ((sum & 0xFF) != sumLow8) {
        return false;
    }

    return true;
}

bool Handler32data::readOperateResult(quint8 cmd, const QByteArray &data, QVariantMap &value)
{
    if (data.length() != 4) {
        qWarning() << "cmd:" << cmd << "data length is not 4:" << data.toHex();
        return false;
    }

    // 读取浓度标定ack
    auto readNdAck = [](const QByteArray &data, QVariantMap &value) {
        quint8 result = static_cast<quint8>(data.at(0));
        value.insert(ACK_RESULT, result == 0 ? "成功" : "失败");
        if (result != 0) {
            quint8 error = static_cast<quint8>(data.at(1));
            switch (error) {
                case 1:
                    value.insert(ACK_ERROR, "浓度超测量量程");
                    break;
                case 2:
                    value.insert(ACK_ERROR, "传感器故障");
                    break;
                case 3:
                    value.insert(ACK_ERROR, "气体探头在加热中");
                    break;
                default:
                    value.insert(ACK_ERROR, "未知错误");
                    break;
            }
        }
    };

    // 读取标定完成ack
    auto readCalibrationAck = [](const QByteArray &data, QVariantMap &value) {
        quint8 result = static_cast<quint8>(data.at(0));
        value.insert(ACK_RESULT, result == 0 ? "成功" : "失败");
        if (result != 0) {
            quint8 error = static_cast<quint8>(data.at(1));
            switch (error) {
                case 1:
                    value.insert(ACK_ERROR, "标定的点数小于二");
                    break;
                case 2:
                    value.insert(ACK_ERROR, "写flash失败");
                    break;
                default:
                    value.insert(ACK_ERROR, "未知错误");
                    break;
            }
        }
    };

    // 查询标定状态ack
    auto readCalibrationStatusAck = [](const QByteArray &data, QVariantMap &value) {
        quint8 result = static_cast<quint8>(data.at(0));
        switch (result) {
            case 0:
                value.insert(ACK_RESULT, "未标定");
                break;
            case 1:
                value.insert(ACK_ERROR, "标定中");
                break;
            case 2:
                value.insert(ACK_ERROR, "标定失败");
                break;
            case 3:
                value.insert(ACK_ERROR, "标定成功");
                break;
            default:
                value.insert(ACK_ERROR, "未知状态");
                break;
        }
    };

    // 打开或者关闭周期性打印数据
    auto readPrintDataAck = [](const QByteArray &data, QVariantMap &value) {
        quint8 result = static_cast<quint8>(data.at(0));
        value.insert(ACK_RESULT, result == 0 ? "成功" : "失败");
        if (result != 0) {
            quint8 error = static_cast<quint8>(data.at(1));
            switch (error) {
                case 0:
                    value.insert(ACK_ERROR, "无错误");
                    break;
                case 1:
                    value.insert(ACK_ERROR, "数据超范围");
                    break;
                default:
                    value.insert(ACK_ERROR, "未知错误");
                    break;
            }
        }
    };

    // 设置报警阈值设置报警阈值
    auto readAlarmThresholdAck = [](const QByteArray &data, QVariantMap &value) {
        quint8 result = static_cast<quint8>(data.at(0));
        value.insert(ACK_RESULT, result == 0 ? "成功" : "失败");
        if (result != 0) {
            quint8 error = static_cast<quint8>(data.at(1));
            switch (error) {
                case 0:
                    value.insert(ACK_ERROR, "无错误");
                    break;
                case 1:
                    value.insert(ACK_ERROR, "报警阈值超范围");
                    break;
                case 2:
                    value.insert(ACK_ERROR, "写flash失败");
                    break;
                default:
                    value.insert(ACK_ERROR, "未知错误");
                    break;
            }
        }
    };

    // 设置模块地址
    auto readSetAddressAck = [](const QByteArray &data, QVariantMap &value) {
        value.insert(MODULE_ADDRESS, true);
        quint8 result = static_cast<quint8>(data.at(0));
        value.insert(ACK_RESULT, result == 0 ? "成功" : "失败");
        if (result != 0) {
            quint8 error = static_cast<quint8>(data.at(3));
            switch (error) {
                case 0:
                    value.insert(ACK_ERROR, "无错误");
                    break;
                case 1:
                    value.insert(ACK_ERROR, "地址超范围");
                    break;
                case 2:
                    value.insert(ACK_ERROR, "写flash失败");
                    break;
                default:
                    value.insert(ACK_ERROR, "未知错误");
                    break;
            }
        }
    };

    // 读取模块地址
    auto readAddressAck = [](const QByteArray &data, QVariantMap &value) {
        value.insert(MODULE_ADDRESS, true);
        quint8 address = static_cast<quint8>(data.at(0));
        value.insert(READ_MODULE_ADDRESS, address);
    };

    switch (cmd) {
        case CMD_01:
            if (m_isSetAddress) {
                readSetAddressAck(data, value);
            } else {
                readNdAck(data, value);
            }
            break;
        case CMD_02:
            if (m_isReadAddress) {
                readAddressAck(data, value);
            } else {
                readCalibrationAck(data, value);
            }
            break;
        case CMD_ND_STATUS_03:
            readCalibrationStatusAck(data, value);
            break;
        case CMD_OPEN_CLOSE_PRINT_42:
            readPrintDataAck(data, value);
            break;
        case CMD_SET_ALARM_THRESHOLD_44:
            readAlarmThresholdAck(data, value);
            break;

        default:
            qWarning() << "cmd:" << cmd << "not support:" << data.toHex();
            return false;
    }

    return true;
}

bool Handler32data::readOperateData(quint8 cmd, const QByteArray &data, QVariantMap &value) {
    if (data.length() != 4) {
        qWarning() << "cmd:" << cmd << "data length is not 4:" << data.toHex();
        return false;
    }

    // 读取ack数值
    float fData = 0;
    memcpy(&fData, data.data(), 4);
    // 4个字节的数据，高位在前，低位在后，使用左移和或运算
//    fData = (static_cast<quint8>(data.at(0)) << 24) | (static_cast<quint8>(data.at(1)) << 16) |
//            (static_cast<quint8>(data.at(2)) << 8) | (static_cast<quint8>(data.at(3)));

    switch (cmd) {
        case CMD_READ_R0_04:
            value.insert(ACK_FLOAT_VALUE, fData);
            break;
        case CMD_READ_PARAM1_05:
            value.insert(ACK_FLOAT_VALUE, fData);
            break;
        case CMD_READ_PARAM2_06:
            value.insert(ACK_FLOAT_VALUE, fData);
            break;
        case CMD_READ_1000PPM_07:
            value.insert(ACK_FLOAT_VALUE, fData);
            break;
        case CMD_READ_5000PPM_08:
            value.insert(ACK_FLOAT_VALUE, fData);
            break;
        default:
            qWarning() << "cmd:" << cmd << "not support:" << data.toHex();
            return false;
    }

    return true;
}

bool Handler32data::readTemperatureHumidity(quint8 cmd, const QByteArray &data, QVariantMap &value)
{
    // 两字节的温度，两字节的湿度
    if (data.length() != 4) {
        qWarning() << "cmd:" << cmd << "data length is not 4:" << data.toHex();
        return false;
    }

    // 读取温度
    qint16 temperature = 0;
    // 高字节在前，低字节在后,有符号数
    temperature = (static_cast<qint8>(data.at(0)) << 8) | (static_cast<quint8>(data.at(1)));
    value.insert(ACK_TEMPERATURE, temperature / 10.0);

    // 读取湿度
    quint16 humidity = 0;
    // 高字节在前，低字节在后
    humidity = (static_cast<quint8>(data.at(2)) << 8) | (static_cast<quint8>(data.at(3)));
    value.insert(ACK_HUMIDITY, humidity / 10.0);

    return true;
}

bool Handler32data::readGasProbeADC(quint8 cmd, const QByteArray &data, QVariantMap &value)
{
    // 共4字节，前两个字节代表ADC值
    if (data.length() != 4) {
        qWarning() << "cmd:" << cmd << "data length is not 4:" << data.toHex();
        return false;
    }

    // 读取ADC值
    quint16 adc = 0;
    // 高字节在前，低字节在后
    adc = (static_cast<quint8>(data.at(0)) << 8) | (static_cast<quint8>(data.at(1)));
    value.insert(ACK_ADC_VALUE, adc);

    return true;
}

bool Handler32data::readGasProbeResistance(quint8 cmd, const QByteArray &data, QVariantMap &value)
{
    // 单位为K欧姆，数据类型为float，将4字节组合在一起
    if (data.length() != 4) {
        qWarning() << "cmd:" << cmd << "data length is not 4:" << data.toHex();
        return false;
    }

    // 读取电阻值
    float resistance = 0;
    // 4个字节存储的float值
    resistance = (static_cast<quint8>(data.at(0)) << 24) | (static_cast<quint8>(data.at(1)) << 16) |
            (static_cast<quint8>(data.at(2)) << 8) | (static_cast<quint8>(data.at(3)));
    value.insert(ACK_FLOAT_VALUE, resistance);

    return true;
}

bool Handler32data::readGasConcentration(quint8 cmd, const QByteArray &data, QVariantMap &value)
{
    // 4个字节，前两个为气体浓度，第三个为报警状态
    if (data.length() != 4) {
        qWarning() << "cmd:" << cmd << "data length is not 4:" << data.toHex();
        return false;
    }

    // 读取浓度值
    quint16 concentration = 0;
    // 高字节在前，低字节在后
    concentration = (static_cast<quint8>(data.at(0)) << 8) | (static_cast<quint8>(data.at(1)));
    value.insert(ACK_GAS_CONCENTRATION, concentration);

    // 读取报警状态
    quint8 alarm = static_cast<quint8>(data.at(2));
        switch (alarm) {
            case 0:
                value.insert(ACK_ALARM_STATUS, "气体浓度正常");
                break;
            case 1:
                value.insert(ACK_ALARM_STATUS, "气体浓度超出报警阈值");
                break;
            default:
                value.insert(ACK_ALARM_STATUS, "未知");
                break;
        }

    return true;
}

bool Handler32data::readFirmwareVersion(quint8 cmd, const QByteArray &data, QVariantMap &value)
{
    // 4个字节，第一个字节为主版本号，第二个字节为副版本号
    if (data.length() != 4) {
        qWarning() << "cmd:" << cmd << "data length is not 4:" << data.toHex();
        return false;
    }

    // 读取主版本号和副版本号，组成版本号
    quint8 mainVersion = static_cast<quint8>(data.at(0));
    quint8 subVersion = static_cast<quint8>(data.at(1));
    QString version = QString("%1.%2").arg(mainVersion).arg(subVersion);
    qInfo() << "version:" << version;
    value.insert(ACK_FIRMWARE_VERSION, mainVersion);
    value.insert(ACK_FIRMWARE_SUB_VERSION, subVersion);

    return true;
}

bool Handler32data::readProductFaultStatus(quint8 cmd, const QByteArray &data, QVariantMap &value)
{
    // 4个字节，第一个字节故障码
    if (data.length() != 4) {
        qWarning() << "cmd:" << cmd << "data length is not 4:" << data.toHex();
        return false;
    }

    quint8 fault = static_cast<quint8>(data.at(0));
    if (fault & (1 << 0))
        value.insert(ACK_ERROR, "内部错误 - 测量结果错误，比如：内部芯片通信故障");
    if (fault & (1 << 1))
        value.insert(ACK_ERROR, "数据超范围 - 传感器检测到的温湿度，气体浓度超范围了");
    if (fault & (1 << 3))
        value.insert(ACK_ERROR, "自检错误 - 设置错误等");
    if (fault & (1 << 4))
        value.insert(ACK_ERROR, "永久性损坏 - 任何无法恢复的传感器错误，需要更换传感器");
    if (fault & (1 << 5))
        value.insert(ACK_ERROR, "总运行时间超范围 - 生命达到了极限");

    return true;
}

void Handler32data::setSlaveAddress(char slaveAddress)
{
    m_address = slaveAddress;
}

void Handler32data::addContent(char cmd, const QVariantMap &info, QByteArray &data)
{
    // 添加4字节 0x00
    auto addFourNoneByte = [&data](){
        char noneByte = 0x00;
        data.append(noneByte);
        data.append(noneByte);
        data.append(noneByte);
        data.append(noneByte);
    };

    data.append(SEND_HEADER);
    data.append(m_address);
    data.append(cmd);
    switch (cmd) {
        case CMD_01: {
            bool isSetAddress = info.value(MODULE_ADDRESS, false).toBool();
            if (!isSetAddress) {
                if (!addCmd_nd_Content(info, data))
                    return;
            } else {
                if (!addCmd_set_address_Content(info, data))
                    return;
            }
            break;
        }
        case CMD_02: {
            addFourNoneByte();
            bool isReadAddress = info.value(MODULE_ADDRESS, false).toBool();
            if (!isReadAddress) {
                m_isReadAddress = false;
            } else {
               data[1] = 0x00;
               m_isReadAddress = true;
            }
            break;
        }
        case CMD_OPEN_CLOSE_PRINT_42:
            if (!addCmd_enable_print_Content(info, data))
                return;
            break;
        case CMD_SET_ALARM_THRESHOLD_44:
            if (!addCmd_set_alarm_threshold_Content(info, data))
                return;
            break;
        default:
            addFourNoneByte();
            break;
    }

    addCheckSum(data);
}

void Handler32data::addCheckSum(QByteArray &data)
{
    quint16 checksum = 0;
    for (char i : data)
        checksum += static_cast<quint8>(i);

    // 只要低8位
    data.append(static_cast<char>(checksum & 0xFF));
}

bool Handler32data::addCmd_nd_Content(const QVariantMap &info, QByteArray &data)
{
    if (!info.contains(SEND_CAL_POINT) || !info.contains(SEND_CAL_CONCENTRATION)) {
        qWarning() << "cmd:" << CMD_01 << "info not contains cal point or cal concentration";
        return false;
    }

    m_isSetAddress = false;
    quint8 calPoint = static_cast<quint8>(info.value(SEND_CAL_POINT).toUInt());
    quint16 calConcentration = static_cast<quint16>(info.value(SEND_CAL_CONCENTRATION).toUInt());
    qInfo() << "calPoint:" << calPoint << "calConcentration:" << calConcentration;

    data.append(calPoint);
    data.append(static_cast<char>(calConcentration >> 8));
    data.append(static_cast<char>(calConcentration & 0xFF));
    // 补充1个字节 0x00
    char noneByte = 0x00;
    data.append(noneByte);

    return true;
}

bool Handler32data::addCmd_enable_print_Content(const QVariantMap &info, QByteArray &data)
{
    if (!info.contains(SEND_PRINT_ENABLE)) {
        qWarning() << "cmd:" << CMD_OPEN_CLOSE_PRINT_42 << "info not contains print content";
        return false;
    }

    data.append(static_cast<char>(info.value(SEND_PRINT_ENABLE).toUInt()));
    // 补充3个字节 0x00
    char noneByte = 0x00;
    data.append(noneByte);
    data.append(noneByte);
    data.append(noneByte);

    return true;
}

bool Handler32data::addCmd_set_alarm_threshold_Content(const QVariantMap &info, QByteArray &data)
{
    if (!info.contains(SEND_ALARM_THRESHOLD)) {
        qWarning() << "cmd:" << CMD_SET_ALARM_THRESHOLD_44 << "info not contains alarm enable or alarm concentration";
        return false;
    }

    quint16 alarmThreshold = static_cast<quint16>(info.value(SEND_ALARM_THRESHOLD).toUInt());

    data.append(static_cast<char>(alarmThreshold >> 8));
    data.append(static_cast<char>(alarmThreshold & 0xFF));

    // 补充两个字节 0x00
    char noneByte = 0x00;
    data.append(noneByte);
    data.append(noneByte);

    return true;
}

bool Handler32data::addCmd_set_address_Content(const QVariantMap &info, QByteArray &data)
{
    if (!info.contains(SET_MODULE_ADDRESS)) {
        qWarning() << "cmd:" << CMD_01 << "info not contains module address";
        return false;
    }

    m_address = static_cast<char>(info.value(SET_MODULE_ADDRESS).toUInt());
    m_isSetAddress = true;
    // 修改i地址字节，添加设置地址数据
    data[1] = 0x00;
    data.append(static_cast<char>(info.value(SET_MODULE_ADDRESS).toUInt()));

    // 添加3字节 0x00
    char noneByte = 0x00;
    data.append(noneByte);
    data.append(noneByte);
    data.append(noneByte);

    return true;
}

bool Handler32data::readAlarmThreshold(quint8 cmd, const QByteArray &data, QVariantMap &value)
{
    if (data.size() != 4) {
        qWarning() << "error cmd data:" << cmd << data;
        return false;
    }

    // 两个字节表示报警阈值
    quint16 alarmThreshold = static_cast<quint16>(data[0] << 8 | data[1]);
    value.insert(ACK_ALARM_THRESHOLD, alarmThreshold);

    return true;
}
