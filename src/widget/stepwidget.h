//
// Created by zyz on 2023/5/22.
//

#ifndef R32_STEPWIDGET_H
#define R32_STEPWIDGET_H

#include <QWidget>

class StepWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StepWidget(const QString &text, QWidget *parent = nullptr);
    ~StepWidget() override;

public:
    void completeStep();
    void reset();
    bool isCompleted();
    QString text() { return m_text; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_completed = false;
    QString m_text;
};

#endif //R32_STEPWIDGET_H
