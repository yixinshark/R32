//
// Created by zyz on 23-5-14.
//

#ifndef R32_STATUSWIDGET_H
#define R32_STATUSWIDGET_H

#include <QWidget>

class StatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusWidget(QWidget *parent = nullptr);
    ~StatusWidget() override;

    void setSelected(bool isSelected);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    bool m_selected = false;
};


#endif //R32_STATUSWIDGET_H
