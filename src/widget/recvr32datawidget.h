//
// Created by zyz on 23-5-13.
//

#ifndef R32_RECVR32DATAWIDGET_H
#define R32_RECVR32DATAWIDGET_H

#include "operatebasewidget.h"

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
};


#endif //R32_RECVR32DATAWIDGET_H
