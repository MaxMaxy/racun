#include "kalkulacija.h"
#include "ui_kalkulacija.h"

Kalkulacija::Kalkulacija(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kalkulacija), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/company_file.txt"), m_cNaziv("")
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    AddItemsToCombo();
}

Kalkulacija::~Kalkulacija()
{
    delete ui;
}

// vstavi podjetja v combo box
void Kalkulacija::AddItemsToCombo()
{
    // zbrise vse iz combo boxa
    ui->comboBox_stranka->clear();
    // naredi QFile var za fileName
    QFile mFile(m_fileName);
    // test ce je odprt za branje
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error pri vstavljanju podjetji v combobox v kalkulaciji cpp";
        return;
    }
    // stream za branje fila
    QTextStream in(&mFile);
    // preberi vsako vrstico ce je  izpusti drugace dodaj vrstico v combo box
    QString mText("");
    // locilo med podatki podjetja v filu ime_podjetja;naslov;ddv;email itd itd
    QRegExp rx("[;]");
    // naredimo listo da posamezne podatke locimo in shranimo kot posamezno kategorijo
    QStringList list;
    bool first(true);
    // preberemo celoten dokument in shranimo posamezne podatke v list
    while(!in.atEnd())
    {
        // prebere podjetje
        mText = in.readLine();
        // v listo shranimo posamezne podatke podjetja iz fila
        list = mText.split(rx, QString::SkipEmptyParts);
        if(first)
        {
            m_cNaziv = list.at(1);
            first = false;
        }
        if(mText == "")
        {
            continue;
        }
        else
        {
            // doda podjetje v combo box
            ui->comboBox_stranka->addItem(list.at(1));
        }
    }
    // zapremo file
    mFile.close();
}
