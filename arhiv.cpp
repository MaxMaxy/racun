#include "arhiv.h"
#include "ui_arhiv.h"

Arhiv::Arhiv(QWidget *parent) :
    QDialog(parent), ui(new Ui::Arhiv), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/arhiv_files.txt")
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Arhiv");
    this->setWindowFlags(Qt::Window);
    ui->pushButton_isci->setFocus();
    ui->pushButton_isci->setVisible(false);
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
    group->addButton(ui->radioButton_noviRacuni);
    group->addButton(ui->radioButton_spremenjeniRacuni);
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
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
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
        ui->radioButton_noviRacuni->setEnabled(true);
        ui->radioButton_spremenjeniRacuni->setEnabled(true);
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
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
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
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
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
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
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
        ui->radioButton_noviRacuni->setEnabled(false);
        ui->radioButton_spremenjeniRacuni->setEnabled(false);
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
    in.setCodec("UTF-8");
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
    in.setCodec("UTF-8");
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
    out.setCodec("UTF-8");
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

void Arhiv::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(ui->comboBox->currentText() == "arhiv_novRacun")
    {
        QString tmp("");
        QStringList list = item->text().split(" ; ", QString::SkipEmptyParts);
        QStringList tmp_list;
        for(int i(0); i < list.length(); i++)
        {
            tmp = list.at(i);
            tmp_list.append(tmp.split(": ", QString::SkipEmptyParts));
        }
        NovRacun racun;
        racun.setModal(true);
        racun.PopraviRacun(tmp_list.at(12), tmp_list.at(4), tmp_list.at(6), tmp_list.at(8), tmp_list.at(22), tmp_list.at(14), tmp_list.at(16), tmp_list.at(18), tmp_list.at(20), tmp_list.at(32));
        racun.exec();
        ui->listWidget->clear();
        Read();
    }
}

void Arhiv::on_radioButton_noviRacuni_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Search("Nov racun");
}

void Arhiv::on_radioButton_spremenjeniRacuni_toggled(bool checked)
{
    ui->listWidget->clear();
    if(checked)
        Search("Sprememba racuna");
}

void Arhiv::on_lineEdit_isci_textChanged()
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
