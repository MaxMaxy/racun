#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/font/average_sans_regular.ttf");
    QIcon icon(":/icons/icon.ico");
    MainWindow w;
    w.setWindowIcon(icon);
    w.show();
    //Login login;
    //login.setModal(true);
    //login.exec();
    return a.exec();
}
