#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon icon(":/icons/icon.ico");
    MainWindow w;
    w.setWindowIcon(icon);
    w.show();
    Login login;
    login.setModal(true);
    login.exec();
    return a.exec();
}
