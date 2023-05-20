//
// Created by zyz on 23-5-12.
//

#include "mainwidget.h"
#include "tabwidget.h"
#include "operatewidget.h"
#include "historydatawidget.h"

#include <QVBoxLayout>
#include <QTabWidget>

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
    setMinimumSize(900, 730);

    auto *tabWidget = new TabWidget(this);
    QStringList tabs;
    tabs << "R32传感器操作" << "数据查询与导出";
    tabWidget->addTabs(tabs);
    tabWidget->addWidget(tabs[0], m_operateWidget);
    tabWidget->addWidget(tabs[1], m_historyDataWidget);
    tabWidget->setDefaultWidget(tabs[0]);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
}
