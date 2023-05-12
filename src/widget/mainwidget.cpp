//
// Created by zyz on 23-5-12.
//

#include "mainwidget.h"
#include "tabwidget.h"
#include "operatewidget.h"
#include "historydatawidget.h"

#include <QVBoxLayout>
#include <QTabWidget>
#include <QTabBar>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , m_operateWidget(new OperateWidget(this))
    , m_historyDataWidget(new HistoryDataWidget(this))
{
    initUI();
}

MainWidget::~MainWidget()
{

}

void MainWidget::initUI()
{
    setMinimumSize(800, 600);

    TabWidget *tabWidget = new TabWidget(this);
    QStringList tabs;
    tabs << "Operation" << "History";
    tabWidget->addTabs(tabs);
    tabWidget->addWidget(tabs[0], m_operateWidget);
    tabWidget->addWidget(tabs[1], m_historyDataWidget);
    tabWidget->setDefaultWidget(tabs[0]);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
}
