//
// Created by zyz on 23-5-13.
//

#ifndef R32_RECVR32DATAWIDGET_H
#define R32_RECVR32DATAWIDGET_H

#include "operatebasewidget.h"

class QLineEdit;
class QVBoxLayout;

/**
 * @brief 接收R32分析仪数据
 */
class RecvR32DataWidget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit RecvR32DataWidget(QWidget *parent = nullptr);
    ~RecvR32DataWidget() override;

private:
    void initUI();

private:
    QVBoxLayout *m_mainLayout;
};


#endif //R32_RECVR32DATAWIDGET_H
