//
// Created by zyz on 23-5-13.
//

#ifndef R32_OPERATEBASEWIDGET_H
#define R32_OPERATEBASEWIDGET_H

#include <QWidget>
#include <QVariantMap>

class QPushButton;

class DelayedButton;
class StatusWidget;
class HandleData;
class SerialPortCom;
class SerialPortSettingsWidget;

class OperateBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OperateBaseWidget(QWidget *parent = nullptr);
    ~OperateBaseWidget() override;

signals:
    void operatedMsg(const QString &msg);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    QLayout *initSerialPortUI();

    // 点击按钮，发送对应的命令
    void sendDataBtnClicked(const QVariantMap &info = QVariantMap());

    virtual void recvAckData() {};
    void operateMsg(const QString &msg);

private:
    bool connectSerialPort();

protected:
    QList<DelayedButton *> m_delayBtnList;

    QPushButton *m_connectBtn;
    StatusWidget *m_cntStatusWidget;
    SerialPortSettingsWidget *m_serialPortSettings;

    SerialPortCom *m_serialPortCom;
    HandleData *m_handleData;
};


#endif //R32_OPERATEBASEWIDGET_H
