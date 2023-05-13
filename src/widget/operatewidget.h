//
// Created by zyz on 23-5-12.
//

#ifndef R32_OPERATEWIDGET_H
#define R32_OPERATEWIDGET_H

#include <QWidget>

class SendCmdWidget;
class RecvR32DataWidget;

class OperateWidget : public QWidget {
Q_OBJECT

public:
    explicit OperateWidget(QWidget *parent = nullptr);
    ~OperateWidget() override;

private:
    void initUI();

private:
    SendCmdWidget *m_sendCmdWidget;
    RecvR32DataWidget *m_recvR32DataWidget;
};


#endif //R32_OPERATEWIDGET_H
