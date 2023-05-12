//
// Created by zyz on 23-5-12.
//

#ifndef R32_TABWIDGET_H
#define R32_TABWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QMap>

class QLabel;

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);
    ~TabWidget() override;

    void addTabs(const QStringList &tabs);
    void addWidget(const QString &tab, QWidget *widget);
    void setDefaultWidget(const QString &tab);

    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void setLabelSelected(QLabel* label);

private:
    QMap<QString, QLabel *> m_tabLabelMap;
    QMap<QString, QWidget *> m_tabWidgetMap;

    QHBoxLayout *m_titleLayout = nullptr;
    QStackedLayout *m_stackedLayout = nullptr;
};


#endif //R32_TABWIDGET_H
