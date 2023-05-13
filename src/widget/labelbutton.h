//
// Created by zyz on 23-5-13.
//

#ifndef R32_LABELBUTTON_H
#define R32_LABELBUTTON_H

#include <QLabel>

class LabelButton : public QLabel
{
    Q_OBJECT
public:
    explicit LabelButton(const QString &text, QWidget *parent = nullptr);
    ~LabelButton() override;


    void setChecked(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    bool m_checked = false;
    bool m_hovered = false;
};


#endif //R32_LABELBUTTON_H
