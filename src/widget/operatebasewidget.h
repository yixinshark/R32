//
// Created by zyz on 23-5-13.
//

#ifndef R32_OPERATEBASEWIDGET_H
#define R32_OPERATEBASEWIDGET_H

#include <QWidget>
#include <QVariantMap>

class QPushButton;
class QLineEdit;

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
    void cmdCompleted(quint8 cmd);
    void operatedMsg(const QString &msg);
    // 分析仪浓度值
    void r32NDValue(quint16 nd);
    void ntcTemperatureValue(const QString &temperture);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    QLayout *initSerialPortUI();
    QLayout *initReadNTCInfoUI();
    QLayout *initReadR32InfoUI(const QString &btnTitle);

    // 点击按钮，发送对应的命令
    void sendDataBtnClicked(const QVariantMap &info = QVariantMap());

    virtual void recvAckData(quint8 cmd, const QVariantMap &info);
    void operateMsg(const QString &msg);

    void showNTCInfo(quint8 cmd, const QVariantMap &info);
    void showR32Info(quint8 cmd, const QVariantMap &info);

private:
    bool connectSerialPort();

protected:
    QList<DelayedButton *> m_delayBtnList;

    QPushButton *m_connectBtn;
    StatusWidget *m_cntStatusWidget;
    SerialPortSettingsWidget *m_serialPortSettings;

    SerialPortCom *m_serialPortCom;
    HandleData *m_handleData;

    // NTC
    QLineEdit *m_showADCValue;
    QLineEdit *m_showTemperatureValue;

    // R32
    QLineEdit *m_showR32ADCValue;
    QLineEdit *m_showR32NDValue;
};

#endif //R32_OPERATEBASEWIDGET_H
