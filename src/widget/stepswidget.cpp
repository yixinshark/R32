//
// Created by yixin on 2023/5/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_StepsWidget.h" resolved

#include "stepswidget.h"
#include "stepwidget.h"
#include "constant.h"

#include <QHBoxLayout>
#include <QPushButton>

StepsWidget::StepsWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

StepsWidget::~StepsWidget() {

}

void StepsWidget::initUI() {
    auto *stepWidget0 = new StepWidget("设置从机地址", this);
    m_stepMap.insert(SET_SLAVE_ADDR_CMD, stepWidget0);
    m_stepWidgets.append(stepWidget0);

    auto *stepWidget1 = new StepWidget("设置产品id", this);
    m_stepMap.insert(SET_ID_CMD, stepWidget1);
    m_stepWidgets.append(stepWidget1);

    auto *stepWidget2 = new StepWidget("读取软件版本", this);
    m_stepMap.insert(VER_CMD, stepWidget2);
    m_stepWidgets.append(stepWidget2);

    auto *stepWidget3 = new StepWidget("零点标点", this);
    m_stepMap.insert(LD_CMD, stepWidget3);
    m_stepWidgets.append(stepWidget3);

    auto *stepWidget4 = new StepWidget("读取NTC数据", this);
    m_stepMap.insert(NTC_CMD, stepWidget4);
    m_stepWidgets.append(stepWidget4);

    auto *stepWidget5 = new StepWidget("浓度标定", this);
    m_stepMap.insert(ND_CMD, stepWidget5);
    m_stepWidgets.append(stepWidget5);

    auto *stepWidget6 = new StepWidget("R32采样", this);
    m_stepMap.insert(R32_CMD, stepWidget6);
    m_stepWidgets.append(stepWidget6);

    auto *stepWidget7 = new StepWidget("记录数据", this);
    m_stepMap.insert(RECORD_DATA, stepWidget7);
    m_stepWidgets.append(stepWidget7);

    auto *resetBtn = new QPushButton("重置步进", this);
    resetBtn->setMinimumWidth(80);
    connect(resetBtn, &QPushButton::clicked, this, &StepsWidget::reset);

    auto* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 设置外边距(上左下右
    mainLayout->setSpacing(0);

    mainLayout->addWidget(resetBtn);
    mainLayout->addStretch();
    foreach (StepWidget *stepWidget, m_stepWidgets) {
        mainLayout->addWidget(stepWidget);
    }
    mainLayout->addStretch();
}

bool StepsWidget::canRecordData() {
    foreach (StepWidget* stepWidget, m_stepWidgets) {
        if (stepWidget->text() == "记录数据") {
            continue;
        }

        if (!stepWidget->isCompleted()) {
            return false;
        }
    }

    return true;
}

void StepsWidget::reset() {
    foreach (StepWidget* stepWidget, m_stepWidgets) {
        stepWidget->reset();
    }
}

void StepsWidget::stepComplete(quint8 step) {
    if (m_stepMap.contains(step)) {
        m_stepMap.value(step)->completeStep();
    }
}
