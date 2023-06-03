//
// Created by yixin on 2023/6/3.
//

#include "valvewidget.h"

#include <QTimer>
#include <QPainter>

ValveWidget::ValveWidget(QWidget *parent)
    :QWidget(parent)
{
    setFixedSize(40, 30);
}

ValveWidget::~ValveWidget()
{
}

void ValveWidget::setValveOpened(bool isOpened)
{
    m_isOpened = isOpened;
    update();
}

void ValveWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);

    // 绘制阀体
    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);
    painter.drawRect(-15, -15, 30, 30);

    // 绘制阀盖
    if (m_isOpened) {
        painter.setBrush(Qt::green);
    } else {
        painter.setBrush(Qt::gray);
    }
    painter.drawRect(20, -10, -5, 20);

    // 绘制阀门
    if (m_isOpened) {
        painter.setBrush(Qt::green);
        painter.drawRect(-10, -5, -5, 10);
    } else {
        painter.setBrush(Qt::gray);
        painter.drawRect(-10, -5, -5, 10);
    }

    QWidget::paintEvent(event);
}
