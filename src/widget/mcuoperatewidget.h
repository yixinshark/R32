//
// Created by yixin on 2023/5/31.
//

#ifndef R32_MCUOPERATEWIDGET_H
#define R32_MCUOPERATEWIDGET_H

#include <QWidget>
#include "operatebasewidget.h"

class QGridLayout;
class QCheckBox;
class QRadioButton;
class QComboBox;

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
    // 显示箱体开关状态
    void showBoxSwitchStatus(const QByteArray &info);
    // 显示电磁阀控制状态
    void showValveCtrlStatus(const QByteArray &info);
    // 显示风扇控制状态
    void showFanCtrlStatus(const QByteArray &info);
    // 显示浓度值
    void showConcentrationValue(const QByteArray &info);
    // 显示通道选择值
    void showChannelSelectValue(const QByteArray &info);
    // 显示报警灯控制状态
    void showAlarmCtrlStatus(const QByteArray &info);

private:
    QGridLayout *m_gridLayout;
    QLineEdit *m_alarmChannelEdit;
    QLineEdit *m_concentrationEdit;
    QComboBox  *m_channelBox;

    // 箱体开关
    QVector<QRadioButton *> m_boxSwitchs;
    // 电磁阀
    QVector<QCheckBox *> m_valves;
    // 风扇
    QVector<QCheckBox *> m_fans;
    // 报警开关
    QVector<QRadioButton *> m_alarmSwitchs;
};

#endif //R32_MCUOPERATEWIDGET_H
