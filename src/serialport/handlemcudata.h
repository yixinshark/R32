//
// Created by yixin on 2023/5/31.
//

#ifndef R32_HANDLEMCUDATA_H
#define R32_HANDLEMCUDATA_H

#include "handledatabase.h"

#include <QMap>

class HandleMcuData : public HandleDataBase
{
    Q_OBJECT
public:
    explicit HandleMcuData(QObject *parent = nullptr);
    ~HandleMcuData() override;

public:
    virtual void processReceivedData(const QByteArray &data) override;

protected:
    // 填充发送内容
    virtual void addContent(char cmd, const QVariantMap &info, QByteArray &data);
    bool frameIsValid(const QByteArray &frameData) override;

    // 添加电磁阀内容
    bool addCmd_valve_Content(const QVariantMap &info, QByteArray &data);
    // 添加风扇内容
    bool addCmd_fan_Content(const QVariantMap &info, QByteArray &data);
    // 添加浓度内容
    bool addCmd_nd_Content(const QVariantMap &info, QByteArray &data);
    // 添加设置通道内容
    bool addCmd_set_channel_Content(const QVariantMap &info, QByteArray &data);
    // 添加箱体开关内容
    bool addCmd_box_switch_Content(const QVariantMap &info, QByteArray &data);
    // 添加报警灯控制内容
    bool addCmd_alarm_light_Content(const QVariantMap &info, QByteArray &data);
};

#endif //R32_HANDLEMCUDATA_H
