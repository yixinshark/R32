//
// Created by yixin on 2023/7/8.
//

#ifndef R32_ALARMWIDGET_H
#define R32_ALARMWIDGET_H

#include <QWidget>

class AlarmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = nullptr);
    ~AlarmWidget() override;

    void setLightStatus(int index, bool status);
    void reset();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    struct Light {
        int index;
        QColor status;
        QRect rect;
    };

    QList<Light> lights;
    const int rowCount = 4;
    const int columnCount = 16;
    const int beginY = 15;
    const int spacing = 8;
};


#endif //R32_ALARMWIDGET_H
