//
// Created by zyz on 23-5-14.
//

#include "statuswidget.h"

#include <QPainter>

StatusWidget::StatusWidget(QWidget *parent)
    : QWidget(parent)
{
}

StatusWidget::~StatusWidget()
{
}

void StatusWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_selected) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::green);
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::gray);
    }

    int circleSize = 10;
    int centerX = width() / 2;
    int centerY = height() / 2;

    painter.drawEllipse(centerX - circleSize / 2, centerY - circleSize / 2, circleSize, circleSize);

    QWidget::paintEvent(event);
}

void StatusWidget::setSelected(bool isSelected)
{
    if (m_selected != isSelected) {
        m_selected = isSelected;
        update();  // 更新窗口绘制
    }
}
