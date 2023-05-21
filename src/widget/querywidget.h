//
// Created by zyz on 2023/5/21.
//

#ifndef R32_QUERYWIDGET_H
#define R32_QUERYWIDGET_H

#include <QWidget>

class QDateTimeEdit;
class QLineEdit;
class QPushButton;
class QTableView;
class QSqlQueryModel;

class QueryWidget : public QWidget {
Q_OBJECT

public:
    explicit QueryWidget(QWidget *parent = nullptr);
    ~QueryWidget() override;

private slots:
    void handleQueryButton();
    void handleExportButton();

private:
    void initUI();
    void exportToExcel(const QString &filename);
    bool isWithinRange(double value, double standardValue, double precision);

private:
    QDateTimeEdit *m_startTimeEdit;
    QDateTimeEdit *m_endTimeEdit;
    QLineEdit *m_concentrationEdit;
    QLineEdit *m_precisionEdit;
    QPushButton *m_queryButton;
    QPushButton *m_exportButton;
    QTableView *m_tableView;

    QSqlQueryModel *m_queryModel;
};

#endif //R32_QUERYWIDGET_H
