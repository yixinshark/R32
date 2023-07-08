//
// Created by yixin on 2023/7/8.
//

#include "alarmwidget.h"

#include <QPainter>
#include <QResizeEvent>

AlarmWidget::AlarmWidget(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setToolTip("");

    setMinimumSize(360, 100);

    // 初始化灯状态
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < columnCount; j++) {
            int index = i * columnCount + j;
            lights.append(Light{index, Qt::gray});
        }
    }

    // for test;
    // setLightStatus(0x011, true);
}

AlarmWidget::~AlarmWidget()
{

}

void AlarmWidget::setLightStatus(int index, bool status)
{
    // 通道从1开始
    index -= 1;
    if (index < 0 || index >= lights.size())
        return;

    lights[index].status = status ? Qt::green : Qt::red;
    update();
}

void AlarmWidget::reset()
{
    // 初始化灯状态
    for (auto &light : lights) {
        light.status = Qt::gray;
    }

    update();
}

void AlarmWidget::resizeEvent(QResizeEvent *event)
{
    int widgetWidth = size().width(); // event->size().width();
    int widgetHeight = size().height(); // event->size().height();

    int diameter=  (widgetWidth - columnCount * spacing) / columnCount;

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < columnCount; j++) {
            int index = i * columnCount + j;
            int x = j * (diameter + spacing);
            int y = i * (diameter + spacing) + beginY;
            QRect rect(x, y, diameter, diameter);
            lights[index].rect = rect;
        }
    }

    QWidget::resizeEvent(event);
}

void AlarmWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (const Light &light : lights) {
        painter.setBrush(light.status);
        painter.setPen(light.status);
        painter.drawEllipse(light.rect);
    }

    QWidget::paintEvent(event);
}

void AlarmWidget::mouseMoveEvent(QMouseEvent *event)
{
    int diameter=  (width() - columnCount * spacing) / columnCount;

    int column = event->x() / (diameter + spacing);
    int row =(event->y() - beginY) / (diameter + spacing);
    int index = row * columnCount + column;

    if (index >= 0 && index < lights.size()) {
        setToolTip(QString("0x%1").arg(index+1, 2, 16, QLatin1Char('0')));
    } else {
        setToolTip("");
    }

    QWidget::mouseMoveEvent(event);
}
