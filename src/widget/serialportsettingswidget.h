//
// Created by zyz on 23-5-13.
//

#ifndef R32_SERIALPORTSETTINGSWIDGET_H
#define R32_SERIALPORTSETTINGSWIDGET_H

#include <QWidget>

class QComboBox;

class SerialPortSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SerialPortSettingsWidget(QWidget *parent = nullptr);
    ~SerialPortSettingsWidget() override;

    QString getSelectedPort() const;
    qint32 getSelectedBaudRate() const;
    QString getSelectedParity() const;
    int getSelectedDataBits() const;
    QString getSelectedStopBits() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void initUI();
    void updateAvailablePorts() const;
    void styleForWindows();

private:
    QComboBox *m_portComboBox;
    QComboBox *m_baudRateComboBox;
    QComboBox *m_parityComboBox;
    QComboBox *m_dataBitsComboBox;
    QComboBox *m_stopBitsComboBox;
};

#endif //R32_SERIALPORTSETTINGSWIDGET_H
