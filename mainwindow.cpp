#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_copy_path("F:/ElraSetiBackup/"), m_currentDir(QDir::currentPath()), m_arhiv_files(m_currentDir + "/arhiv_files.txt"), m_arhiv_login(m_currentDir + "/arhiv_login.txt"),
                            m_arhiv_novRacun(m_currentDir + "/arhiv_novRacun.txt"), m_arhiv_produkti(m_currentDir + "/arhiv_produkti.txt"), m_arhiv_stRacuna(m_currentDir + "/arhiv_stRacuna.txt"),
                            m_arhiv_stranke(m_currentDir + "/arhiv_stranke.txt"), m_arhiv_upniki(m_currentDir + "/arhiv_upniki.txt"), m_arhiv_upnikiSeznam(m_currentDir + "/arhiv_upnikiSeznam.txt"),
                            m_company_file(m_currentDir + "/company_file.txt"), m_delavni_proces(m_currentDir + "/delavni_proces.txt"), m_login_file(m_currentDir + "/login_file.txt"),
                            m_material(m_currentDir + "/material.txt"), m_num_company(m_currentDir + "/num_company.txt"), m_settings(m_currentDir + "/settings.txt"),
                            m_show_child(false)
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
    QFile file(m_arhiv_files);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString arhiv_files("arhiv_files\n");
            QString arhiv_login("arhiv_login\n");
            QString arhiv_novRacun("arhiv_novRacun\n");
            QString arhiv_produkti("arhiv_produkti\n");
            QString arhiv_stRacuna("arhiv_stRacuna\n");
            QString arhiv_stranke("arhiv_stranke\n");
            QString arhiv_upniki("arhiv_upniki\n");
            QString arhiv_upnikiSeznam("arhiv_upnikiSeznam\n");
            QString company_file("company_file\n");
            QString delavni_proces("delavni_proces\n");
            QString login_file("login_file\n");
            QString material("material\n");
            QString num_company("num_company\n");
            QString settings("settings\n");
            stream << arhiv_files.toUtf8() << arhiv_login.toUtf8() << arhiv_novRacun.toUtf8() << arhiv_produkti.toUtf8() << arhiv_stRacuna.toUtf8() << arhiv_stranke.toUtf8()
                   << arhiv_upniki.toUtf8() << arhiv_upnikiSeznam.toUtf8() << company_file.toUtf8() << delavni_proces.toUtf8() << login_file.toUtf8() << material.toUtf8()
                   << num_company.toUtf8() << settings.toUtf8();
            file.flush();
            file.close();
        }
    }
    file.close();
    file.setFileName(m_arhiv_login);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString prijava("Prijava v sistem: 01.01.2018 - 00:00:00.000 ; ELRA\n");
            stream << prijava.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error arhiv_files";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_arhiv_login.txt")) {
            QFile::remove(m_copy_path + "m_arhiv_login.txt");
        }
        QFile::copy(m_arhiv_login, m_copy_path + "m_arhiv_login.txt");
    }
    file.close();
    file.setFileName(m_arhiv_novRacun);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error arhiv_novRacun";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_arhiv_novRacun.txt")) {
            QFile::remove(m_copy_path + "m_arhiv_novRacun.txt");
        }
        QFile::copy(m_arhiv_novRacun, m_copy_path + "m_arhiv_novRacun.txt");
    }
    file.close();
    file.setFileName(m_arhiv_produkti);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error arhiv_produkti";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_arhiv_produkti.txt")) {
            QFile::remove(m_copy_path + "m_arhiv_produkti.txt");
        }
        QFile::copy(m_arhiv_produkti, m_copy_path + "m_arhiv_produkti.txt");
    }
    file.close();
    file.setFileName(m_arhiv_stRacuna);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString num("0\n");
            stream << num.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error arhiv_stRacuna";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_arhiv_stRacuna.txt")) {
            QFile::remove(m_copy_path + "m_arhiv_stRacuna.txt");
        }
        QFile::copy(m_arhiv_stRacuna, m_copy_path + "m_arhiv_stRacuna.txt");
    }
    file.close();
    file.setFileName(m_arhiv_stranke);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error arhiv_stranke";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_arhiv_stranke.txt")) {
            QFile::remove(m_copy_path + "m_arhiv_stranke.txt");
        }
        QFile::copy(m_arhiv_stranke, m_copy_path + "m_arhiv_stranke.txt");
    }
    file.close();
    file.setFileName(m_arhiv_upniki);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error arhiv_upniki";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_arhiv_upniki.txt")) {
            QFile::remove(m_copy_path + "m_arhiv_upniki.txt");
        }
        QFile::copy(m_arhiv_upniki, m_copy_path + "m_arhiv_upniki.txt");
    }
    file.close();
    file.setFileName(m_arhiv_upnikiSeznam);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error arhiv_upnikiSeznam";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_arhiv_upnikiSeznam.txt")) {
            QFile::remove(m_copy_path + "m_arhiv_upnikiSeznam.txt");
        }
        QFile::copy(m_arhiv_upnikiSeznam, m_copy_path + "m_arhiv_upnikiSeznam.txt");
    }
    file.close();
    file.setFileName(m_company_file);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error company_file";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_company_file.txt")) {
            QFile::remove(m_copy_path + "m_company_file.txt");
        }
        QFile::copy(m_company_file, m_copy_path + "m_company_file.txt");
    }
    file.close();
    file.setFileName(m_delavni_proces);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("Test;0.00; ; ;\n");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error delavni_proces";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_delavni_proces.txt")) {
            QFile::remove(m_copy_path + "m_delavni_proces.txt");
        }
        QFile::copy(m_delavni_proces, m_copy_path + "m_delavni_proces.txt");
    }
    file.close();
    file.setFileName(m_login_file);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString admin("Admin\n");
            stream << admin.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error login_file";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_login_file.txt")) {
            QFile::remove(m_copy_path + "m_login_file.txt");
        }
        QFile::copy(m_login_file, m_copy_path + "m_login_file.txt");
    }
    file.close();
    file.setFileName(m_material);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("Test;0.00;0;kos;\n");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error material";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_material.txt")) {
            QFile::remove(m_copy_path + "m_material.txt");
        }
        QFile::copy(m_material, m_copy_path + "m_material.txt");
    }
    file.close();
    file.setFileName(m_num_company);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString empty("");
            stream << empty.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error num_company";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_num_company.txt")) {
            QFile::remove(m_copy_path + "m_num_company.txt");
        }
        QFile::copy(m_num_company, m_copy_path + "m_num_company.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString lastLine("");
            while(!stream.atEnd()) {
                lastLine = stream.readLine();

            }
            for(int i(1); i < lastLine.toInt(); i++) {
                if (QFile::exists(m_copy_path + QString::number(i) + ".txt")) {
                    QFile::remove(m_copy_path + QString::number(i) + ".txt");
                }
                QFile::copy(m_currentDir + "/" + QString::number(i) + ".txt", m_copy_path + QString::number(i) + ".txt");
            }
            file.close();
        }
    }
    file.close();
    file.setFileName(m_settings);
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString shrani("Shrani v: ; ");
            QString end(" ;");
            stream << shrani.toUtf8() << m_currentDir.toUtf8() << end.toUtf8();
            file.flush();
            file.close();
        } else {
            qDebug() << "Error settings";
        }
    } else {
        if (QFile::exists(m_copy_path + "m_settings.txt")) {
            QFile::remove(m_copy_path + "m_settings.txt");
        }
        QFile::copy(m_settings, m_copy_path + "m_settings.txt");
    }
}
