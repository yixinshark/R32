//
// Created by zyz on 23-5-13.
//

#ifndef R32_SENDCMDWIDGET_H
#define R32_SENDCMDWIDGET_H

#include "operatebasewidget.h"

class SendCmdWidget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit SendCmdWidget(QWidget *parent = nullptr);
    ~SendCmdWidget() override;

protected:
    void recvAckData(int cmd, const QVariantMap &info);
    void sendDataBtnClicked();

private:
    void initUI();
};

#endif //R32_SENDCMDWIDGET_H
