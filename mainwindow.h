#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "novracun.h"
#include "dodajpodjetje.h"
#include "dodajprodukt.h"
#include "oprogramu.h"
#include "arhiv.h"
#include "terjatve.h"
#include "vnosobveznosti.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionIzhod_triggered();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_actionDodaj_podjetje_triggered();
    void on_actionDodaj_produkt_triggered();
    void on_actionNov_racun_triggered();
    void on_actionO_programu_triggered();

    void on_actionArhiv_triggered();

    void on_pushButton_arhiv_clicked();

    void on_actionTerjatve_obveznosti_triggered();

    void on_pushButton_5_clicked();

    void on_actionVnos_obveznosti_triggered();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
