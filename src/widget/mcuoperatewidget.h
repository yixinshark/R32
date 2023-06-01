//
// Created by yixin on 2023/5/31.
//

#ifndef R32_MCUOPERATEWIDGET_H
#define R32_MCUOPERATEWIDGET_H

#include <QWidget>
#include "operatebasewidget.h"

class QGridLayout;

class McuOperateWidget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit McuOperateWidget(HandleDataBase *handleData, QWidget *parent = nullptr);
    ~McuOperateWidget() override;

protected:
    void recvAckData(quint8 cmd, const QVariantMap &info) override;

private:
    void initUI();
    // 初始化箱体开关界面
    void initBoxSwitchUI(int rowIndex);
    // 初始化电磁阀控制界面
    void initValveCtrlUI(int rowIndex);
    // 初始化风扇控制界面
    void initFanCtrlUI(int rowIndex);
    // 初始化浓度控制界面
    void initConcentrationCtrlUI(int rowIndex);
    // 初始化通道选择界面
    void initChannelSelectUI(int rowIndex);
    // 初始化报警灯控制界面
    void initAlarmCtrlUI(int rowIndex);

private:
    QGridLayout *m_gridLayout;
};

#endif //R32_MCUOPERATEWIDGET_H
