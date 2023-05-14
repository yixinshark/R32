//
// Created by zyz on 23-5-13.
//

#ifndef R32_SENDCMDWIDGET_H
#define R32_SENDCMDWIDGET_H

#include "operatebasewidget.h"

#include <QVariantMap>

class QLabel;
class QLineEdit;
class QVBoxLayout;

class DelayedButton;

class SendCmdWidget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit SendCmdWidget(QWidget *parent = nullptr);
    ~SendCmdWidget() override;

protected:
    void recvAckData(int cmd, const QVariantMap &info);

private:
    void initUI();

    // 设置从机地址
    void initSlaveAddressUI();
    // 读取从机地址
    void initReadSlaveAddressUI();
    // 读取产品的ID号(唯一识别码)
    void initReadProductIDUI();
    // 显示读取软件版本号
    void initReadSoftVersionUI();

    // 零点标定
    void initSetLDUI();
    // 浓度标定
    void initSetNDUI();

private:
    void showProductSlaveAddress(const QVariantMap &info);
    void showProductID(const QVariantMap &info);
    void showSoftwareVersion(const QVariantMap &info);
    void showSetLDResult(const QVariantMap &info);
    void showSetNDResult(const QVariantMap &info);

private:
    QVBoxLayout *m_mainLayout;

    // 输入从机地址框
    QLineEdit *m_inputSlaveAddress;

    // 显示从机地址
    QLineEdit *m_slaveAddressLabel;
    // 显示产品的ID号(唯一识别码)
    QLineEdit *m_productIDLabel;
    // 显示软件版本
    QLineEdit *m_softwareLabel;
    // 显示设置零点标定结果
    QLineEdit *m_showSetLDResult;
    // 显示设置浓度标定结果
    QLineEdit *m_showSetNDResult;
};

#endif //R32_SENDCMDWIDGET_H
