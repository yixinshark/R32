//
// Created by zyz on 23-5-12.
//

#ifndef R32_MAINWIDGET_H
#define R32_MAINWIDGET_H

#include <QWidget>

class QueryWidget;
class OperateWidget;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override;


private:
    void initUI();

private:
    QueryWidget *m_queryWidget = nullptr;
    OperateWidget *m_operateWidget = nullptr;
};

#endif //R32_MAINWIDGET_H
