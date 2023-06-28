//
// Created by zyz on 23-5-13.
//

#include "labelbutton.h"

#include <QPainter>

LabelButton::LabelButton(const QString &text, QWidget *parent)
    : QLabel(parent)
{
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    setFont(font);

    setMinimumHeight(30);
    setAlignment(Qt::AlignCenter);
    setText(text);
}

LabelButton::~LabelButton()
{

}

void LabelButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 防锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 绘制浅色背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 20));
    painter.drawRect(rect());

    if (m_checked) {
        // 下边缘绘制两个像素高的蓝色线条
        painter.setPen(QColor(0, 0, 255, 100));
        painter.drawLine(0, height() - 2, width(), height() - 2);
        painter.drawLine(0, height() - 1, width(), height() - 1);
    }

    if (m_hovered) {
        // 绘制半透明黑色背景
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 50));
        painter.drawRect(rect());
    }

    QLabel::paintEvent(event);
}

void LabelButton::enterEvent(QEvent *event)
{
    m_hovered = true;
    update();

    QWidget::enterEvent(event);
}

void LabelButton::leaveEvent(QEvent *event)
{
    m_hovered = false;
    update();

    QWidget::leaveEvent(event);
}

void LabelButton::setChecked(bool checked)
{
    m_checked = checked;
    update();
}
