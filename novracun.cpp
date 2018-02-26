#include "novracun.h"
#include "ui_novracun.h"

NovRacun::NovRacun(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NovRacun), m_fileName("/usr/home/cloudjunkie/test.txt"), m_cNaziv(""), m_naslov(""), m_posta(""), m_ddv(""), m_email(""), m_numItems("1"), m_count(true), m_total(0), m_itemsAdded(0), m_max_produktov(19), m_vrstic(0)
{
    ui->setupUi(this);
    ui->treeWidget_seznam->setColumnCount(3);
    ui->treeWidget_seznam->setColumnWidth(0,80);
    ui->treeWidget_seznam->setColumnWidth(1,300);
    ui->treeWidget_dodani->setColumnCount(5);
    ui->treeWidget_dodani->setColumnWidth(0,80);
    ui->treeWidget_dodani->setColumnWidth(1,250);
    ui->treeWidget_dodani->setColumnWidth(2,50);
    ui->lineEdit_popust->setText("0");
    ui->label_skupaj->setText("0€");
    ui->label_ddv->setText("0€");
    ui->label_osnova->setText("0€");
    ui->dateEdit->setMinimumDate(QDate(2016,1,1));
    QFile mFile(m_arhivStRacuna);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() constructor";
        return;
    }
    QTextStream in(&mFile);
    QString mText("");
    while(!in.atEnd())
    {
        mText = in.readLine();
    }
    mFile.close();
    int stRacuna = mText.toInt() + 1;
    mText = QString::number(stRacuna);
    ui->dateEdit->setDisplayFormat("yyyy");
    ui->lineEdit_stRacuna->setText(mText + "-" + ui->dateEdit->text());
    ui->dateEdit->setDisplayFormat("d. M. yyyy");
    ui->dateEdit->setDate(QDate::currentDate());
    AddItemsToCombo();
    Read();
}

NovRacun::~NovRacun()
{
    delete ui;
}

void NovRacun::Arhiv(QString arhiv_file, QString stream)
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

// Preberi produkt iz file-a
void NovRacun::Read()
{
    // zbrise vse iz treeWidgeta
    ui->treeWidget_seznam->clear();
    // direktorij je trenutno nastavljen na moj komp
    QDir::setCurrent("/usr/home/cloudjunkie/");
    int izbPod = ui->comboBox_narocnik->currentIndex() + 1;
    QString podjetje = QString::number(izbPod) + ".txt";

    QFile fileName(podjetje);
    // test ce je odprt za branje
    if(!fileName.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() READ";
        return;
    }
    // stream za branje fila
    QTextStream in(&fileName);
    // preberi vsako vrstico ce je  izpusti drugace dodaj vrstico v listWidget
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
        else if(list.at(0) == "Dodano: ")
        {
            continue;
        }
        else
        {
            // v treeWidget vnesemo vsa podjetja (posamezni podatki iz liste so list.at(?))
            AddRoot(list.at(0), list.at(1), list.at(2));
        }
    }
    // zapremo file
    fileName.close();
}

// vstavi podjetja v combo box
void NovRacun::AddItemsToCombo()
{
    // zbrise vse iz combo boxa
    ui->comboBox_narocnik->clear();
    // naredi QFile var za fileName
    QFile mFile(m_fileName);
    // test ce je odprt za branje
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() addItemsToCombo";
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
            m_cNaziv = list.at(2);
            m_naslov = list.at(3);
            m_posta = list.at(4);
            m_ddv = list.at(5);
            m_email = list.at(6);
            first = false;
        }
        if(mText == "")
        {
            continue;
        }
        else
        {
            // doda podjetje v combo box
            ui->comboBox_narocnik->addItem(list.at(1));
        }
    }
    // zapremo file
    mFile.close();
    Read();
}

// dodat root v treeWidget
void NovRacun::AddRoot(QString id, QString naziv, QString cena)
{
    // pointer na treeWidget
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_seznam);
    // nastavimo stevilko podjetja in ime podjetja v colom 0 in 1
    itm->setText(0, id);
    itm->setText(1, naziv);
    itm->setText(2, cena + "€");
    // dodamo podjetje v treeWidget
    ui->treeWidget_seznam->addTopLevelItem(itm);
    // nastavimo barvo za vsako drugo podjetje
    QColor color(210,210,210);
    QColor wcolor(250,250,250);
    // m_count je member bool ce je tru je barva siva drugace bela
    if(m_count)
    {
        itm->setBackgroundColor(0,color);
        itm->setBackgroundColor(1,color);
        itm->setBackgroundColor(2,color);
        m_count = false;
    } else {
        itm->setBackgroundColor(0,wcolor);
        itm->setBackgroundColor(1,wcolor);
        itm->setBackgroundColor(2,wcolor);
        m_count = true;
    }
}

void NovRacun::on_comboBox_narocnik_currentIndexChanged()
{
    m_cNaziv = "";
    m_naslov = "";
    m_posta = "";
    m_ddv = "";
    m_email = "";
    QFile mFile(m_fileName);
    // test ce je odprt za branje
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() addItemsToCombo";
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
        else if(list.at(1) == ui->comboBox_narocnik->currentText())
        {
            m_cNaziv = list.at(2);
            m_naslov = list.at(3);
            m_posta = list.at(4);
            m_ddv = list.at(5);
            m_email = list.at(6);
            break;
        }
    }
    // zapremo file
    mFile.close();
    Read();
    ui->treeWidget_dodani->clear();
    ui->label_skupaj->setText("0€");
    ui->label_ddv->setText("0€");
    ui->label_osnova->setText("0€");
    ui->lineEdit_popust->setText("0");
    m_total = 0;
    m_itemsAdded = 0;
}

QString NovRacun::CenaDDV(QString price, QString items)
{
    QRegExp eur("[€]");
    QRegExp dot("[,]");
    QStringList price_list;
    price_list = price.split(eur, QString::SkipEmptyParts);
    price = price_list.at(0);
    price_list = price.split(dot, QString::SkipEmptyParts);
    if(price_list.size() == 2)
    {
        QRegularExpression place_dot(price);
        QString rep_produkt(price_list.at(0) + "." + price_list.at(1));
        price.replace(place_dot, rep_produkt);
    }
    float fPrice = price.toFloat();
    int intItems = items.toInt();
    float sumPrice = fPrice * intItems;
    return QString::number(sumPrice);
}

void NovRacun::on_treeWidget_seznam_doubleClicked()
{
    int vrstic(0);
    if(ui->treeWidget_seznam->currentItem()->text(1).length() < 43 && (m_vrstic + 1) <= m_max_produktov)
        vrstic++;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 57 && (m_vrstic + 3) <= m_max_produktov)
        vrstic += 3;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 77 && (m_vrstic + 4) <= m_max_produktov)
        vrstic += 4;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 90 && (m_vrstic + 5) <= m_max_produktov)
        vrstic += 5;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 108 && (m_vrstic + 6) <= m_max_produktov)
        vrstic += 6;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 126 && (m_vrstic + 7) <= m_max_produktov)
        vrstic += 7;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 144 && (m_vrstic + 8) <= m_max_produktov)
        vrstic += 8;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 162 && (m_vrstic + 9) <= m_max_produktov)
        vrstic += 9;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 180 && (m_vrstic + 10) <= m_max_produktov)
        vrstic += 10;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 198 && (m_vrstic + 11) <= m_max_produktov)
        vrstic += 11;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 216 && (m_vrstic + 12) <= m_max_produktov)
        vrstic += 12;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 234 && (m_vrstic + 13) <= m_max_produktov)
        vrstic += 13;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 252 && (m_vrstic + 14) <= m_max_produktov)
        vrstic += 14;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 270 && (m_vrstic + 15) <= m_max_produktov)
        vrstic += 15;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 288 && (m_vrstic + 16) <= m_max_produktov)
        vrstic += 16;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 306 && (m_vrstic + 17) <= m_max_produktov)
        vrstic += 17;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 324 && (m_vrstic + 18) <= m_max_produktov)
        vrstic += 18;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 342 && (m_vrstic + 19) <= m_max_produktov)
        vrstic += 19;
    else if(ui->treeWidget_seznam->currentItem()->text(1).length() < 360 && (m_vrstic + 20) <= m_max_produktov)
        vrstic += 20;
    else
    {
        QMessageBox error_stringToLong;
        error_stringToLong.critical(this, "Napaka", "Racun vsebuje prevec produktov ali pa je ime produkta predolgo!");
        error_stringToLong.show();
        return;
    }

    if(m_vrstic >= m_max_produktov)
        return;
    else
        m_vrstic += vrstic;

    NumOfItems numItems;
    numItems.setModal(true);
    numItems.exec();
    m_numItems = numItems.m_numKosov;
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_dodani);
    QString cenaDDV = CenaDDV(ui->treeWidget_seznam->currentItem()->text(2), m_numItems);
    itm->setText(0, ui->treeWidget_seznam->currentItem()->text(0));
    itm->setText(1, ui->treeWidget_seznam->currentItem()->text(1));
    itm->setText(2, ui->treeWidget_seznam->currentItem()->text(2));
    itm->setText(3, m_numItems);
    itm->setText(4, cenaDDV);
    ui->treeWidget_dodani->addTopLevelItem(itm);
    QColor color(210,210,210);
    QColor wcolor(250,250,250);
    // m_count je member bool ce je tru je barva siva drugace bela
    if(m_count)
    {
        itm->setBackgroundColor(0,color);
        itm->setBackgroundColor(1,color);
        itm->setBackgroundColor(2,color);
        itm->setBackgroundColor(3,color);
        m_count = false;
    } else {
        itm->setBackgroundColor(0,wcolor);
        itm->setBackgroundColor(1,wcolor);
        itm->setBackgroundColor(2,wcolor);
        itm->setBackgroundColor(3,wcolor);
        m_count = true;
    }
    QString price = ui->treeWidget_seznam->currentItem()->text(2);
    CalcSkupaj(price, m_numItems, true);
    ui->treeWidget_dodani->setUniformRowHeights(true);
    m_itemsAdded++;
}

// sesteva v label skupaj
void NovRacun::CalcSkupaj(QString &price, QString &numOfItems, bool plus)
{
    float popust(0);
    float skupaj(0);
    float ddv(0);
    QRegExp eur("[€]");
    QRegExp dot("[,]");
    QStringList price_list;
    price_list = price.split(eur, QString::SkipEmptyParts);
    price = price_list.at(0);
    price_list = price.split(dot, QString::SkipEmptyParts);
    if(price_list.size() == 2)
    {
        QRegularExpression place_dot(price);
        QString rep_produkt(price_list.at(0) + "." + price_list.at(1));
        price.replace(place_dot, rep_produkt);
    }

    if(plus) {
        m_total = m_total + (numOfItems.toInt() * price.toFloat());
    }
    else {
        m_total = m_total - (numOfItems.toInt() * price.toFloat());
    }

    if(m_total < 0)
        ui->label_osnova->setText("0€");
    else
        ui->label_osnova->setText(QString::number(m_total) + "€");

    ddv = m_total * 0.22;
    if(ddv < 0)
        ui->label_ddv->setText("0€");
    else
        ui->label_ddv->setText(QString::number(ddv) + "€");

    popust = ui->lineEdit_popust->text().toFloat();
    if(popust == 0)
        skupaj = m_total + ddv;
    else
    {
        popust = ui->lineEdit_popust->text().toFloat() / 100.0;
        skupaj = (m_total + ddv) - ((m_total + ddv) * popust);
    }

    if(skupaj < 0)
        ui->label_skupaj->setText("0€");
    else
        ui->label_skupaj->setText(QString::number(skupaj) + "€");
}

void NovRacun::on_treeWidget_dodani_doubleClicked()
{
    int vrstic(0);
    if(ui->treeWidget_dodani->currentItem()->text(1).length() < 43)
        vrstic++;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 57)
        vrstic += 3;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 77)
        vrstic += 4;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 90)
        vrstic += 5;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 108)
        vrstic += 6;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 126)
        vrstic += 7;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 144)
        vrstic += 8;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 162)
        vrstic += 9;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 180)
        vrstic += 10;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 198)
        vrstic += 11;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 216)
        vrstic += 12;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 234)
        vrstic += 13;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 252)
        vrstic += 14;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 270)
        vrstic += 15;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 288)
        vrstic += 16;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 306)
        vrstic += 17;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 324)
        vrstic += 18;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 342)
        vrstic += 19;
    else if(ui->treeWidget_dodani->currentItem()->text(1).length() < 360)
        vrstic += 20;
    else
    {
        QMessageBox error_stringToLong;
        error_stringToLong.critical(this, "Napaka", "Racun vsebuje prevec produktov ali pa je ime produkta predolgo!");
        error_stringToLong.show();
        return;
    }
    m_vrstic -= vrstic;
    QString price = ui->treeWidget_dodani->currentItem()->text(2);
    QString numItems = ui->treeWidget_dodani->currentItem()->text(3);
    CalcSkupaj(price, numItems, false);
    delete ui->treeWidget_dodani->takeTopLevelItem(ui->treeWidget_dodani->currentIndex().row());
    m_itemsAdded--;
}

void NovRacun::Search(QString searchName)
{
    QDir::setCurrent("/usr/home/cloudjunkie/");
    int izbPod = ui->comboBox_narocnik->currentIndex() + 1;
    QString podjetje = QString::number(izbPod) + ".txt";
    // odpre file za branje
    QFile mFile(podjetje);
    // test ce je file odprt
    if(!mFile.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug() << "Error opening mFile for reading in popravi button";
        return;
    }
    // stream za shranit text fila
    QTextStream out(&mFile);
    // prebere celoten dokument in shrani v var
    QString line;
    while(!out.atEnd()){
        line = out.readLine();
        if(line.contains(searchName, Qt::CaseInsensitive)) {
            // locilo med podatki podjetja v filu ime_podjetja;naslov;ddv;email itd itd
            QRegExp rx("[;]");
            // naredimo listo da posamezne podatke locimo in shranimo kot posamezno kategorijo
            QStringList list;
            list = line.split(rx, QString::SkipEmptyParts);
                if(list.at(0) == "Dodano: ")
                    break;
                else
                {
                    // v treeWidget vnesemo vsa podjetja (posamezni podatki iz liste so list.at(?))
                    AddRoot(list.at(0), list.at(1), list.at(2));
                }
        }
    }
    // zapre file
    mFile.close();
}

void NovRacun::on_lineEdit_popust_editingFinished()
{
    QString price = "0";
    QString numItems = "0";
    CalcSkupaj(price, numItems, false);
}

void NovRacun::on_lineEdit_isci_editingFinished()
{
    QString search = ui->lineEdit_isci->text();
    if(search == "")
        Read();
    else
    {
        ui->treeWidget_seznam->clear();
        Search(search);
    }
}

int NovRacun::creatPDF()
{
    // Arhivski del
    QDate inputDate = ui->dateEdit->date();
    QDateTime date = QDateTime::currentDateTime();
    QString items;
    QString product;
    for(int i(0); i < m_itemsAdded; i++)
    {
        items.append("(");
        product = ui->treeWidget_dodani->topLevelItem(i)->text(0);
        items.append(product);
        items.append(") - ");
        product = ui->treeWidget_dodani->topLevelItem(i)->text(1);
        items.append(product);
        items.append(" - (");
        product = ui->treeWidget_dodani->topLevelItem(i)->text(2);
        items.append(product);
        items.append(") | ");
    }

    QFile mFile(m_arhivLogin);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in creatPDF login";
        return 1;
    }
    QTextStream in(&mFile);
    QString mText("");
    while(!in.atEnd())
    {
        mText = in.readLine();
    }
    QStringList mTextList = mText.split(" ; ");
    mText = mTextList.at(1);
    mFile.close();
    QString arhiv = "Nov racun ; " + date.toString("dd.MM.yyyy ; hh:mm:ss.zzz") + " ; "
            + "St_rac: " + ui->lineEdit_stRacuna->text() + " ; "
            + "Vezni_dok: " + ui->lineEdit_sklic->text() + " ; "
            + "Dat_izdaje: " + ui->dateEdit->text() + " ; "
            + "Dat_valute: " + inputDate.addDays(45).toString("d. M. yyyy") + " ; "
            + "Narocnik: " + ui->comboBox_narocnik->currentText() + " ; "
            + "Produkti: " + items + " ; "
            + "Sum_osnova: " + ui->label_osnova->text() + " ; "
            + "Sum_ddv: " + ui->label_ddv->text() + " ; "
            + "Sum_skupaj: " + ui->label_skupaj->text() + " ; "
            + "Opomba: " + ui->lineEdit->text() + " ; "
            + "Rac_napisal: " + mText + " ; "
            + "Placilo: " + "" + " ; " + "Dat_placila: " + "" + " ; " + "Opomba: " + "" + "; (end)" ;
    Arhiv(m_arhivNovRacun, arhiv);
    arhiv = ui->lineEdit_stRacuna->text();
    QStringList arhivLine = arhiv.split("-", QString::SkipEmptyParts);
    arhiv = arhivLine.at(0);
    Arhiv(m_arhivStRacuna, arhiv);

    // PDF del
    QString footer = "<table align=center width=100% height=100%>"
                     "<div align=center>"
                        "<hr width=100% size=2>""<font color=#0000aa size=2>""|""</font>""<font color=#aa0000 size=2>"" ELRA SETI d.o.o., proizvodnja setov iz kablov in vodnikov ""</font>""<font color=#0000aa size=2>""|""</font>""<font color=#aa0000 size=2>"" Andraž nad Polzelo 74a ""</font>""<font color=#0000aa size=2>""|""</font>""<font color=#aa0000 size=2>"" SI-3313 Polzela ""</font>""<font color=#0000aa size=2>""|""</font>""<font color=#aa0000 size=2>"" Slovenija ""</font>""<font color=#0000aa size=2>""|""</font>""<br>"
                        "<font color=#aa0000 size=2>""|""</font>""<font color=#0000aa size=2>"" T+386(0)3 897 44 46 ""</font>""<font color=#aa0000 size=2>""|""</font>""<font color=#0000aa size=2>"" GSM +386 (0)41 326 103 ""</font>""<font color=#aa0000 size=2>""|""</font>""<font color=#0000aa size=2>"" F+386(0)3 897 44 47 ""</font>""<font color=#aa0000 size=2>""|""</font>""<font color=#0000aa size=2>"" info@elraseti.si ""</font>""<font color=#aa0000 size=2>""|""</font>""<br>"
                        "<font color=#000000 size=2>"
                        "| ID DDV (VAT nr.): SI83452010 | Matična št.: 6514812000 | Reg. št.: Srg 2013/53219 |<br>"
                        "| TRR (BANK): SBERBANK: SI56 3000 0001 1326 144 | osnovni kapital 7.500,00€ |"
                        "</font>"
                     "</div>"
                     "</table>";

    QString header = "<div align=center>"
                        "<font color='#aa0000'>"
                            "<h1>"
                                "ELRA SETI D.O.O.<hr width=100% size=2>"
                            "</h1>"
                        "</font>"
                     "</div>";

    QString tabela_za_dodane_produkte = "<table align=left width=100% height=100%>"
                                        "<tr>"
                                            "<th>"
                                                "<div align=center>"
                                                    "<hr width=100% size=1>Zap. št."
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>"
                                                    "<hr width=100% size=1>P. ident"
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>"
                                                    "<hr width=100% size=1>Opis produkta"
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>"
                                                    "<hr width=100% size=1>Količina EM"
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>"
                                                    "<hr width=100% size=1>Cena brez DDV"
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>"
                                                    "<hr width=100% size=1>DDV %"
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>"
                                                    "<hr width=100% size=1>Vrednost brez DDV"
                                                "</div>"
                                            "</th>"
                                        "</tr>";

    QString opombe_izdelal = "<table align=left width=100% height=100%>"
                             "<div align=left>"
                                 "<tr>"
                                     "<th>"
                                         "<div align=left>"
                                             "Opombe: " + ui->lineEdit->text() + "<br>"
                                         "</div>"
                                     "</th>"

                                 "</tr>"
                                 "<tr>"
                                     "<th>"
                                         "<div align=left>"
                                             "Dokument sestavil:"
                                         "</div>"
                                     "</th>"
                                     "<th>"
                                         "<div align=right>"
                                             "Srecko Rajh"
                                         "</div>"
                                     "</th>"

                                 "</tr>"
                                 "<tr>"
                                     "<th>"
                                         "<div align=left>"
                                             "Nejc Sedovnik"
                                         "</div>"
                                     "</th>"
                                     "<th>"
                                         "<div align=right>"
                                             "prokurist  "
                                         "</div>"
                                     "</th>"
                                 "</tr>"
                             "</div>"
                         "</table>";

    QString produkti("");

    for(int i(0); i < m_itemsAdded; i++)
    {
        if(i == 0)
            produkti.append("<tr>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""</tr>");

        produkti.append("<tr>");
        produkti.append("<th>");
        produkti.append("<div align=center>");
        produkti.append(QString::number(i+1));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(0));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(1));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(3));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(2));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>");
        produkti.append("22%");
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(4) + "€");
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("</tr>");
    }

    QString produkti_brezCen("");
    for(int i(0); i < m_itemsAdded; i++)
    {
        if(i == 0)
            produkti_brezCen.append("<tr>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""<th>""<hr width=100% size=1>""""</th>""</tr>");

        produkti_brezCen.append("<tr>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>");
        produkti_brezCen.append(QString::number(i+1));
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>");
        produkti_brezCen.append(ui->treeWidget_dodani->topLevelItem(i)->text(0));
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>");
        produkti_brezCen.append(ui->treeWidget_dodani->topLevelItem(i)->text(1));
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>");
        produkti_brezCen.append(ui->treeWidget_dodani->topLevelItem(i)->text(3));
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>");
        produkti_brezCen.append("");
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>");
        produkti_brezCen.append("");
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>");
        produkti_brezCen.append("");
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("</tr>");
    }

    if(m_vrstic < m_max_produktov)
    {
        for(int i(0); i <= (m_max_produktov - m_vrstic); i++)
        {
            produkti.append("<tr>");
            produkti.append("<th>");
            produkti.append("<div align=center>");
            produkti.append("");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>");
            produkti.append("");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>");
            produkti.append("");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>");
            produkti.append("");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>");
            produkti.append("");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>");
            produkti.append("");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>");
            produkti.append("");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("</tr>");
            produkti_brezCen.append("<tr>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>");
            produkti_brezCen.append("");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>");
            produkti_brezCen.append("");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>");
            produkti_brezCen.append("");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>");
            produkti_brezCen.append("");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>");
            produkti_brezCen.append("");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>");
            produkti_brezCen.append("");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>");
            produkti_brezCen.append("");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("</tr>");
        }
    }

    QString sklic = ui->lineEdit_stRacuna->text();
    QStringList sklicList = sklic.split("-", QString::SkipEmptyParts);
    QString text_racun =
            /**************************** RACUN *********************************/
            header +
            "<table align=left width=100% height=100%>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "<font color='#ffffff'>"
                                "Izdelal: Nejc Sedovnik"
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<font size=5>"
                            "<div align=left>"
                                "<b>Št. računa:</b>"
                            "</div>"
                        "</font>"
                    "</th>"
                    "<th>"
                        "<font size=5>"
                            "<div align=left>"
                                "<b>" + ui->lineEdit_stRacuna->text() + "</b>"
                            "</div>"
                        "</font>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Kraj izdaje:"
                        "</div>"
                    "</th>"
                  "<th>"
                      "<div align=left>"
                          "Andraž nad Polzelo"
                      "</div>"
                  "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Vezni dokument:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            + ui->lineEdit_sklic->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            "<font size=4>"
                                "<b><u>Naročnik:</u></b>"
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Številka dobavnice:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            + ui->lineEdit_stRacuna->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            + ui->comboBox_narocnik->currentText() +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Datum opr. posla:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            + ui->dateEdit->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            + m_naslov +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Datum izd. računa:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            + ui->dateEdit->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            + m_posta +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Datum valute:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            + inputDate.addDays(45).toString("d. M. yyyy") +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "TRR:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "SI56 3000 0001 1326 144"
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            "<font size=1>"
                                + m_email +
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Naša ID št. za DDV:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "SI83452010"
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            "ID za DDV: " + m_ddv +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Telefon:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "041 326 103"
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Sklic:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "00 " + sklicList.at(0) + " " + sklicList.at(1) +
                        "</div>"
                    "</th>"
                "</tr>"
            "</table>"
            + tabela_za_dodane_produkte
            + produkti
            +
            "</table>"

            "<table align=left width=100% height=100%>"
                "<div align=left>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "<hr width=100% size=2>OSNOVA"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>" + ui->label_osnova->text() +
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "DDV %"
                            "</div>"
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            + ui->label_ddv->text() +
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "Popust %"
                            "</div>"
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            + ui->lineEdit_popust->text() + "%"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "<font size=4>"
                                "<hr width=100% size=2>ZA PLAČILO €<hr width=100% size=2>"
                            "</font>"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                                "<hr width=100% size=2>" + ui->label_skupaj->text() + "<hr width=100% size=2>"
                            "</font>"
                        "</th>"
                    "</tr>"
            "</table>"
            + opombe_izdelal
            + footer;

    QString text_dobavnica =
            /**************************** DOBAVNICA *********************************/
            "<font color=#ffffff size=0>""prazno""</font>" +
            header +
            "<table align=left width=100% height=100%>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "<font color='#ffffff'>"
                                "Izdelal: Nejc Sedovnik"
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<font size=5>"
                            "<div align=left>"
                                "<b>Dobavnica št.:</b>"
                            "</div>"
                        "</font>"
                    "</th>"
                    "<th>"
                        "<font size=5>"
                            "<div align=left>"
                                "<b>" + ui->lineEdit_stRacuna->text() + "</b>"
                            "</div>"
                        "</font>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Kraj izdaje:"
                        "</div>"
                    "</th>"
                  "<th>"
                      "<div align=left>"
                          "Andraž nad Polzelo"
                      "</div>"
                  "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Vezni dokument:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            + ui->lineEdit_sklic->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            "<font size=4>"
                                "<b><u>Naročnik:</u></b>"
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Datum opr. posla:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            + ui->dateEdit->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            + ui->comboBox_narocnik->currentText() +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Sklic:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "00 " + sklicList.at(0) + " " + sklicList.at(1) +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            + m_naslov +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "Telefon:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "041 326 103"
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            + m_posta +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            "<font size=1>"
                                + m_email +
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                "</tr>"
            "</table>"

            + tabela_za_dodane_produkte
            + produkti_brezCen
            +
            "</table>"

            "<table align=left width=100% height=100%>"
                "<div align=left>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=4>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                    "</tr>"
            "</table>"
            + opombe_izdelal
            + footer;

    QTextDocument document_racun;
    QTextCursor cursor(&document_racun);
    cursor.insertHtml(text_racun);
    QTextBlockFormat blockFormat;
    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
    cursor.insertBlock(blockFormat);
    cursor.insertHtml(text_dobavnica);

    QPrinter* printer = new QPrinter(QPrinter::HighResolution);
    QPrintDialog printDlg(printer);
    QList<QWidget*> childWidgets = printDlg.findChildren<QWidget*>(QLatin1String("printers"), Qt::FindChildrenRecursively);

    if (childWidgets.count() == 1)
    {
       QComboBox* comboBox(qobject_cast<QComboBox*>(childWidgets.at(0)));

       if (comboBox && comboBox->findText(printer->printerName()))
          comboBox->setCurrentText(printer->printerName());

    }

    if (printDlg.exec() == QDialog::Accepted)
    {
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setPaperSize(QPrinter::A4);
        ui->dateEdit->setDisplayFormat("ddMMyyyy");
        QString stevilka_racuna = ui->dateEdit->text();
        QString stranka = ui->comboBox_narocnik->currentText();
        QRegExp rx("[ ]");
        QStringList list;
        list = stranka.split(rx, QString::SkipEmptyParts);

        if(list.at(1) == "d.o.o." || list.at(1) == "doo" || list.at(1) == "D.O.O."  || list.at(1) == "DOO" || list.at(1) == "s.p." || list.at(1) == "sp" || list.at(1) == "s.p" || list.at(1) == "S.P." || list.at(1) == "S.P" || list.at(1) == "SP")
            stranka = list.at(0);
        else
            stranka = list.at(0) + "_" + list.at(1);

        QString output = "/usr/home/cloudjunkie/" "RACUN_IN_DOBAVNICA_" + stranka + "_" + stevilka_racuna + ".pdf";
        printer->setOutputFileName(output);
        qreal left = 0, right = 0, top = 0, bottom = 0;
        printer->setPageMargins(left, top, right, bottom, QPrinter::Millimeter);
        document_racun.print(printer);
        ui->dateEdit->setDisplayFormat("d. M. yyyy");
    }
    NovRacun::close();
    return 0;
}

void NovRacun::on_pushButton_dodajNovProdukt_clicked()
{
    DodajProdukt produkt;
    produkt.setModal(true);
    produkt.exec();
    Read();
}
