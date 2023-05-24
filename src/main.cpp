#include <QApplication>
#include <QFile>
#include <QDebug>

#include "widget/mainwidget.h"
#include "dbutil/connectionpool.h"
#include "r32recordvalue.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWidget mainWidget;
    mainWidget.show();

#ifdef Q_OS_WIN
    // qss 文件
    QFile File(":src/res/r32.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QString::fromLatin1(File.readAll());
    app.setStyleSheet(StyleSheet);
    File.close();
#endif

    // 数据库链接
    ConnectionPool &pool = Singleton<ConnectionPool>::getInstance();
    qInfo() << "database connected:" << pool.connected();
    if (pool.connected()) {
        bool ret = R32RecordValueDao::createTable();
        qInfo() << "database create table ret:" << ret;
    }

    return app.exec();
}
