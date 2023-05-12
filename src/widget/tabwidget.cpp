//
// Created by zyz on 23-5-12.
//

#include <QLabel>
#include <QEvent>
#include <QDebug>

#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLayout(new QHBoxLayout())
    , m_stackedLayout(new QStackedLayout())
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(m_titleLayout);
    mainLayout->addLayout(m_stackedLayout);
    setLayout(mainLayout);
}

TabWidget::~TabWidget()
{
}

void TabWidget::addTabs(const QStringList &tabs)
{
    for (const QString &tab : tabs) {
        QLabel* label = new QLabel(tab, this);
        label->setAlignment(Qt::AlignCenter);
        label->setAutoFillBackground(true);
        label->installEventFilter(this);
        m_titleLayout->addWidget(label);
        m_tabLabelMap.insert(tab, label);
    }
}

bool TabWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel *label = qobject_cast<QLabel*>(obj);
        if (label) {
            if (m_tabWidgetMap.contains(label->text())) {
                m_stackedLayout->setCurrentWidget(m_tabWidgetMap.value(label->text()));
            }

            setLabelSelected(label);
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void TabWidget::setLabelSelected(QLabel *selectedLabel)
{
    QPalette palette;
    QList<QLabel *> labels = m_tabLabelMap.values();
    for (QLabel *label : labels) {
        if (label == selectedLabel) {
            palette.setColor(QPalette::Background, Qt::lightGray);
        } else {
            palette.setColor(QPalette::Background, Qt::white);
        }

        label->setPalette(palette);
    }
}

void TabWidget::addWidget(const QString &tab, QWidget *widget)
{
    if (!widget || !m_tabLabelMap.contains(tab)) {
        return;
    }

    m_stackedLayout->addWidget(widget);
    m_tabWidgetMap.insert(tab, widget);
}

void TabWidget::setDefaultWidget(const QString &tab)
{
    if (!m_tabWidgetMap.contains(tab)) {
        m_stackedLayout->setCurrentIndex(0);
    }

    m_stackedLayout->setCurrentWidget(m_tabWidgetMap.value(tab));

    // 选中tab
    setLabelSelected(m_tabLabelMap.value(tab));
}
