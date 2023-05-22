//
// Created by zyz on 2023/5/22.
//

#include "stepwidget.h"

#include <QPainter>

StepWidget::StepWidget(const QString &text, QWidget *parent)
    : QWidget(parent)
    , m_text(text)
{
    QFontMetrics fontMetrics(font());
    int textWidth = fontMetrics.horizontalAdvance(text);
    int width = textWidth + 36;
    setFixedWidth(width);
    setMinimumHeight(40);
}

StepWidget::~StepWidget() {

}

void StepWidget::completeStep() {
    m_completed = true;
    update();
}

void StepWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

//    painter.drawRect(this->rect());

    int width = this->width();
    int height = this->height();
    int lineY = 6;
    int circleRadius = 4;
    int circleDiameter = circleRadius * 2;
    int circleX = (width - circleDiameter) / 2;
    int circleY = lineY - circleRadius;

    // Draw the gray line
    painter.setPen(Qt::gray);
    painter.drawLine(0, lineY, width, lineY);

    // Draw the circle
    if (m_completed) {
        painter.setBrush(Qt::green);
    } else {
        painter.setBrush(Qt::gray);
    }
    painter.drawEllipse(circleX, circleY, circleDiameter, circleDiameter);

    // Draw the text
    painter.setPen(Qt::black);
    painter.drawText(0, lineY + 10, width, height - lineY - 10, Qt::AlignCenter, m_text);
}

void StepWidget::reset() {
    m_completed = false;
    update();
}

bool StepWidget::isCompleted() {
    return m_completed;
}
