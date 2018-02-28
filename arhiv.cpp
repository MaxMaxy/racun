#include "arhiv.h"
#include "ui_arhiv.h"

Arhiv::Arhiv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Arhiv), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/arhiv_files.txt")
{
    ui->setupUi(this);
    AddItemsToCombo();
}

Arhiv::~Arhiv()
{
    delete ui;
}

// doda text iz arhivskih filov ob spremembi indexa v comboboxu
void Arhiv::on_comboBox_currentIndexChanged(const QString &item)
{
    ui->listWidget->clear();
    QButtonGroup *group = new QButtonGroup();
    group->addButton(ui->radioButton_dodaneStranke);
    group->addButton(ui->radioButton_dodaniProdukti);
    group->addButton(ui->radioButton_neuspesnaPrijava);
    group->addButton(ui->radioButton_popravljeneStranke);
    group->addButton(ui->radioButton_popravljeniProdukti);
    group->addButton(ui->radioButton_uspesnaPrijava);
    group->addButton(ui->radioButton_vse);
    QAbstractButton * checked = group->checkedButton();
    if(checked)
    {
        group->setExclusive(false);
        checked->setChecked(false);
        group->setExclusive(true);
    }
    ui->radioButton_vse->setChecked(true);
    if(item == "arhiv_login")
    {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(true);
        ui->radioButton_neuspesnaPrijava->setEnabled(true);
    }
    else if(item == "arhiv_novRacun")
    {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
    }
    else if(item == "arhiv_produkti")
    {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(true);
        ui->radioButton_popravljeniProdukti->setEnabled(true);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
    }
    else if(item == "arhiv_stranke")
    {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(true);
        ui->radioButton_popravljeneStranke->setEnabled(true);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
    }
    else if(item == "arhiv_files")
    {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
    }
    else
    {
        ui->radioButton_vse->setEnabled(true);
        ui->radioButton_dodaneStranke->setEnabled(false);
        ui->radioButton_popravljeneStranke->setEnabled(false);
        ui->radioButton_dodaniProdukti->setEnabled(false);
        ui->radioButton_popravljeniProdukti->setEnabled(false);
        ui->radioButton_uspesnaPrijava->setEnabled(false);
        ui->radioButton_neuspesnaPrijava->setEnabled(false);
    }
}

// Preberi produkt iz file-a
void Arhiv::Read()
{
    QString comboIzbira = ui->comboBox->currentText() + ".txt";
    QFile fileName(comboIzbira);
    // test ce je odprt za branje
    if(!fileName.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read()";
        return;
    }
    // stream za branje fila
    QTextStream in(&fileName);
    // preberi vsako vrstico ce je prazna izpusti drugace dodaj vrstico v listWidget
    QString mText("");
    // preberemo celoten dokument in shranimo posamezne podatke v list
    while(!in.atEnd())
    {
        // prebere podjetje
        mText = in.readLine();
        if(mText == "")
        {
            continue;
        }
        else
        {
            // v treeWidget vnesemo vsa podjetja (posamezni podatki iz liste so list.at(?))
            ui->listWidget->addItem(mText);
        }
    }
    // zapremo file
    fileName.close();
}

// vstavi podjetja v combo box
void Arhiv::AddItemsToCombo()
{
    // zbrise vse iz combo boxa
    ui->comboBox->clear();
    // naredi QFile var za fileName
    QFile mFile(m_fileName);
    // test ce je odprt za branje
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read()";
        return;
    }
    // stream za branje fila
    QTextStream in(&mFile);
    QString mText("");
    while(!in.atEnd())
    {
        // prebere podjetje
        mText = in.readLine();
        if(mText == "")
        {
            continue;
        }
        else
        {
            // doda podjetje v combo box
            ui->comboBox->addItem(mText);
        }
    }
    // zapremo file
    mFile.close();
}

// iskalnik
void Arhiv::Search(QString searchName)
{
    ui->listWidget->clear();
    QString comboIzbira = ui->comboBox->currentText() + ".txt";
    // odpre file za branje
    QFile mFile(comboIzbira);
    // test ce je file odprt
    if(!mFile.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug() << "Error opening mFile for reading in Search() Arhiv";
        return;
    }
    // stream za shranit text fila
    QTextStream out(&mFile);
    // prebere celoten dokument in shrani v var
    QString line;
    while(!out.atEnd()){
        line = out.readLine();
        if(line.contains(searchName, Qt::CaseInsensitive)) {
            ui->listWidget->addItem(line);
        }
    }
    // zapre file
    mFile.close();
}

// radial gumbi opcije
void Arhiv::on_lineEdit_isci_editingFinished()
{
    QString search = ui->lineEdit_isci->text();
    if(search == "")
        Read();
    else
    {
        ui->listWidget->clear();
        Search(search);
    }
}

void Arhiv::on_radioButton_uspesnaPrijava_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Search("Prijava v sistem");
}

void Arhiv::on_radioButton_vse_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Read();
}

void Arhiv::on_radioButton_neuspesnaPrijava_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Search("Nepravilna prijava");
}

void Arhiv::on_radioButton_dodaniProdukti_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Search("Dodano");
}

void Arhiv::on_radioButton_popravljeniProdukti_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Search("Popravljeno");
}

void Arhiv::on_radioButton_dodaneStranke_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Search("Dodano");
}

void Arhiv::on_radioButton_popravljeneStranke_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Search("Popravljeno");
}
