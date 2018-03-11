#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon(":icon/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Glavni meni");
    MainWindow::setWindowIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionIzhod_triggered()
{
    QApplication::quit();
}

// nov racun gumb
void MainWindow::on_pushButton_2_clicked()
{
    MainWindow::on_actionNov_racun_triggered();
}

// dodaj podjetje gumb
void MainWindow::on_pushButton_3_clicked()
{
    MainWindow::on_actionDodaj_podjetje_triggered();
}

// dodaj produkt gumb
void MainWindow::on_pushButton_4_clicked()
{
    MainWindow::on_actionDodaj_produkt_triggered();
}

// terjatve gumb
void MainWindow::on_pushButton_5_clicked()
{
    MainWindow::on_actionTerjatve_obveznosti_triggered();
}

// obveznosti gump
void MainWindow::on_pushButton_6_clicked()
{
    MainWindow::on_actionVnos_obveznosti_triggered();
}

// statistika gumb
void MainWindow::on_pushButton_7_clicked()
{
    MainWindow::on_actionStatistika_triggered();
}

// action dodaj podjetje
void MainWindow::on_actionDodaj_podjetje_triggered()
{
    DodajPodjetje dodajPodjetje;
    dodajPodjetje.setModal(true);
    dodajPodjetje.exec();
}

// action dodaj produkt
void MainWindow::on_actionDodaj_produkt_triggered()
{
    DodajProdukt dodajProdukt;
    dodajProdukt.setModal(true);
    dodajProdukt.exec();
}

//action nov racun
void MainWindow::on_actionNov_racun_triggered()
{
    NovRacun novRacun;
    novRacun.setModal(true);
    novRacun.exec();
}

// action O programu
void MainWindow::on_actionO_programu_triggered()
{
    Oprogramu program;
    program.setModal(true);
    program.exec();
}

// action Arhiv
void MainWindow::on_actionArhiv_triggered()
{
    Arhiv arhiv;
    arhiv.setModal(true);
    arhiv.exec();
}

// action terjatve/obveznosti
void MainWindow::on_actionTerjatve_obveznosti_triggered()
{
    Terjatve terjatve;
    terjatve.setModal(true);
    terjatve.exec();
}

// action vnos obveznosti
void MainWindow::on_actionVnos_obveznosti_triggered()
{
    VnosObveznosti obveznost;
    obveznost.setModal(true);
    obveznost.exec();
}

void MainWindow::on_actionNastavitve_triggered()
{
    Settings settings;
    settings.setModal(true);
    settings.exec();
}

void MainWindow::on_actionStatistika_triggered()
{
    Statistic statistika;
    statistika.setModal(true);
    statistika.exec();
}

void MainWindow::on_actionTesting_triggered()
{
    TestingDialog testing;
    testing.setModal(true);
    testing.exec();
}
