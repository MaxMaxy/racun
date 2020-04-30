#include "arhiv.h"
#include "ui_arhiv.h"

Arhiv::Arhiv(QWidget *parent) :
    QDialog(parent), ui(new Ui::Arhiv), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/arhiv_files.txt"), m_arhivRacun(m_currentDir + "/arhiv_novRacun.txt"),
    m_show_child(false)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Arhiv");
    this->setWindowFlags(Qt::Window);
    this->showMaximized();
    ui->pushButton_isci->setFocus();
    ui->pushButton_isci->setVisible(false);
    AddItemsToCombo();
}

Arhiv::~Arhiv() {
    delete ui;
}

void Arhiv::CloseChild() {
    m_show_child = false;
}

void Arhiv::closeEvent(QCloseEvent *) {
    emit close_me();
}

void Arhiv::on_comboBox_currentIndexChanged(const QString &item) {
    ui->listWidget->clear();
    QButtonGroup *group = new QButtonGroup();
    group->addButton(ui->radioButton_dodaneStranke);
    group->addButton(ui->radioButton_dodaniProdukti);
    group->addButton(ui->radioButton_neuspesnaPrijava);
    group->addButton(ui->radioButton_popravljeneStranke);
    group->addButton(ui->radioButton_popravljeniProdukti);
    group->addButton(ui->radioButton_uspesnaPrijava);
    group->addButton(ui->radioButton_noviRacuni);
    group->addButton(ui->radioButton_spremenjeniRacuni);
    group->addButton(ui->radioButton_vse);
    QAbstractButton * checked = group->checkedButton();
    if(checked) {
        group->setExclusive(false);
        checked->setChecked(false);
        group->setExclusive(true);
    }
    ui->radioButton_vse->setChecked(true);
    if(item == "arhiv_login") {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(true);
        ui->radioButton_neuspesnaPrijava->setEnabled(true);
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
    }
    else if(item == "arhiv_novRacun") {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
        ui->radioButton_noviRacuni->setEnabled(true);
        ui->radioButton_spremenjeniRacuni->setEnabled(true);
    }
    else if(item == "arhiv_produkti") {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(true);
        ui->radioButton_popravljeniProdukti->setEnabled(true);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
    }
    else if(item == "arhiv_stranke") {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(true);
        ui->radioButton_popravljeneStranke->setEnabled(true);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
    }
    else if(item == "arhiv_files") {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
    }
    else {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
    }
}

void Arhiv::Read() {
    QString comboIzbira = ui->comboBox->currentText().toUtf8() + ".txt";
    QFile fileName(comboIzbira);
    if(!fileName.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in Read() function in arhiv.cpp";
        return;
    }
    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    QString mText("");
    while(!in.atEnd()) {
        mText = in.readLine();
        if(mText == "") {
            continue;
        } else {
            ui->listWidget->addItem(mText.toUtf8());
        }
    }
    fileName.close();
}

void Arhiv::AddItemsToCombo() {
    ui->comboBox->clear();
    QFile mFile(m_fileName);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in AddItemsToCombo() function in arhiv.cpp)";
        return;
    }
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QString mText("");
    while(!in.atEnd()) {
        mText = in.readLine();
        if(mText == "") {
            continue;
        } else {
            ui->comboBox->addItem(mText.toUtf8());
        }
    }
    mFile.close();
}

void Arhiv::Search(QString searchName) {
    ui->listWidget->clear();
    QString comboIzbira = ui->comboBox->currentText().toUtf8() + ".txt";
    QFile mFile(comboIzbira);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening mFile for reading in Search() function in arhiv.cpp";
        return;
    }
    QTextStream out(&mFile);
    out.setCodec("UTF-8");
    QString line;
    while(!out.atEnd()) {
        line = out.readLine();
        if(line.contains(searchName, Qt::CaseInsensitive)) {
            ui->listWidget->addItem(line.toUtf8());
        }
    }
    mFile.close();
}

void Arhiv::on_radioButton_uspesnaPrijava_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Search("Prijava v sistem");
}

void Arhiv::on_radioButton_vse_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Read();
}

void Arhiv::on_radioButton_neuspesnaPrijava_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Search("Nepravilna prijava");
}

void Arhiv::on_radioButton_dodaniProdukti_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Search("Dodano");
}

void Arhiv::on_radioButton_popravljeniProdukti_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Search("Popravljeno");
}

void Arhiv::on_radioButton_dodaneStranke_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Search("Dodano");
}

void Arhiv::on_radioButton_popravljeneStranke_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Search("Popravljeno");
}

void Arhiv::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    if(ui->comboBox->currentText() == "arhiv_novRacun") {
        QString tmp("");
        QStringList list = item->text().split(" ; ", QString::SkipEmptyParts);
        QStringList tmp_list;
        for(int i(0); i < list.length(); i++) {
            tmp = list.at(i);
            if(list.at(i) == "")
                tmp = "/";
            tmp_list.append(tmp.split(": ", QString::SkipEmptyParts));
        }
        NovRacun racun;
        racun.setModal(true);
        QFile mFile(m_arhivRacun);

        if(!mFile.open(QFile::Text | QFile::ReadOnly)) {
            qDebug() << "Error opening mFile for reading in popravi button";
            return;
        }
        QTextStream in(&mFile);
        in.setCodec("UTF-8");
        QString allText = in.readAll();
        mFile.close();

        QString produkt_star = item->text().toUtf8();
        QString opomba = list.at(12).toUtf8();
        QString produkt_nov = item->text().replace(opomba, "Opomba: racun_je_bil_spremenjen!!!????").toUtf8();
        allText.replace(produkt_star, produkt_nov);

        if(!mFile.open(QFile::WriteOnly | QFile::Truncate)) {
            qDebug() << "Error opening mFile for truncate in popravi button";
            return;
        }
        mFile.flush();
        mFile.close();

        if(!mFile.open(QFile::WriteOnly | QFile::Text)) {
            qDebug() << "Error opening mFile for writing in popravi button";
            return;
        }
        in << allText.toUtf8();
        mFile.flush();
        mFile.close();
        racun.PopraviRacun(tmp_list.at(12).toUtf8(), tmp_list.at(4).toUtf8(), tmp_list.at(6).toUtf8(), tmp_list.at(8).toUtf8(), tmp_list.at(22).toUtf8(), tmp_list.at(14).toUtf8(), tmp_list.at(32).toUtf8());
        this->hide();
        QObject::connect(&racun,SIGNAL(close_me()),this,SLOT(CloseChild()));
        m_show_child = true;
        while (m_show_child) {
            racun.exec();
        }
        this->show();
        Arhiv::show();
        ui->listWidget->clear();
        Read();
    }
}

void Arhiv::on_radioButton_noviRacuni_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Search("Nov racun");
}

void Arhiv::on_radioButton_spremenjeniRacuni_toggled(bool checked) {
    ui->listWidget->clear();
    if(checked)
        Search("Sprememba racuna");
}

void Arhiv::on_lineEdit_isci_textChanged() {
    QString search = ui->lineEdit_isci->text().toUtf8();
    if(search == "")
        Read();
    else {
        ui->listWidget->clear();
        Search(search);
    }
}

void Arhiv::on_pushButton_clicked() {
    close();
}
