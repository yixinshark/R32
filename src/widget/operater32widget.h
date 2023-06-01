//
// Created by yixin on 2023/6/1.
//

#ifndef R32_OPERATER32WIDGET_H
#define R32_OPERATER32WIDGET_H

#include <QWidget>

#include "operatebasewidget.h"

class QGridLayout;
class QVBoxLayout;

class OperateR32Widget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit OperateR32Widget(HandleDataBase *handleData, QWidget *parent = nullptr);
    ~OperateR32Widget() override;

protected:
    void recvAckData(quint8 cmd, const QVariantMap &info) override;

private:
    void initUI();

    // 初始化标定类指令UI
    void initCalibrationCmdUI();
    // 初始化读取参数类指令UI
    void initReadParamCmdUI();
    // 初始化检测类指令UI
    void initDetectionCmdUI();
    // 初始化系统类指令UI
    void initSystemCmdUI();
    // 初始化广播类指令UI
    void initBroadcastCmdUI();

private:
    QGridLayout *m_gridLayout;
    QVBoxLayout *m_mainLayout;

    // 标定浓度结果框
    QLineEdit *m_calibrationResultEdit;
    // 标定完成结果框
    QLineEdit *m_calFinishResultEdit;
    // 获取标定状态结果框
    QLineEdit *m_calStatusResultEdit;

    // 读取零点电阻结果框
    QLineEdit *m_readZeroResResultEdit;
    // 读取标定参数1结果框
    QLineEdit *m_readCalP1ResultEdit;
    // 读取标定参数2结果框
    QLineEdit *m_readCalP2ResultEdit;
    // 读取标定参数3结果框
    QLineEdit *m_readCalP3ResultEdit;
    // 读取标定参数4结果框
    QLineEdit *m_readCalP4ResultEdit;

    // 当前温度结果框
    QLineEdit *m_curTempResultEdit;
    // 当前湿度结果框
    QLineEdit *m_curHumidityResultEdit;
    // 当前气体ADC结果框
    QLineEdit *m_curGasADCResultEdit;
    // 获取气体阻值结果框
    QLineEdit *m_gasResResultEdit;
    // 获取补偿后的阻值结果框
    QLineEdit *m_compensateResResultEdit;
    // 获取浓度结果框
    QLineEdit *m_concentrationResultEdit;
    // 获取报警状态结果框
    QLineEdit *m_alarmStatusResultEdit;

    // 主版本号结果框
    QLineEdit *m_mainVersionResultEdit;
    // 副版本号结果框
    QLineEdit *m_subVersionResultEdit;
    // 设置报警阈值结果
    QLineEdit *m_setAlarmThresholdResultEdit;
    // 读取报警阈值结果框
    QLineEdit *m_readAlarmThresholdResultEdit;
    // 读取故障码结果框
    QLineEdit *m_readFaultCodeResultEdit;
    // 读取打印数据开关结果框
    QLineEdit *m_printDataSwitchResultEdit;

    // 设置模块地址结果框
    QLineEdit *m_setModuleAddressResultEdit;
    // 读取模块地址结果框
    QLineEdit *m_readModuleAddressResultEdit;
};

#endif //R32_OPERATER32WIDGET_H
