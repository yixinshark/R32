//
// Created by zyz on 23-5-13.
//

#ifndef R32_RECVR32DATAWIDGET_H
#define R32_RECVR32DATAWIDGET_H

#include "operatebasewidget.h"

class QLineEdit;
class QVBoxLayout;

class RecvR32DataWidget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit RecvR32DataWidget(QWidget *parent = nullptr);
    ~RecvR32DataWidget() override;

private:
    void recvAckData(int cmd, const QVariantMap &info);

private:
    void initUI();

    void initReadNTCInfoUI();
    void initReadR32InfoUI();

private:
    void showNTCInfo(const QVariantMap &info);
    void showR32Info(const QVariantMap &info);

private:
    QVBoxLayout *m_mainLayout;

    // NTC
    QLineEdit *m_showADCValue;
    QLineEdit *m_showTemperatureValue;

    // R32
    QLineEdit *m_showR32ADCValue;
    QLineEdit *m_showR32NDValue;
};


#endif //R32_RECVR32DATAWIDGET_H
