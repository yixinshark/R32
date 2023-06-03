//
// Created by yixin on 2023/6/3.
//

#ifndef R32_MCUCTRLSTATUSWIDGET_H
#define R32_MCUCTRLSTATUSWIDGET_H

#include <QWidget>

class FanWidget;
class ValveWidget;
class QGridLayout;
/**
 * @brief MCU控制状态显示
 */
class McuCtrlStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit McuCtrlStatusWidget(QWidget *parent = nullptr);
    ~McuCtrlStatusWidget() override;

private:
    void initUi();
    void initFanWidgets();
    void initValveWidgets();

private:
    QGridLayout *m_gridLayout;
    QVector<FanWidget *> m_fanWidgets;
    QVector<ValveWidget *> m_valveWidgets;
};
#endif //R32_MCUCTRLSTATUSWIDGET_H
