#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QTextCodec>
#include <QDir>
#include <QDirIterator>
#include "novracun.h"
#include "dodajpodjetje.h"
#include "dodajprodukt.h"
#include "oprogramu.h"
#include "arhiv.h"
#include "terjatve.h"
#include "vnosobveznosti.h"
#include "settings.h"
#include "statistic.h"
#include "kalkulacija.h"
#include "testingdialog.h"
#include "statistikaobveznosti.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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
    void on_actionTerjatve_obveznosti_triggered();
    void on_pushButton_5_clicked();
    void on_actionVnos_obveznosti_triggered();
    void on_pushButton_6_clicked();
    void on_actionNastavitve_triggered();
    void on_actionStatistika_triggered();
    void on_pushButton_7_clicked();
    void on_actionTesting_triggered();
    void on_pushButton_8_clicked();
    void on_actionKalkulacija_produkta_triggered();
    void CheckFiles();
    void CloseChild();
    void on_actionStatistika_obveznosti_triggered();
    void on_pushButton_9_clicked();

private:
    Ui::MainWindow *ui;
    QString m_copy_path;
    QString m_currentDir;
    QString m_arhiv_files;
    QString m_arhiv_login;
    QString m_arhiv_novRacun;
    QString m_arhiv_produkti;
    QString m_arhiv_stRacuna;
    QString m_arhiv_stranke;
    QString m_arhiv_upniki;
    QString m_arhiv_upnikiSeznam;
    QString m_company_file;
    QString m_delavni_proces;
    QString m_login_file;
    QString m_material;
    QString m_num_company;
    QString m_settings;
    bool m_show_child;
    QString m_saveRacun;
};

#endif // MAINWINDOW_H
