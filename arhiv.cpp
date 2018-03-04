#include "arhiv.h"
#include "ui_arhiv.h"

Arhiv::Arhiv(QWidget *parent) :
    QDialog(parent), ui(new Ui::Arhiv), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/arhiv_files.txt")
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    AddItemsToCombo();
}

Arhiv::~Arhiv()
{
    delete ui;
}

// if comboBox index change function
void Arhiv::on_comboBox_currentIndexChanged(const QString &item)
{
    ui->listWidget->clear();
    // radioButton enable/desable based on comboBox index
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

// read file to populate comboBox function
void Arhiv::Read()
{
    // open file based on comboBox text
    QString comboIzbira = ui->comboBox->currentText() + ".txt";
    QFile fileName(comboIzbira);
    if(!fileName.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() function in arhiv.cpp";
        return;
    }
    QTextStream in(&fileName);
    QString mText("");
    while(!in.atEnd())
    {
        mText = in.readLine();
        if(mText == "")
        {
            continue;
        }
        else
        {
            ui->listWidget->addItem(mText);
        }
    }
    fileName.close();
}

// add items to combobox function
void Arhiv::AddItemsToCombo()
{
    ui->comboBox->clear();
    QFile mFile(m_fileName);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in AddItemsToCombo() function in arhiv.cpp)";
        return;
    }
    QTextStream in(&mFile);
    QString mText("");
    while(!in.atEnd())
    {
        mText = in.readLine();
        if(mText == "")
        {
            continue;
        }
        else
        {
            ui->comboBox->addItem(mText);
        }
    }
    mFile.close();
}

// search function
void Arhiv::Search(QString searchName)
{
    ui->listWidget->clear();
    // open file based on comboBox current file text
    QString comboIzbira = ui->comboBox->currentText() + ".txt";
    QFile mFile(comboIzbira);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening mFile for reading in Search() function in arhiv.cpp";
        return;
    }
    QTextStream out(&mFile);
    QString line;
    while(!out.atEnd()){
        line = out.readLine();
        if(line.contains(searchName, Qt::CaseInsensitive)) {
            ui->listWidget->addItem(line);
        }
    }
    mFile.close();
}

// radial button editing options
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
