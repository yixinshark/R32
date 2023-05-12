//
// Created by zyz on 23-5-12.
//

#ifndef R32_DELAYEDBUTTON_H
#define R32_DELAYEDBUTTON_H

#include <QPushButton>

class DelayedButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DelayedButton(QWidget *parent = nullptr);

    ~DelayedButton() override;

signals:
    void delayedClicked();

private slots:
    void handleButtonClicked();

private:
    QTimer *m_timer = nullptr;
};

#endif //R32_DELAYEDBUTTON_H
