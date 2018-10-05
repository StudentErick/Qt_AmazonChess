#include "mainwindow.h"
#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(0));  // 随机数种子，全局初始化的
    MainWindow w;
    w.show();

    return a.exec();
}
