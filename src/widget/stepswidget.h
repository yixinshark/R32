//
// Created by zyz on 2023/5/22.
//

#ifndef R32_STEPSWIDGET_H
#define R32_STEPSWIDGET_H

#include <QMap>
#include <QWidget>

class StepWidget;

class StepsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StepsWidget(QWidget *parent = nullptr);
    ~StepsWidget() override;

    bool canRecordData();
    void reset();
    void stepComplete(quint8 step);

private:
    void initUI();

private:
    QList<StepWidget *> m_stepWidgets;
    QMap<quint8, StepWidget *> m_stepMap;
};

#endif //R32_STEPSWIDGET_H
