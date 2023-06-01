//
// Created by zyz on 23-5-13.
//

#ifndef R32_SENDCMDWIDGET_H
#define R32_SENDCMDWIDGET_H

#include "operatebasewidget.h"
#include "r32recordvalue.h"

#include <QVariantMap>

class QLabel;
class QLineEdit;
class QVBoxLayout;
class StepsWidget;

class DelayedButton;

class SendCmdWidget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit SendCmdWidget(HandleDataBase *handleData, QWidget *parent = nullptr);
    ~SendCmdWidget() override;

signals:
    void r32NDValue(quint16 nd);
    void ntcTemperatureValue(const QString &temperture);

public:
    void setNDValue(quint16 nd);
    void setNTCTemperature(const QString &temperture);

protected:
    void recvAckData(quint8 cmd, const QVariantMap &info);
    void sendCmdData(quint8 cmd, const QVariantMap &info = QVariantMap()) override;

private:
    void initUI();

    // 设置从机地址
    void initSlaveAddressUI();
    // 读取从机地址
    void initReadSlaveAddressUI();
    // 设置产品的ID号(唯一识别码)
    void initSetProductIDUI();
    // 读取产品的ID号(唯一识别码)
    void initReadProductIDUI();
    // 显示读取软件版本号
    void initReadSoftVersionUI();

    // 零点标定
    void initSetLDUI();
    // 浓度标定
    void initSetNDUI();
    // 读取NTC信息
    void initReadNTCInfoUI();
    // 读取R32信息
    void initReadR32InfoUI();

private:
    void showProductSlaveAddress(quint8 cmd, const QVariantMap &info);
    void showProductID(quint8 cmd, const QVariantMap &info);
    void showSoftwareVersion(quint8 cmd, const QVariantMap &info);
    void showOperateResult(quint8 cmd, const QVariantMap &info, QLineEdit *resultEdit);
    void stepCompleted(quint8 cmd);

    void showNTCInfo(quint8 cmd, const QVariantMap &info);
    void showR32Info(quint8 cmd, const QVariantMap &info);

private:
    R32RecordValue m_r32RecordValue;

private:
    QVBoxLayout *m_mainLayout;
    StepsWidget *m_stepsWidget;

    // 输入从机地址框
    QLineEdit *m_inputSlaveAddress;
    QLineEdit *m_inputSlaveAddressResult;

    QLineEdit *m_setProductIDInput;
    // 浓度输入
    QLineEdit *m_ndInput;
    // 温度输入
    QLineEdit *m_temperatureInput;

    // 显示从机地址
    QLineEdit *m_slaveAddressLabel = nullptr;
    // 显示产品的ID号(唯一识别码)
    QLineEdit *m_productIDLabel = nullptr;
    // 显示软件版本
    QLineEdit *m_softwareLabel;
    // 显示设置零点标定结果
    QLineEdit *m_showSetLDResult;
    // 显示设置浓度标定结果
    QLineEdit *m_showSetNDResult;
    // 显示读取产品的ID号(唯一识别码)结果
    QLineEdit *m_showSetProductIDResult;

    QTimer *m_timer;
     // NTC
    QLineEdit *m_showADCValue;
    QLineEdit *m_showTemperatureValue;

    // R32
    QLineEdit *m_showR32ADCValue;
    QLineEdit *m_showR32NDValue;
};

#endif //R32_SENDCMDWIDGET_H
