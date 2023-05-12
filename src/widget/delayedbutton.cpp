//
// Created by zyz on 23-5-12.
//


#include "delayedbutton.h"

#include <QTimer>

DelayedButton::DelayedButton(QWidget *parent)
    : QPushButton(parent)
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
