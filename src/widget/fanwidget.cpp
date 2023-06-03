//
// Created by yixin on 2023/6/3.
//

#include "fanwidget.h"

#include <QTimer>
#include <QPainter>

FanWidget::FanWidget(QWidget *parent)
    : QWidget(parent)
    , m_timer(new QTimer(this))
{
    setFixedSize(30, 30);
    m_timer->setInterval(100);
    connect(m_timer, &QTimer::timeout, this, [this](){
        update();
    });
}

FanWidget::~FanWidget()
{
}

void FanWidget::setFanOpened(bool isOpened)
{
    m_isOpened = isOpened;
    if (isOpened) {
        m_timer->start();
    } else {
        m_timer->stop();
        update();
    }
}

void FanWidget::paintEvent(QPaintEvent *event)
{static int rotationAngle = 0;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);

    rotationAngle += 20;
    if (rotationAngle >= 360) {
        rotationAngle = 0;
    }

    // 绘制风扇叶片
    painter.setPen(Qt::NoPen);
    if (m_isOpened) {
        painter.setBrush(Qt::green);
        painter.save();
        painter.rotate(rotationAngle);
        painter.drawPie(-15, -15, 30, 30, 45 * 16, 120 * 16);
        painter.restore();

        painter.setBrush(Qt::magenta);
        painter.save();
        painter.rotate(rotationAngle + 120);
        painter.drawPie(-15, -15, 30, 30, 45 * 16, 120 * 16);
        painter.restore();

        painter.setBrush(Qt::yellow);
        painter.save();
        painter.rotate(rotationAngle + 240);
        painter.drawPie(-15, -15, 30, 30, 45 * 16, 120 * 16);
        painter.restore();
    } else {
        painter.setBrush(Qt::lightGray);
        painter.drawPie(-15, -15, 30, 30, 45 * 16, 120 * 16);
        painter.setBrush(Qt::lightGray);
        painter.drawPie(-15, -15, 30, 30, 165 * 16, 120 * 16);
        painter.setBrush(Qt::lightGray);
        painter.drawPie(-15, -15, 30, 30, 285 * 16, 120 * 16);
    }

    // 绘制风扇中心
    painter.setBrush(Qt::gray);
    painter.drawEllipse(-3, -3, 6, 6);

    QWidget::paintEvent(event);
}
