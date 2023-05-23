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
    explicit SendCmdWidget(QWidget *parent = nullptr);
    ~SendCmdWidget() override;

public slots:
    void setNDValue(quint16 nd);
    void setNTCTemperature(const QString &temperture);

protected:
    void recvAckData(quint8 cmd, const QVariantMap &info);

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

private:
    void showProductSlaveAddress(quint8 cmd, const QVariantMap &info);
    void showProductID(quint8 cmd, const QVariantMap &info);
    void showSoftwareVersion(quint8 cmd, const QVariantMap &info);
    void showOperateResult(quint8 cmd, const QVariantMap &info, QLineEdit *resultEdit);
    void stepCompleted(quint8 cmd);

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
};

#endif //R32_SENDCMDWIDGET_H
