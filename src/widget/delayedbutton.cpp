//
// Created by zyz on 23-5-12.
//


#include "delayedbutton.h"

#include <QTimer>
#include <QPainter>

DelayedButton::DelayedButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
    , m_timer(new QTimer(this))
{
    connect(this, &QPushButton::clicked, this, &DelayedButton::handleButtonClicked);

    m_timer->setSingleShot(true);
    m_timer->setInterval(2000); // 2秒

    connect(m_timer, &QTimer::timeout, [this]() {
        setEnabled(true);
    });
}

DelayedButton::~DelayedButton()
{

}

void DelayedButton::handleButtonClicked()
{
    setEnabled(false);
    m_timer->start();

    Q_EMIT delayedClicked();
}

void DelayedButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    if (!m_selected) {
        return;
    }

    // 在x的第5个像素开始，绘制一个直径为5的绿色圆圈作为提示
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::green);
    painter.drawEllipse(5, this->height()/2 - 2, 5, 5);
}

void DelayedButton::setSelected(bool selected)
{
    if (m_selected == selected) {
        return;
    }

    m_selected = selected;
    update();
}