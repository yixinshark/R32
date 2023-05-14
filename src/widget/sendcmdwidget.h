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

class SendCmdWidget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit SendCmdWidget(QWidget *parent = nullptr);
    ~SendCmdWidget() override;

protected:
    void recvAckData(int cmd, const QVariantMap &info);
    void sendDataBtnClicked(const QVariantMap &info = QVariantMap());

private:
    void initUI();

    // 设置从机地址
    void initSlaveAddressUI();
    void initReadSlaveAddressUI();

private:
    void showProductSlaveAddress(const QVariantMap &info);

private:
    QVBoxLayout *m_mainLayout;
    // 输入从机地址框
    QLineEdit *m_inputSlaveAddress;

    QLabel *m_slaveAddressLabel;

};

#endif //R32_SENDCMDWIDGET_H
