//
// Created by zyz on 23-5-13.
//

#ifndef R32_OPERATEBASEWIDGET_H
#define R32_OPERATEBASEWIDGET_H

#include <QWidget>

class QPushButton;

class HandleData;
class SerialPortCom;
class SerialPortSettingsWidget;

class OperateBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OperateBaseWidget(QWidget *parent = nullptr);
    ~OperateBaseWidget() override;

protected:
    QLayout *initSerialPortUI();
    virtual void recvAckData() {};

private:
    bool connectSerialPort();

protected:
    QPushButton *m_connectBtn;
    SerialPortSettingsWidget *m_serialPortSettings;

    SerialPortCom *m_serialPortCom;
    HandleData *m_handleData;
};


#endif //R32_OPERATEBASEWIDGET_H
