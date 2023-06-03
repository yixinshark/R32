//
// Created by zyz on 23-5-13.
//

#ifndef R32_OPERATEBASEWIDGET_H
#define R32_OPERATEBASEWIDGET_H

#include <QWidget>
#include <QVariantMap>
#include "serialportsettingswidget.h"

class QPushButton;
class QLineEdit;

class DelayedButton;
class StatusWidget;
class HandleDataBase;
class SerialPortCom;

/**
 * @brief 操作基类
 */
class OperateBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OperateBaseWidget(HandleDataBase *handleData, QWidget *parent = nullptr);
    ~OperateBaseWidget() override;

signals:
    void cmdCompleted(quint8 cmd);
    void operatedMsg(const QString &msg);
    void serialPortClosed();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    QLayout *initSerialPortUI(SerialPortSettingsWidget::LayoutDirection direction = SerialPortSettingsWidget::Horizontal);

    // 发送数据
    virtual void sendCmdData(quint8 cmd, const QVariantMap &info = QVariantMap());

    virtual void recvAckData(quint8 cmd, const QVariantMap &info) = 0;
    void operateMsg(const QString &msg);

private:
    bool connectSerialPort();

protected:
    QList<DelayedButton *> m_delayBtnList;

    QPushButton *m_connectBtn;
    StatusWidget *m_cntStatusWidget;
    SerialPortSettingsWidget *m_serialPortSettings;

    SerialPortCom *m_serialPortCom;
    HandleDataBase *m_handleData;
};

#endif //R32_OPERATEBASEWIDGET_H
