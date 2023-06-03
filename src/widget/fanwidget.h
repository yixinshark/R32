//
// Created by yixin on 2023/6/3.
//

#ifndef R32_FANWIDGET_H
#define R32_FANWIDGET_H

#include <QWidget>

class FanWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FanWidget(QWidget *parent = nullptr);
    ~FanWidget() override;

public:
    void setFanOpened(bool isOpened);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer *m_timer;
    bool m_isOpened = false;
};


#endif //R32_FANWIDGET_H
