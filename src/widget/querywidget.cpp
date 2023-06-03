//
// Created by zyz on 2023/5/21.
//

#include "querywidget.h"
#include "sqls.h"
#include "connectionpool.h"

#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFileDialog>
#include <QAxObject>

const char * const SQL_R32TABLE = "R32Table";
QueryWidget::QueryWidget(QWidget *parent)
    : QWidget(parent)
    , m_startTimeEdit(new QDateTimeEdit(this))
    , m_endTimeEdit(new QDateTimeEdit(this))
    , m_concentrationEdit(new QLineEdit(this))
    , m_precisionEdit(new QLineEdit(this))
    , m_queryButton(new QPushButton("查询",this))
    , m_exportButton(new QPushButton("导出",this))
    , m_tableView(new QTableView(this))
    , m_queryModel(new QSqlQueryModel(this))
{
    initUI();

    // 设置连接槽函数
    connect(m_queryButton, &QPushButton::clicked, this, &QueryWidget::handleQueryButton);
    connect(m_exportButton, &QPushButton::clicked, this, &QueryWidget::handleExportButton);
}

QueryWidget::~QueryWidget()
{

}

void QueryWidget::initUI() {
    m_startTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    m_endTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    m_startTimeEdit->setCalendarPopup(true);
    m_endTimeEdit->setCalendarPopup(true);
    m_startTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-1));
    m_endTimeEdit->setDateTime(QDateTime::currentDateTime());

    m_concentrationEdit->setMaximumWidth(40);
    m_precisionEdit->setMaximumWidth(40);

    auto *hLayout = new QHBoxLayout;
    hLayout->setSpacing(10);
    hLayout->addWidget(new QLabel("开始时间:"));
    hLayout->addWidget(m_startTimeEdit);
    hLayout->addWidget(new QLabel("结束时间:"));
    hLayout->addWidget(m_endTimeEdit);
    hLayout->addWidget(new QLabel("标准浓度值:"));
    hLayout->addWidget(m_concentrationEdit);
    hLayout->addWidget(new QLabel("精度值%:"));
    hLayout->addWidget(m_precisionEdit);
    hLayout->addStretch();
    hLayout->addWidget(m_queryButton);
    hLayout->addWidget(m_exportButton);

    auto *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(m_tableView);

    m_tableView->setModel(m_queryModel);

    m_queryModel->setHeaderData(0, Qt::Horizontal, "id");
    m_queryModel->setHeaderData(1, Qt::Horizontal, "流水号");
    m_queryModel->setHeaderData(2, Qt::Horizontal, "时间");
    m_queryModel->setHeaderData(3, Qt::Horizontal, "产品id");
    m_queryModel->setHeaderData(4, Qt::Horizontal, "软件版本");
    m_queryModel->setHeaderData(5, Qt::Horizontal, "ADC值");
    m_queryModel->setHeaderData(6, Qt::Horizontal, "浓度值");
}

void QueryWidget::handleQueryButton() {

    m_queryModel->clear();

    QString startTime = m_startTimeEdit->text();
    QString endTime = m_endTimeEdit->text();
    QString concentration = m_concentrationEdit->text();
    QString precision = m_precisionEdit->text();

    // 检查时间范围是否为两天内
    QDateTime startDateTime = QDateTime::fromString(startTime, "yyyy-MM-dd hh:mm:ss");
    QDateTime endDateTime = QDateTime::fromString(endTime, "yyyy-MM-dd hh:mm:ss");
    if (startDateTime.daysTo(endDateTime) > 2) {
        QMessageBox::warning(this, "错误", "时间范围必须在两天之内");
        return;
    }

    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_R32TABLE, "findByDateTime");
    sql = sql.arg(startTime).arg(endTime);
    qDebug() << "sql:" << sql;

    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().openConnection();
    QSqlQuery query(db);
    query.prepare(sql);
    if (!query.exec()) {
        Singleton<ConnectionPool>::getInstance().closeConnection(db);
        QMessageBox::warning(this, "查询失败", "查询失败");
        return;
    }

    // 设置查询结果到查询模型
    m_queryModel->setQuery(query);
    Singleton<ConnectionPool>::getInstance().closeConnection(db);
}

void QueryWidget::handleExportButton() {
    QString filename = QFileDialog::getSaveFileName(this, "选择保存路径", "", "Excel 文件 (*.xlsx)");
    if (filename.isEmpty()) {
        return;
    }

    exportToExcel(filename);
}

void QueryWidget::exportToExcel(const QString &filename) {
    QAxObject *excel = new QAxObject("Excel.Application", this);
    excel->dynamicCall("SetVisible(bool)", false);
    QAxObject *workbooks = excel->querySubObject("Workbooks");
    QAxObject *workbook = workbooks->querySubObject("Add");
    QAxObject *worksheets = workbook->querySubObject("Worksheets");
    QAxObject *worksheet = worksheets->querySubObject("Item(int)", 1);

    // 写入表头
    QAxObject *cell = worksheet->querySubObject("Cells(int,int)", 1, 1);
    cell->setProperty("Value", "ID");
    cell = worksheet->querySubObject("Cells(int,int)", 1, 2);
    cell->setProperty("Value", "流水号");
    cell = worksheet->querySubObject("Cells(int,int)", 1, 3);
    cell->setProperty("Value", "时间");
    cell = worksheet->querySubObject("Cells(int,int)", 1, 4);
    cell->setProperty("Value", "传感器ID");
    cell = worksheet->querySubObject("Cells(int,int)", 1, 5);
    cell->setProperty("Value", "软件版本号");
    cell = worksheet->querySubObject("Cells(int,int)", 1, 6);
    cell->setProperty("Value", "ADC值");
    cell = worksheet->querySubObject("Cells(int,int)", 1, 7);
    cell->setProperty("Value", "浓度值");
    cell = worksheet->querySubObject("Cells(int,int)", 1, 8);
    cell->setProperty("Value", "结果信息");

    // 写入数据
    for (int row = 0; row < m_queryModel->rowCount(); ++row) {
        for (int column = 0; column < m_queryModel->columnCount(); ++column) {
            cell = worksheet->querySubObject("Cells(int,int)", row + 2, column + 1);
            cell->setProperty("Value", m_queryModel->data(m_queryModel->index(row, column)).toString());
        }

        // 计算结果信息
        QString concentration = m_queryModel->data(m_queryModel->index(row, 6)).toString();
        double userConcentration = m_concentrationEdit->text().toDouble();
        double precision = m_precisionEdit->text().toDouble();
        // 根据需要计算结果信息并写入单元格
        bool in = isWithinRange(concentration.toDouble(), userConcentration, precision);
        if (in) {
            cell = worksheet->querySubObject("Cells(int,int)", row + 2, 8);
            cell->setProperty("Value", "合格");
        } else {
            cell = worksheet->querySubObject("Cells(int,int)", row + 2, 8);
            cell->setProperty("Value", "不合格");
        }
    }

    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filename));
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    excel->deleteLater();
}

bool QueryWidget::isWithinRange(double value, double standardValue, double precision) {
    qInfo() << "value:" << value << "standardValue:" << standardValue << "precision:" << precision;
    double difference = qAbs(value - standardValue);
    double percentDifference = (difference / standardValue) * 100;

    return percentDifference <= precision;
}
