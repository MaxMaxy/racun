#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_copy_path(QDir::currentPath()), m_currentDir(QDir::currentPath()), m_arhiv_files(m_currentDir + "/arhiv_files.txt"), m_arhiv_login(m_currentDir + "/arhiv_login.txt"),
                            m_arhiv_novRacun(m_currentDir + "/arhiv_novRacun.txt"), m_arhiv_produkti(m_currentDir + "/arhiv_produkti.txt"), m_arhiv_stRacuna(m_currentDir + "/arhiv_stRacuna.txt"),
                            m_arhiv_stranke(m_currentDir + "/arhiv_stranke.txt"), m_arhiv_upniki(m_currentDir + "/arhiv_upniki.txt"), m_arhiv_upnikiSeznam(m_currentDir + "/arhiv_upnikiSeznam.txt"),
                            m_company_file(m_currentDir + "/company_file.txt"), m_delavni_proces(m_currentDir + "/delavni_proces.txt"), m_login_file(m_currentDir + "/login_file.txt"),
                            m_material(m_currentDir + "/material.txt"), m_num_company(m_currentDir + "/num_company.txt"), m_settings(m_currentDir + "/settings.txt"),
                            m_show_child(false), m_saveRacun(m_currentDir)
{
    ui->setupUi(this);
    QIcon icon(":icon/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Glavni meni");
    ui->pushButton->setFocus();
    MainWindow::setWindowIcon(icon);
    CheckFiles();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::CloseChild() {
  m_show_child = false;
}

void MainWindow::on_actionIzhod_triggered() {
    QApplication::quit();
}

// nov racun gumb
void MainWindow::on_pushButton_2_clicked() {
    MainWindow::on_actionNov_racun_triggered();
}

// dodaj podjetje gumb
void MainWindow::on_pushButton_3_clicked() {
    MainWindow::on_actionDodaj_podjetje_triggered();
}

// dodaj produkt gumb
void MainWindow::on_pushButton_4_clicked() {
    MainWindow::on_actionDodaj_produkt_triggered();
}

// terjatve gumb
void MainWindow::on_pushButton_5_clicked() {
    MainWindow::on_actionTerjatve_obveznosti_triggered();
}

// obveznosti gump
void MainWindow::on_pushButton_6_clicked() {
    MainWindow::on_actionVnos_obveznosti_triggered();
}

// terjatve statistika gumb
void MainWindow::on_pushButton_7_clicked() {
    MainWindow::on_actionStatistika_triggered();
}

// kalkulacija gumb
void MainWindow::on_pushButton_8_clicked() {
    MainWindow::on_actionKalkulacija_produkta_triggered();
}

// statistika obveznosti gumb
void MainWindow::on_pushButton_9_clicked() {
    MainWindow::on_actionStatistika_obveznosti_triggered();
}
// action dodaj podjetje
void MainWindow::on_actionDodaj_podjetje_triggered() {
    DodajPodjetje dodajPodjetje;
    dodajPodjetje.setModal(true);
    this->hide();
    QObject::connect(&dodajPodjetje,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        dodajPodjetje.exec();
    }
    this->show();
}

// action dodaj produkt
void MainWindow::on_actionDodaj_produkt_triggered() {
    DodajProdukt dodajProdukt;
    dodajProdukt.setModal(true);
    this->hide();
    QObject::connect(&dodajProdukt,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        dodajProdukt.exec();
    }
    this->show();
}

//action nov racun
void MainWindow::on_actionNov_racun_triggered() {
    NovRacun novRacun;
    novRacun.setModal(true);
    this->hide();
    QObject::connect(&novRacun,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        novRacun.exec();
    }
    this->show();
}

// action O programu
void MainWindow::on_actionO_programu_triggered() {
    Oprogramu program;
    program.setModal(true);
    this->hide();
    QObject::connect(&program,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        program.exec();
    }
    this->show();
}

// action Arhiv
void MainWindow::on_actionArhiv_triggered() {
    Arhiv arhiv;
    arhiv.setModal(true);
    this->hide();
    QObject::connect(&arhiv,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        arhiv.exec();
    }
    this->show();
}

// action terjatve/obveznosti
void MainWindow::on_actionTerjatve_obveznosti_triggered() {
    Terjatve terjatve;
    terjatve.setModal(true);
    this->hide();
    QObject::connect(&terjatve,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        terjatve.exec();
    }
    this->show();
}

// action vnos obveznosti
void MainWindow::on_actionVnos_obveznosti_triggered() {
    VnosObveznosti obveznost;
    obveznost.setModal(true);
    this->hide();
    QObject::connect(&obveznost,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        obveznost.exec();
    }
    this->show();
}

// action nastavitve
void MainWindow::on_actionNastavitve_triggered() {
    Settings settings;
    settings.setModal(true);
    this->hide();
    QObject::connect(&settings,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        settings.exec();
    }
    this->show();
}

// action statistika terjatve
void MainWindow::on_actionStatistika_triggered() {
    Statistic statistika;
    statistika.setModal(true);
    this->hide();
    QObject::connect(&statistika,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        statistika.exec();
    }
    this->show();
}

// action testing
void MainWindow::on_actionTesting_triggered() {
    TestingDialog testing;
    testing.setModal(true);
    this->hide();
    QObject::connect(&testing,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        testing.exec();
    }
    this->show();
}

// action kalkulacija
void MainWindow::on_actionKalkulacija_produkta_triggered() {
    Kalkulacija kalkulacija;
    kalkulacija.setModal(true);
    this->hide();
    QObject::connect(&kalkulacija,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        kalkulacija.exec();
    }
    this->show();
}

// action statistika obveznosti
void MainWindow::on_actionStatistika_obveznosti_triggered() {
    StatistikaObveznosti statistikaObveznosti;
    statistikaObveznosti.setModal(true);
    this->hide();
    QObject::connect(&statistikaObveznosti,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        statistikaObveznosti.exec();
    }
    this->show();
}

void MainWindow::CheckFiles() {
    QDateTime date = QDateTime::currentDateTime();
    QFile file(m_settings);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString shrani("Shrani v: ; " + m_saveRacun + " ;\n" + "Backup: ; " + m_copy_path + " ;\n");
            stream << shrani.toUtf8();
            file.flush();
            file.close();
            if(!QDir(m_saveRacun + "/Racuni_in_dobavnice").exists()) {
                QDir().mkdir(m_saveRacun + "/Racuni_in_dobavnice");
                QDir().mkdir(m_saveRacun + "/Racuni_in_dobavnice/" + "IZBRISANI");
            }
            if(!QDir(m_copy_path + "/ElraSetiBackup").exists()) {
                QDir().mkdir(m_copy_path + "/ElraSetiBackup");
                QDir().mkdir(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz"));
            }
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/settings.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/settings.txt");
            }
            QFile::copy(m_settings, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/settings.txt");
        } else {
            qDebug() << "Error settings";
        }
    } else {
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString text = stream.readAll();
            QRegularExpression exp(";");
            QStringList list = text.split(exp, QString::SkipEmptyParts);
            m_copy_path = list.at(3);
            m_saveRacun = list.at(1);
            m_copy_path.remove(" ");
            m_saveRacun.remove(" ");
            file.close();
            if(!QDir(m_saveRacun + "/Racuni_in_dobavnice").exists()) {
                QDir().mkdir(m_saveRacun + "/Racuni_in_dobavnice");
                QDir().mkdir(m_saveRacun + "/Racuni_in_dobavnice/" + "IZBRISANI");
            }
            if(!QDir(m_copy_path + "/ElraSetiBackup").exists()) {
                QDir().mkdir(m_copy_path + "/ElraSetiBackup");
                QDir().mkdir(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz"));
            }
            if(!QDir(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz")).exists())
                QDir().mkdir(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz"));
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/settings.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/settings.txt");
            }
            QFile::copy(m_settings, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/settings.txt");
        }
    }

    file.setFileName(m_arhiv_login);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString prijava("Prijava v sistem: 01.01.2018 - 00:00:00.000 ; ELRA\n");
            stream << prijava.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_login.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_login.txt");
            }
            QFile::copy(m_arhiv_login, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_login.txt");
        } else {
            qDebug() << "Error arhiv_files";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_login.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_login.txt");
        }
        QFile::copy(m_arhiv_login, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_login.txt");
        file.close();
    }

    file.setFileName(m_arhiv_novRacun);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_novRacun.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_novRacun.txt");
            }
            QFile::copy(m_arhiv_novRacun, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_novRacun.txt");
        } else {
            qDebug() << "Error arhiv_novRacun";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_novRacun.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_novRacun.txt");
        }
        QFile::copy(m_arhiv_novRacun, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_novRacun.txt");
        file.close();
    }

    file.setFileName(m_arhiv_produkti);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_produkti.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_produkti.txt");
            }
            QFile::copy(m_arhiv_produkti, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_produkti.txt");
        } else {
            qDebug() << "Error arhiv_produkti";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_produkti.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_produkti.txt");
        }
        QFile::copy(m_arhiv_produkti, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_produkti.txt");
        file.close();
    }

    file.setFileName(m_arhiv_stRacuna);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString num("0\n");
            stream << num.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stRacuna.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stRacuna.txt");
            }
            QFile::copy(m_arhiv_stRacuna, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stRacuna.txt");
        } else {
            qDebug() << "Error arhiv_stRacuna";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stRacuna.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stRacuna.txt");
        }
        QFile::copy(m_arhiv_stRacuna, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stRacuna.txt");
        file.close();
    }

    file.setFileName(m_arhiv_stranke);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stranke.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stranke.txt");
            }
            QFile::copy(m_arhiv_stranke, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stranke.txt");
        } else {
            qDebug() << "Error arhiv_stranke";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stranke.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stranke.txt");
        }
        QFile::copy(m_arhiv_stranke, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_stranke.txt");
        file.close();
    }

    file.setFileName(m_arhiv_upniki);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upniki.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upniki.txt");
            }
            QFile::copy(m_arhiv_upniki, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upniki.txt");
        } else {
            qDebug() << "Error arhiv_upniki";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upniki.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upniki.txt");
        }
        QFile::copy(m_arhiv_upniki, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upniki.txt");
        file.close();
    }

    file.setFileName(m_arhiv_upnikiSeznam);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upnikiSeznam.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upnikiSeznam.txt");
            }
            QFile::copy(m_arhiv_upnikiSeznam, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upnikiSeznam.txt");
        } else {
            qDebug() << "Error arhiv_upnikiSeznam";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upnikiSeznam.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upnikiSeznam.txt");
        }
        QFile::copy(m_arhiv_upnikiSeznam, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_upnikiSeznam.txt");
        file.close();
    }

    file.setFileName(m_company_file);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/company_file.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/company_file.txt");
            }
            QFile::copy(m_company_file, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/company_file.txt");
        } else {
            qDebug() << "Error company_file";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/company_file.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/company_file.txt");
        }
        QFile::copy(m_company_file, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/company_file.txt");
        file.close();
    }

    file.setFileName(m_delavni_proces);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("Test;0.00; ; ; ;\n");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if (QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/delavni_proces.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/delavni_proces.txt");
            }
            QFile::copy(m_delavni_proces, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/delavni_proces.txt");
        } else {
            qDebug() << "Error delavni_proces";
        }
    } else {
        if (QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/delavni_proces.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/delavni_proces.txt");
        }
        QFile::copy(m_delavni_proces, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/delavni_proces.txt");
        file.close();
    }

    file.setFileName(m_login_file);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString admin("Admin\n");
            stream << admin.toUtf8();
            file.flush();
            file.close();
            if (QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/login_file.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/login_file.txt");
            }
            QFile::copy(m_login_file, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/login_file.txt");
        } else {
            qDebug() << "Error login_file";
        }
    } else {
        if (QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/login_file.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/login_file.txt");
        }
        QFile::copy(m_login_file, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/login_file.txt");
        file.close();
    }

    file.setFileName(m_material);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("Test;Test;0.00;0;kos;\n");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/material.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/material.txt");
            }
            QFile::copy(m_material, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/material.txt");
        } else {
            qDebug() << "Error material";
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/material.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/material.txt");
        }
        QFile::copy(m_material, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/material.txt");
        file.close();
    }

    file.setFileName(m_num_company);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/num_company.txt")) {
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/num_company.txt");
            }
            QFile::copy(m_num_company, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/num_company.txt");
        } else {
            qDebug() << "Error num_company";
        }
    } else {
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString lastLine("");
            while(!stream.atEnd()) {
                lastLine = stream.readLine();
            }
            for(int i(1); i < lastLine.toInt(); i++) {
                if (QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/" + QString::number(i) + ".txt")) {
                    QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/" + QString::number(i) + ".txt");
                }
                QFile::copy(m_currentDir + "/" + QString::number(i) + ".txt", m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/" + QString::number(i) + ".txt");
            }
            file.close();
        }
        file.close();
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/num_company.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/num_company.txt");
        }
        QFile::copy(m_num_company, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/num_company.txt");
    }

    file.setFileName(m_arhiv_files);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QStringList list;
            QStringList tmp;
            QString tmp_text;
            QRegularExpression exp("/");
            QDirIterator dirIt(QDir::currentPath());
            while (dirIt.hasNext()) {
                dirIt.next();
                if (QFileInfo(dirIt.filePath()).isFile())
                    if (QFileInfo(dirIt.filePath()).suffix() == "txt") {
                        tmp_text = dirIt.filePath();
                        tmp_text.remove(".txt");
                        tmp = tmp_text.split(exp, QString::SkipEmptyParts);
                        list.append(tmp.at(tmp.size()-1) + "\n");
                    }
            }
            for(int i(0); i < list.size(); i++)
                stream << list.at(i).toUtf8();
            file.flush();
            file.close();
            if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_files.txt"))
                QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_files.txt");
            QFile::copy(m_arhiv_files, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_files.txt");
        }
    } else {
        if(QFile::exists(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_files.txt")) {
            QFile::remove(m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_files.txt");
            if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                QStringList list;
                QStringList tmp;
                QString tmp_text;
                QRegularExpression exp("/");
                QDirIterator dirIt(QDir::currentPath());
                while (dirIt.hasNext()) {
                    dirIt.next();
                    if (QFileInfo(dirIt.filePath()).isFile())
                        if (QFileInfo(dirIt.filePath()).suffix() == "txt") {
                            tmp_text = dirIt.filePath();
                            tmp_text.remove(".txt");
                            tmp = tmp_text.split(exp, QString::SkipEmptyParts);
                            list.append(tmp.at(tmp.size()-1) + "\n");
                        }
                }
                for(int i(0); i < list.size(); i++)
                    stream << list.at(i).toUtf8();
                file.flush();
                file.close();
            }
        }
        QFile::copy(m_arhiv_files, m_copy_path + "/ElraSetiBackup/" + date.toString("dd_MM_yyyy_hh_mm_ss_zzz") + "/arhiv_files.txt");
        file.close();
    }
}
