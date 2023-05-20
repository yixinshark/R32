#include <QApplication>
#include <QFile>

#include "widget/mainwidget.h"
#include "dbutil/connectionpool.h"

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

    return app.exec();
}
