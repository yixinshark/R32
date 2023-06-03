//
// Created by yixin on 2023/5/28.
//

#ifndef R32_HANDLER32DATA_H
#define R32_HANDLER32DATA_H

#include "handledatabase.h"

#include <QMap>

class Handler32data : public HandleDataBase
{
    Q_OBJECT
public:
    explicit Handler32data(QObject *parent = nullptr);
    ~Handler32data() override;

    void setSlaveAddress(char slaveAddress);

public:
    virtual void processReceivedData(const QByteArray &data);

protected:
    // 填充发送内容
    virtual void addContent(char cmd, const QVariantMap &info, QByteArray &data);
    virtual void addCheckSum(QByteArray &data) override;

private:
    // 添加浓度标定命令的数据
    bool addCmd_nd_Content(const QVariantMap &info, QByteArray &data);
    // 打开或者关闭周期性打印数据
    bool addCmd_enable_print_Content(const QVariantMap &info, QByteArray &data);
    // 设置报警阈值
    bool addCmd_set_alarm_threshold_Content(const QVariantMap &info, QByteArray &data);
    // 设置模块地址
    bool addCmd_set_address_Content(const QVariantMap &info, QByteArray &data);

private:
    // 帧数据是否有效
    virtual bool frameIsValid(const QByteArray &frameData);

    // 读取操作结果
    bool readOperateResult(quint8 cmd, const QByteArray &data, QVariantMap &value);
    // 读取操作后的数据
    bool readOperateData(quint8 cmd, const QByteArray &data, QVariantMap &value);
    // 读取温湿度
    bool readTemperatureHumidity(quint8 cmd, const QByteArray &data, QVariantMap &value);
    // 读取气体探头的ADC采样值
    bool readGasProbeADC(quint8 cmd, const QByteArray &data, QVariantMap &value);
    // 读取气体探头的阻值 / 4.读取温度补偿后的气体探头阻值
    bool readGasProbeResistance(quint8 cmd, const QByteArray &data, QVariantMap &value);
    // 读取气体浓度ppm和报警状态值
    bool readGasConcentration(quint8 cmd, const QByteArray &data, QVariantMap &value);
    // 读取固件版本号
    bool readFirmwareVersion(quint8 cmd, const QByteArray &data, QVariantMap &value);
    // 查询产品的故障状态
    bool readProductFaultStatus(quint8 cmd, const QByteArray &data, QVariantMap &value);
    // 读取报警阈值
    bool readAlarmThreshold(quint8 cmd, const QByteArray &data, QVariantMap &value);

private:
    typedef bool (Handler32data::*readFunc)(quint8 cmd, const QByteArray &data, QVariantMap &value);
    QMap<quint8, readFunc> m_readFuncMap;

private:
    char m_address = 0x00;
    // 设置模块的地址的命令号:0x01
    bool m_isSetAddress = false;
    // 读取模块的地址的命令号:0x02
    bool m_isReadAddress = false;
};


#endif //R32_HANDLER32DATA_H
