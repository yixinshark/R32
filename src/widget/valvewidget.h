//
// Created by yixin on 2023/6/3.
//

#ifndef R32_VALVEWIDGET_H
#define R32_VALVEWIDGET_H

#include <QWidget>

class ValveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ValveWidget(QWidget *parent = nullptr);
    ~ValveWidget() override;

public:
    void setValveOpened(bool isOpened);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isOpened = false;
};

#endif //R32_VALVEWIDGET_H
