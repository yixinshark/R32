//
// Created by zyz on 23-5-12.
//

#include <QEvent>
#include <QDebug>

#include "tabwidget.h"
#include "labelbutton.h"

TabWidget::TabWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLayout(new QHBoxLayout())
    , m_stackedLayout(new QStackedLayout())
{
    m_titleLayout->setSpacing(3);

    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(5, 0, 5, 0);
    mainLayout->addLayout(m_titleLayout);
    mainLayout->addLayout(m_stackedLayout);
//    mainLayout->setSpacing(10);
    setLayout(mainLayout);
}

TabWidget::~TabWidget()
{
}

void TabWidget::addTabs(const QStringList &tabs)
{
    for (const QString &tab : tabs) {
        auto* label = new LabelButton(tab, this);
        //label->setAlignment(Qt::AlignCenter);
        //label->setAutoFillBackground(true);
        label->installEventFilter(this);
        m_titleLayout->addWidget(label);
        m_tabLabelMap.insert(tab, label);
    }
}

bool TabWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        auto *label = qobject_cast<LabelButton *>(obj);
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

void TabWidget::setLabelSelected(LabelButton *selectedLabel)
{
    QPalette palette;
    QList<LabelButton *> labels = m_tabLabelMap.values();
    for (LabelButton *label : labels) {
        if (label == selectedLabel) {
            label->setChecked(true);
        } else {
            label->setChecked(false);
        }
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
