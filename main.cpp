#include "mainwindow.h"
#include <QApplication>
#include "udpclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qDebug("%s", __func__);

    return a.exec();
}
