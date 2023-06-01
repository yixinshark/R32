//
// Created by zyz on 23-5-12.
//

#ifndef R32_OPERATEWIDGET_H
#define R32_OPERATEWIDGET_H

#include <QWidget>

//class SendCmdWidget;
class OperateR32Widget;
class RecvR32DataWidget;
class McuOperateWidget;

class QLabel;
class OperateWidget : public QWidget
{
Q_OBJECT

public:
    explicit OperateWidget(QWidget *parent = nullptr);
    ~OperateWidget() override;

private:
    void initUI();
    void showMsg(const QString &msg);

private:
//    SendCmdWidget *m_sendCmdWidget;
    OperateR32Widget *m_operateR32Widget;
    RecvR32DataWidget *m_recvR32DataWidget;
    McuOperateWidget *m_mcuOperateWidget;

    QLabel *m_msgLabel;
};


#endif //R32_OPERATEWIDGET_H
