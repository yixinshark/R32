//
// Created by zyz on 23-5-12.
//

#ifndef R32_OPERATEWIDGET_H
#define R32_OPERATEWIDGET_H

#include <QWidget>

class SendCmdWidget;
class RecvR32DataWidget;

class QLabel;
class OperateWidget : public QWidget {
Q_OBJECT

public:
    explicit OperateWidget(QWidget *parent = nullptr);
    ~OperateWidget() override;

private:
    void initUI();
    void showMsg(const QString &msg);

private:
    SendCmdWidget *m_sendCmdWidget;
    RecvR32DataWidget *m_recvR32DataWidget;

    QLabel *m_msgLabel;
};


#endif //R32_OPERATEWIDGET_H
