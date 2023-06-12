//
// Created by zyz on 23-5-13.
//

#ifndef R32_RECVR32DATAWIDGET_H
#define R32_RECVR32DATAWIDGET_H

#include "operatebasewidget.h"

class QLineEdit;
class QGridLayout;

/**
 * @brief 接收R32分析仪数据
 */
class RecvR32DataWidget : public OperateBaseWidget
{
    Q_OBJECT
public:
    explicit RecvR32DataWidget(HandleDataBase *handleData, QWidget *parent = nullptr);
    ~RecvR32DataWidget() override;

signals:
    void r32NDValue(quint16 nd);

protected:
    void recvAckData(quint8 cmd, const QVariantMap &info) override;

private:
    void initUI();
    void initReadR32InfoUI();
    void showR32Info(const QVariantMap &info);

private:
    QGridLayout *m_gridLayout;

    // R32
    QLineEdit *m_showAlarmValue;
    QLineEdit *m_showR32NDValue;
};


#endif //R32_RECVR32DATAWIDGET_H
