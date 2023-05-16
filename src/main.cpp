#include <QApplication>

#include "widget/mainwidget.h"
#include "dbutil/connectionpool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 初始化数据库连接池
    Singleton<ConnectionPool>::getInstance();

    MainWidget mainWidget;
    mainWidget.show();

    return a.exec();
}
