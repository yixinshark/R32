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
    explicit DelayedButton(const QString &text, QWidget *parent = nullptr);
    ~DelayedButton() override;

    void setSelected(bool selected);

signals:
    void delayedClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void handleButtonClicked();

private:
    QTimer *m_timer = nullptr;
    bool m_selected = false;
};

#endif //R32_DELAYEDBUTTON_H
