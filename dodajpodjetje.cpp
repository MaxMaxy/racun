#include "dodajpodjetje.h"
#include "ui_dodajpodjetje.h"

// constructor
DodajPodjetje::DodajPodjetje(QWidget *parent) :
    QDialog(parent), ui(new Ui::DodajPodjetje), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/test.txt"), m_numOfCompany(m_currentDir + "/num.txt"), m_arhivStranke(m_currentDir + "/arhiv_stranke.txt"), m_zapSt("") ,m_cNaziv(""), m_kNaziv(""), m_naslov(""),m_posta(""), m_ddv(""), m_email(""), m_stranka(""), m_itr(0), m_count(true)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setColumnWidth(0,40);
    Read();
    TotalCompany();
}

// destruktor
DodajPodjetje::~DodajPodjetje()
{
    delete ui;
}

void DodajPodjetje::TotalCompany()
{
    // odpri num file
    QFile mNum(m_numOfCompany);
    // test ce je file odprt
    if(!mNum.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening mNum for reading in Write()";
        return;
    }
    // stream za num file
    QTextStream in(&mNum);
    // string za zadnjo stevilko v num file
    QString number;
    // shrani zadnjo stevilko
    while(!in.atEnd())
    {
        number = in.readLine();
    }
    m_zapSt = number;
    // zapre mNum file
    mNum.close();
}

// dodat root v treeWidget
void DodajPodjetje::AddRoot(QString name, QString num, QString cNaziv, QString naslov,QString posta, QString ddv, QString email)
{
    // pointer na treeWidget
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    // nastavimo stevilko podjetja in ime podjetja v colom 0 in 1
    itm->setText(0, num);
    itm->setText(1, name);
    // dodamo podjetje v treeWidget
    ui->treeWidget->addTopLevelItem(itm);
    // nastavimo barvo za vsako drugo podjetje
    QColor color(210,210,210);
    QColor wcolor(250,250,250);
    // m_count je member bool ce je tru je barva siva drugace bela
    if(m_count)
    {
        itm->setBackgroundColor(0,color);
        itm->setBackgroundColor(1,color);
        m_count = false;
    } else {
        itm->setBackgroundColor(0,wcolor);
        itm->setBackgroundColor(1,wcolor);
        m_count = true;
    }
    // dodamo podkategorije za vsako podjetje
    AddChild(itm, cNaziv);
    AddChild(itm, naslov);
    AddChild(itm, posta);
    AddChild(itm, ddv);
    AddChild(itm, email);
}

// doda child v treeWidget
void DodajPodjetje::AddChild(QTreeWidgetItem* parent, QString name)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(1, name);
    QFont font;
    font.setBold(true);
    font.setItalic(true);
    itm->setFont(1,font);
    parent->addChild(itm);
}

void DodajPodjetje::Arhiv(QString arhiv_file, QString stream)
{
    // odpri file
    QFile mFile(arhiv_file);
    // test ce je file odprt
    if(!mFile.open(QFile::WriteOnly | QFile::Append))
    {
        qDebug() << "Error opening mFile for writing in Arhiv()";
        return;
    }
    // stream za num file
    QTextStream out(&mFile);
    out << stream << "\n";
    mFile.flush();
    mFile.close();
}

// Zapisi podjetje v file
void DodajPodjetje::Write(QString file_podjetje, QString file_num)
{
    // odpri file
    QFile mFile(file_podjetje);
    // test ce je file odprt
    if(!mFile.open(QFile::WriteOnly | QFile::Append))
    {
        qDebug() << "Error opening mFile for writing in Write()";
        return;
    }
    // odpri num file
    QFile mNum(file_num);
    // test ce je file odprt
    if(!mNum.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening mNum for reading in Write()";
        return;
    }
    // stream za num file
    QTextStream in(&mNum);
    // string za zadnjo stevilko v num file
    QString number;
    // shrani zadnjo stevilko
    while(!in.atEnd())
    {
        number = in.readLine();
    }
    m_zapSt = number;
    // zapre mNum file
    mNum.close();
    // stream za pisanje v mFile
    QTextStream out(&mFile);
    // vars za out file
    m_kNaziv = ui->lineEdit_kNaziv->text();
    if(m_kNaziv == "")
        m_kNaziv = "ni podatka";
    m_cNaziv = ui->lineEdit_cNaziv->text();
    if(m_cNaziv == "")
        m_cNaziv = "ni podatka";
    m_naslov = ui->lineEdit_naslov->text();
    if(m_naslov == "")
        m_naslov = "ni podatka";
    m_posta = ui->lineEdit_posta->text();
    if(m_posta == "")
        m_posta = "ni podatka";
    m_ddv = ui->lineEdit_ddv->text();
    if(m_ddv == "")
        m_ddv = "ni podatka";
    m_email = ui->lineEdit_email->text();
    if(m_email == "")
        m_email = "ni podatka";
    // doda podjetje v naslednjo vrstico
    out << number << ";" << m_kNaziv << ";" << m_cNaziv << ";" << m_naslov << ";" << m_posta << ";" << m_ddv << ";" << m_email << ";" << "\n";
    // flusha in zapre file
    mFile.flush();
    mFile.close();
    // test ce je file odprt (za pisanje v num file)
    if(!mNum.open(QFile::WriteOnly | QFile::Append))
    {
        qDebug() << "Error opening mNum for writing in Write()";
        return;
    }
    // spremeni QString number v int number
    int numInt = number.toInt();
    // povisa stevilo podjetij za ena
    numInt++;
    number = QString::number(numInt);
    // doda number v mNum file
    in << "\n" << number;
    // flasha in zapre mNum file
    mNum.flush();
    mNum.close();
}

// Preberi podjetje iz file-a
void DodajPodjetje::Read()
{
    // zbrise vse iz treeWidgeta
    ui->treeWidget->clear();
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
    // nastavimo m_itr da prestejemo vsa podjetja z namenom da pozneje zapisemo stevilke v num.txt
    m_itr = 1;
    // preberi vsako vrstico ce je prazna izpusti drugace dodaj vrstico v listWidget
    QString mText("");
    // locilo med podatki podjetja v filu ime_podjetja;naslov;ddv;email itd itd
    QRegExp rx("[;]");
    // naredimo listo da posamezne podatke locimo in shranimo kot posamezno kategorijo
    QStringList list;
    // preberemo celoten dokument in shranimo posamezne podatke v list
    while(!in.atEnd())
    {
        // prebere podjetje
        mText = in.readLine();
        // v listo shranimo posamezne podatke podjetja iz fila
        list = mText.split(rx, QString::SkipEmptyParts);
        if(mText == "")
        {
            continue;
        }
        else
        {
            // v treeWidget vnesemo vsa podjetja (posamezni podatki iz liste so list.at(?))
            AddRoot(list.at(1), list.at(0), list.at(2), list.at(3), list.at(4), list.at(5), list.at(6));
            // povecamo m_itr za vsako podjetje
            m_itr++;
        }
    }
    // zapremo file
    mFile.close();
    // odpremo num.txt da shranimo stevilo podjetij
    QFile mNum(m_numOfCompany);
    // izbrisemo stare podatke iz num.txt
    if(!mNum.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << "Error opening file for writing in Read()";
        return;
    }
    // flushamo in zapremo file
    mNum.flush();
    mNum.close();
    // odpremo file ponovno da shranimo
    if(!mNum.open(QFile::WriteOnly | QFile::Append))
    {
        qDebug() << "Error opening file for writing in Read() #2";
        return;
    }
    // stream za vnos v file
    QTextStream out(&mNum);
    // zapisemo v file stevilo podjetij (for loop da je v vsaki vrstici nova cifra)
    for(int i(1); i <= m_itr; i++)
    {
        out << i << "\n";
    }
    // flashamo in zapremo file
    mNum.flush();
    mNum.close();
}

// dodaj podjetje gumb
void DodajPodjetje::on_pushButton_dodajPodjetje_clicked()
{
    Write(m_fileName, m_numOfCompany);
    QFile file;
    file.setFileName(m_zapSt + ".txt");
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "File not opened!!";
        return;
    }
    QDateTime date = QDateTime::currentDateTime();
    QTextStream stream(&file);
    stream << "Dodano: ;" << m_kNaziv << " - " << date.toString("dd.MM.yyyy ; hh:mm:ss.zzz ;") << "\n";
    file.flush();
    file.close();
    Read();
    m_zapSt = QString::number(m_zapSt.toInt()+1);
    QString dodano = "Dodano";
    QString arhiv = dodano + " ; " + date.toString("dd.MM.yyyy ; hh:mm:ss.zzz") + " ; " + m_kNaziv + " ; " + m_cNaziv + " ; " + m_naslov + " ; " + m_posta + " ; " + m_ddv + " ; " + m_email + " ; " + "Rajh" + " !?!";
    Arhiv(m_arhivStranke, arhiv);
}

// popravi podatke gumb
void DodajPodjetje::on_pushButton_popraviPodatke_clicked()
{
    if(ui->treeWidget->currentItem() == 0)
    {
        return;
    }
    // odpre file za branje
    QFile mFile(m_fileName);
    // test ce je file odprt
    if(!mFile.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug() << "Error opening mFile for reading in popravi button";
        return;
    }
    // stream za shranit text fila
    QTextStream out(&mFile);
    // prebere celoten dokument in shrani v var
    QString allText = out.readAll();
    // zapre file
    mFile.close();

    // zamenja spremenjene dele besedila
    QRegularExpression stranka(m_stranka);
    // zamenja spremenjene dele besedila
    QString rep_stranka(m_zapSt + ";" + ui->lineEdit_kNaziv->text() + ";" + ui->lineEdit_cNaziv->text() + ";" + ui->lineEdit_naslov->text() + ";" + ui->lineEdit_posta->text() + ";" + ui->lineEdit_ddv->text() + ";" + ui->lineEdit_email->text() + ";");
    allText.replace(stranka, rep_stranka);
    // odpre file za pisanje (z namenom da zbrise vse stare podatke) in test
    if(!mFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << "Error opening mFile for truncate in popravi button";
        return;
    }
    // zbrise celoten file in ga zapre
    mFile.flush();
    mFile.close();
    // odpre file za pisanje (z namenom da zapise nove podatke) in test
    if(!mFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error opening mFile for writing in popravi button";
        return;
    }
    // zapise nove podatke, flush in zapre
    out << allText;
    mFile.flush();
    mFile.close();
    // ponovno branje, da se osvezi treeWidget
    Read();
    QDateTime date = QDateTime::currentDateTime();
    QString popravljeno = "Popravljeno";
    QString arhiv = popravljeno + " ; " + date.toString("dd.MM.yyyy ; hh:mm:ss.zzz") + " ; " + m_kNaziv + " ; " + m_cNaziv + " ; " + m_naslov + " ; " + m_posta + " ; " + m_ddv + " ; " + m_email + " ; " + "Rajh" + " !?!";
    Arhiv(m_arhivStranke, arhiv);
}

// ce dvakrat kliknes na treeWidget
void DodajPodjetje::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    ui->treeWidget->collapseAll();
    if(index.parent().row() == -1)
    {
        QFile mFile(m_fileName);
        // test ce je file odprt
        if(!mFile.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "Error opening mFile for reading in list clicked";
            return;
        }
        // stream za file
        QTextStream in(&mFile);
        // itr nastavimo na -1 da prebere 0-to vrstico v filu
        m_itr = -1;
        // var za text
        QString text("");
        // while loop da pride do vrstice ki smo jo kliknali in shrani to vrstico v text var
        while(!(m_itr == index.row()))
        {
            text = in.readLine();
            m_itr++;
            // safety da ne pride v endless loop
            if(m_itr > 100) break;
        }
        m_stranka = text;
        // rx var kjer je locilo med podatki o podjetju
        QRegExp rx("[;]");
        // v listo shranimo posamezne podatke podjetja iz fila
        QStringList list = text.split(rx, QString::SkipEmptyParts);
        // iz liste vzame posamezne podatke in jih vstavi v lineEdite po kategorijah
        ui->lineEdit_kNaziv->setText(list.at(1));
        ui->lineEdit_cNaziv->setText(list.at(2));
        ui->lineEdit_naslov->setText(list.at(3));
        ui->lineEdit_posta->setText(list.at(4));
        ui->lineEdit_ddv->setText(list.at(5));
        ui->lineEdit_email->setText(list.at(6));
        // nastavi spremenljivke za podjetje ki smo ga oznacili
        m_zapSt = list.at(0);
        m_cNaziv = ui->lineEdit_cNaziv->text();
        m_kNaziv = ui->lineEdit_kNaziv->text();
        m_naslov = ui->lineEdit_naslov->text();
        m_posta = ui->lineEdit_posta->text();
        m_ddv = ui->lineEdit_ddv->text();
        m_email = ui->lineEdit_email->text();
    }
}
