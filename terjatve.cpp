#include "terjatve.h"
#include "ui_terjatve.h"

Terjatve::Terjatve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Terjatve), m_currentDir(QDir::currentPath()), m_stranke(m_currentDir + "/company_file.txt"), m_terjatve(m_currentDir + "/arhiv_novRacun.txt"), m_obveznosti(m_currentDir + "/arhiv_upniki.txt"), m_upnikiSeznam(m_currentDir + "/arhiv_upnikiSeznam.txt"), m_totalTerjatve(0), m_totalObveznosti(0), m_totalTerMinObv(0)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Terjatve in obveznosti");
    this->setWindowFlags(Qt::Window);
    ui->dateEdit_obveznostiOd->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_obveznostiDo->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_terjatveOd->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_terjatveDo->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_terjatveOd->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit_terjatveDo->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit_obveznostiOd->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit_obveznostiDo->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit_terjatveOd->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEdit_terjatveDo->setDate(QDate::currentDate());
    ui->dateEdit_obveznostiOd->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEdit_obveznostiDo->setDate(QDate::currentDate());
    ui->treeWidget_obveznosti->setSortingEnabled(true);
    ui->treeWidget_terjatve->setSortingEnabled(true);
    ui->treeWidget_obveznosti->setRootIsDecorated(false);
    ui->treeWidget_terjatve->setRootIsDecorated(false);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    ui->treeWidget_terjatve->header()->setStretchLastSection(false);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    ui->treeWidget_obveznosti->header()->setStretchLastSection(false);
    AddItemsToComboBox();
    ui->label_skupajTerjatve->setText("€" + QString::number(m_totalTerjatve, 'f', 2));
    ui->label_skupajObveznosti->setText("€" + QString::number(m_totalObveznosti, 'f', 2));
    if(m_totalTerMinObv < 0)
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: red;}");
    else
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: green}");
    ui->label_terjatve_obveznosti->setText("€" + QString::number(m_totalTerMinObv, 'f', 2));
    ui->pushButton_isci->setFocus();
}

Terjatve::~Terjatve()
{
    delete ui;
}

void Terjatve::AddItemsToComboBox()
{
    ui->comboBox_stranke->clear();
    /* TERJATVE */
    ui->comboBox_stranke->addItem("Vse terjatve");
    QFile mFile(m_stranke);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Terjatve AddItemsToComboBox";
        return;
    }
    else
    {
        QString mText("");
        QRegularExpression exp(";");
        QStringList list;
        while(!mFile.atEnd())
        {
            mText = mFile.readLine();
            list = mText.split(exp, QString::SkipEmptyParts);
            ui->comboBox_stranke->addItem(list.at(1));
        }
    }
    mFile.close();

    /* OBVEZNOSTI */
    ui->comboBox_upniki->clear();
    ui->comboBox_upniki->addItem("Vse obveznosti");
    QFile mFileUpniki(m_upnikiSeznam);
    if(!mFileUpniki.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Terjatve AddItemsToComboBox";
        return;
    }
    else
    {
        QString mText("");
        QRegularExpression exp(";");
        QStringList list;
        while(!mFileUpniki.atEnd())
        {
            mText = mFileUpniki.readLine();
            list = mText.split(exp, QString::SkipEmptyParts);
            ui->comboBox_upniki->addItem(list.at(0));
        }
    }
    mFileUpniki.close();
}

void Terjatve::AddRootTerjatve(QStringList textList)
{
    QStringList list;
    QRegularExpression exp(":");
    QString st_rac = textList.at(3);
    list = st_rac.split(exp, QString::SkipEmptyParts);
    st_rac = list.at(1);
    QString stranka = textList.at(7);
    list = stranka.split(exp, QString::SkipEmptyParts);
    stranka = list.at(1);
    QString dat_izdaje = textList.at(5);
    list = dat_izdaje.split(exp, QString::SkipEmptyParts);
    dat_izdaje = list.at(1);
    QString dat_valute = textList.at(6);
    list = dat_valute.split(exp, QString::SkipEmptyParts);
    dat_valute = list.at(1);
    QString znesek = textList.at(11);
    list = znesek.split(exp, QString::SkipEmptyParts);
    znesek = list.at(1);
    znesek.remove('?');
    m_totalTerjatve += znesek.toDouble();
    znesek.prepend("€");
    QString opomba = textList.at(12);
    list = opomba.split(exp, QString::SkipEmptyParts);
    opomba = list.at(1);
    QString placilo = textList.at(14);
    list = placilo.split(exp, QString::SkipEmptyParts);
    placilo = list.at(1);
    m_totalTerjatve -= placilo.toDouble();
    placilo.prepend("€");
    QString dat_placila = textList.at(15);
    list = dat_placila.split(exp, QString::SkipEmptyParts);
    dat_placila = list.at(1);
    QString itemDatum = textList.at(1);
    itemDatum.remove(0,1);
    itemDatum.remove(-1,1);
    itemDatum.append(textList.at(2));
    itemDatum.remove(-1,1);
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_terjatve);
    itm->setText(0, st_rac);
    itm->setText(1, stranka);
    itm->setText(2, dat_izdaje);
    itm->setText(3, dat_valute);
    itm->setText(4, znesek);
    itm->setText(7, opomba);
    itm->setText(5, placilo);
    itm->setText(6, dat_placila);
    itm->setText(8, itemDatum);

    if(znesek.remove("€").toDouble() > placilo.remove("€").toDouble())
    {
        for(int i(0); i < itm->columnCount(); i++)
            itm->setTextColor(i, QColor("red"));
    }
    else if(znesek.remove("€").toDouble() < placilo.remove("€").toDouble())
    {
        for(int i(0); i < itm->columnCount(); i++)
            itm->setBackgroundColor(i, QColor("red"));
    }
    else
    {
        for(int i(0); i < itm->columnCount(); i++)
            itm->setTextColor(i, QColor("green"));
    }

    ui->treeWidget_terjatve->addTopLevelItem(itm);
}

void Terjatve::ReadTerjatve()
{
    ui->treeWidget_terjatve->clear();
    m_totalTerjatve = 0;
    QFile mFile(m_terjatve);
    QString mText("");
    QRegularExpression exp(";");
    QRegularExpression exp_second(": ");
    QStringList textList;
    QStringList tmp;
    QString stranka("");
    QString date("");
    QDate date_Od_Do;
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Terjatve ReadTerjatve()";
        return;
    }
    else
    {
        while(!mFile.atEnd())
        {
            mText = mFile.readLine();
            textList = mText.split(exp, QString::SkipEmptyParts);
            stranka = textList.at(7);
            tmp = stranka.split(exp_second, QString::SkipEmptyParts);
            stranka = tmp.at(1);
            stranka.remove(-1,1);
            date = textList.at(5);
            tmp = date.split(exp_second, QString::SkipEmptyParts);
            date = tmp.at(1);
            date.remove(-1,1);
            date_Od_Do = date_Od_Do.fromString(date, "d. M. yyyy");
            if(ui->comboBox_stranke->currentText() == "Vse terjatve" && date_Od_Do >= ui->dateEdit_terjatveOd->date() && date_Od_Do <= ui->dateEdit_terjatveDo->date())
            {
                AddRootTerjatve(textList);
                continue;
            }
            else if(stranka == ui->comboBox_stranke->currentText() && date_Od_Do >= ui->dateEdit_terjatveOd->date() && date_Od_Do <= ui->dateEdit_terjatveDo->date())
                AddRootTerjatve(textList);
        }
    }
    mFile.close();
    ui->treeWidget_terjatve->sortByColumn(3, Qt::DescendingOrder);
    m_totalTerMinObv = m_totalTerjatve - m_totalObveznosti;
}

void Terjatve::AddRootObveznosti(QStringList textList)
{
    QStringList list;
    QRegularExpression exp(": ");
    QString listina = textList.at(3);
    list = listina.split(exp, QString::SkipEmptyParts);
    listina = list.at(1);
    QString stranka = textList.at(4);
    list = stranka.split(exp, QString::SkipEmptyParts);
    stranka = list.at(1);
    QString znesek = textList.at(5);
    list = znesek.split(exp, QString::SkipEmptyParts);
    znesek = list.at(1);
    znesek.remove('?');
    m_totalObveznosti += znesek.toDouble();
    znesek.prepend("€");
    QString dat_valute = textList.at(6);
    list = dat_valute.split(exp, QString::SkipEmptyParts);
    dat_valute = list.at(1);
    QString placilo = textList.at(7);
    list = placilo.split(exp, QString::SkipEmptyParts);
    placilo = list.at(1);
    m_totalObveznosti -= placilo.toDouble();
    placilo.prepend("€");
    QString dat_placila = textList.at(8);
    list = dat_placila.split(exp, QString::SkipEmptyParts);
    dat_placila = list.at(1);
    QString opomba = textList.at(9);
    list = opomba.split(exp, QString::SkipEmptyParts);
    opomba = list.at(1);
    QString ident = textList.at(1) + textList.at(2);
    ident.remove(-1,1);
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_obveznosti);
    itm->setText(0, listina);
    itm->setText(1, stranka);
    itm->setText(2, znesek);
    itm->setText(3, dat_valute);
    itm->setText(4, placilo);
    itm->setText(5, dat_placila);
    itm->setText(6, opomba);
    itm->setText(7, ident);
    if(znesek.remove("€").toDouble() > placilo.remove("€").toDouble())
    {
        for(int i(0); i < itm->columnCount(); i++)
            itm->setTextColor(i, QColor("red"));
    }
    else if(znesek.remove("€").toDouble() < placilo.remove("€").toDouble())
    {
        for(int i(0); i < itm->columnCount(); i++)
            itm->setBackgroundColor(i, QColor("red"));
    }
    else
    {
        for(int i(0); i < itm->columnCount(); i++)
            itm->setTextColor(i, QColor("green"));
    }
    ui->treeWidget_obveznosti->addTopLevelItem(itm);
}

void Terjatve::ReadObveznosti()
{
    ui->treeWidget_obveznosti->clear();
    m_totalObveznosti = 0;
    QFile mFile(m_obveznosti);
    QString mText("");
    QRegularExpression exp("; ");
    QRegularExpression exp_second(": ");
    QStringList textList;
    QStringList tmp;
    QString stranka("");
    QString date("");
    QDate date_Od_Do;
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Terjatve ReadObveznosti()";
        return;
    }
    else
    {
        while(!mFile.atEnd())
        {
            mText = mFile.readLine();
            textList = mText.split(exp, QString::SkipEmptyParts);
            stranka = textList.at(4);
            tmp = stranka.split(exp_second, QString::SkipEmptyParts);
            stranka = tmp.at(1);
            stranka.remove(-1,1);
            date = textList.at(1);
            date.remove(-1,1);
            date_Od_Do = date_Od_Do.fromString(date, "d. M. yyyy");
            if(ui->comboBox_upniki->currentText() == "Vse obveznosti" && date_Od_Do >= ui->dateEdit_obveznostiOd->date() && date_Od_Do <= ui->dateEdit_obveznostiDo->date())
            {
                AddRootObveznosti(textList);
                continue;
            }
            else if(stranka == ui->comboBox_upniki->currentText() && date_Od_Do >= ui->dateEdit_obveznostiOd->date() && date_Od_Do <= ui->dateEdit_obveznostiDo->date())
                AddRootObveznosti(textList);
        }
    }
    mFile.close();
    ui->treeWidget_obveznosti->sortByColumn(3, Qt::DescendingOrder);
    m_totalTerMinObv = m_totalTerjatve - m_totalObveznosti;
}

void Terjatve::Search(QString searchName, QString file, bool ter_obv)
{
    QFile mFile(file);
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
            // v treeWidget vnesemo vsa podjetja (posamezni podatki iz liste so list.at(?))
            if(ter_obv)
                AddRootTerjatve(list);
            else
                AddRootObveznosti(list);
        }
    }
    // zapre file
    mFile.close();
}

void Terjatve::on_comboBox_stranke_currentIndexChanged()
{
    if(ui->comboBox_stranke->currentText() == "Vse terjatve")
    {
        ui->lineEdit_iskalnikTerjatve->setEnabled(true);
        ui->lineEdit_iskalnikTerjatve->clear();
    }
    else
    {
        ui->lineEdit_iskalnikTerjatve->setEnabled(false);
        ui->lineEdit_iskalnikTerjatve->clear();
    }
    ReadTerjatve();
}

void Terjatve::on_dateEdit_terjatveOd_editingFinished()
{
    ReadTerjatve();
}

void Terjatve::on_dateEdit_terjatveDo_editingFinished()
{
     ReadTerjatve();
}

// vnos obveznosti
void Terjatve::on_pushButton_clicked()
{
    VnosObveznosti vnos;
    vnos.setModal(true);
    vnos.exec();
    ui->treeWidget_obveznosti->clear();
    AddItemsToComboBox();
    ReadObveznosti();
    ui->label_skupajTerjatve->setText("€" + QString::number(m_totalTerjatve, 'f', 2));
    ui->label_skupajObveznosti->setText("€" + QString::number(m_totalObveznosti, 'f', 2));
    if(m_totalTerMinObv < 0)
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: red;}");
    else
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: green}");
    ui->label_terjatve_obveznosti->setText("€" + QString::number(m_totalTerMinObv, 'f', 2));
}

void Terjatve::on_comboBox_upniki_currentIndexChanged()
{
    if(ui->comboBox_upniki->currentText() == "Vse obveznosti")
    {
        ui->lineEdit_iskalnikObveznosti->setEnabled(true);
        ui->lineEdit_iskalnikObveznosti->clear();
    }
    else
    {
        ui->lineEdit_iskalnikObveznosti->setEnabled(false);
        ui->lineEdit_iskalnikObveznosti->clear();
    }
    ReadObveznosti();
}

void Terjatve::on_dateEdit_obveznostiOd_editingFinished()
{
    ReadObveznosti();
}

void Terjatve::on_dateEdit_obveznostiDo_editingFinished()
{
    ReadObveznosti();
}

// vnos nov racun
void Terjatve::on_pushButton_2_clicked()
{
    NovRacun racun;
    racun.setModal(true);
    racun.exec();
    AddItemsToComboBox();
    ReadTerjatve();
    ui->label_skupajTerjatve->setText("€" + QString::number(m_totalTerjatve, 'f', 2));
    ui->label_skupajObveznosti->setText("€" + QString::number(m_totalObveznosti, 'f', 2));
    if(m_totalTerMinObv < 0)
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: red;}");
    else
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: green}");
    ui->label_terjatve_obveznosti->setText("€" + QString::number(m_totalTerMinObv, 'f', 2));
}

void Terjatve::on_treeWidget_terjatve_itemDoubleClicked(QTreeWidgetItem *item)
{
    qDebug() << item;
    QString itemDatum = item->text(8);
    QFile mFile(m_terjatve);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening in treeWidget_terjatve_itemDoubleClicked";
        return;
    }
    else
    {
        QTextStream in(&mFile);
        QString allText("");
        QString text_stRacuna("");
        QString tmp("");
        QStringList list;
        QRegularExpression exp(" ;");
        QStringList placiloList;
        PlaciloRacuna placilo;
        QStringList tmp_list;
        allText = in.readAll();
        mFile.close();
        if(!mFile.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "Error opening in treeWidget_terjatve_itemDoubleClicked";
            return;
        }
        else
        {
            while(!in.atEnd())
            {
                text_stRacuna = in.readLine();
                list = text_stRacuna.split(exp, QString::SkipEmptyParts);
                tmp = list.at(1);
                tmp.remove(0,1);
                tmp.append(list.at(2));
                if(tmp == itemDatum)
                    break;
            }
            tmp = text_stRacuna;
            tmp_list = tmp.split(exp, QString::SkipEmptyParts);
            placilo.setModal(true);
            QString opomba = tmp_list.at(12);
            opomba.remove("Opomba: ");
            opomba.remove(0,1);
            QString cena = tmp_list.at(14);
            cena.remove("Placilo: ");
            cena.remove(0,1);
            QString datum = tmp_list.at(15);
            datum.remove("Dat_placila: ");
            datum.remove(0,1);
            if(opomba == " ") opomba = "";
            if(cena == " ") cena = "";
            placilo.setOpombe(opomba, cena, datum);
            placilo.exec();
            placiloList = placilo.on_pushButton_clicked();
            tmp.replace(QString(tmp_list.at(14)), QString(" Placilo: " + placiloList.at(0)));
            tmp.replace(QString(tmp_list.at(15)), QString(" Dat_placila: " + placiloList.at(1)));
            tmp.replace(QString(tmp_list.at(12)), QString(" Opomba: " + placiloList.at(2)));
            allText.replace(text_stRacuna, tmp);
            mFile.close();
            if(!mFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
            {
                qDebug() << "Error opening in treeWidget_terjatve_itemDoubleClicked";
                return;
            }
            else
            {
                in << allText;
                mFile.flush();
                mFile.close();
            }
        }
    }
    AddItemsToComboBox();
    ReadTerjatve();
    ui->label_skupajTerjatve->setText("€" + QString::number(m_totalTerjatve, 'f', 2));
    ui->label_skupajObveznosti->setText("€" + QString::number(m_totalObveznosti, 'f', 2));
    if(m_totalTerMinObv < 0)
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: red;}");
    else
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: green}");
    ui->label_terjatve_obveznosti->setText("€" + QString::number(m_totalTerMinObv, 'f', 2));
}

void Terjatve::on_treeWidget_obveznosti_itemDoubleClicked(QTreeWidgetItem *item)
{
    qDebug() << item;
    QString itemDatum = item->text(7);
    QFile mFile(m_obveznosti);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening in treeWidget_terjatve_itemDoubleClicked";
        return;
    }
    else
    {
        QTextStream in(&mFile);
        QString allText("");
        QString text_obveznost("");
        QString tmp("");
        QStringList list;
        QRegularExpression exp(" ;");
        QStringList placiloList;
        PlaciloRacuna placilo;
        QStringList tmp_list;
        allText = in.readAll();
        mFile.close();

        if(!mFile.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "Error opening in treeWidget_terjatve_itemDoubleClicked";
            return;
        }
        else
        {
            while(!in.atEnd())
            {
                text_obveznost = in.readLine();
                list = text_obveznost.split(exp, QString::SkipEmptyParts);
                tmp = list.at(1) + list.at(2);
                tmp.remove(0,1);
                if(tmp == itemDatum)
                    break;
            }
            tmp = text_obveznost;
            tmp_list = text_obveznost.split(exp, QString::SkipEmptyParts);
            placilo.setModal(true);
            QString opomba = tmp_list.at(9);
            opomba.remove("Opombe: ");
            opomba.remove(0,1);
            QString cena = tmp_list.at(7);
            cena.remove("Placilo: ");
            cena.remove(0,1);
            QString datum = tmp_list.at(8);
            datum.remove("Dat_placila: ");
            datum.remove(0,1);
            placilo.setOpombe(opomba, cena, datum);
            placilo.exec();
            placiloList = placilo.on_pushButton_clicked();
            tmp.replace(QString(tmp_list.at(7)), QString(" Placilo: " + placiloList.at(0)));
            tmp.replace(QString(tmp_list.at(8)), QString(" Dat_placila: " + placiloList.at(1)));
            tmp.replace(QString(tmp_list.at(9)), QString(" Opombe: " + placiloList.at(2)));
            allText.replace(text_obveznost, tmp);
            mFile.close();
            if(!mFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
            {
                qDebug() << "Error opening in treeWidget_terjatve_itemDoubleClicked";
                return;
            }
            else
            {
                in << allText;
                mFile.flush();
                mFile.close();
            }
        }
    }
    AddItemsToComboBox();
    ReadObveznosti();
    ui->label_skupajTerjatve->setText("€" + QString::number(m_totalTerjatve, 'f', 2));
    ui->label_skupajObveznosti->setText("€" + QString::number(m_totalObveznosti, 'f', 2));
    if(m_totalTerMinObv < 0)
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: red;}");
    else
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: green}");
    ui->label_terjatve_obveznosti->setText("€" + QString::number(m_totalTerMinObv, 'f', 2));
}

void Terjatve::on_pushButton_statistika_clicked()
{
    Statistic statistika;
    statistika.setModal(true);
    statistika.exec();
}

void Terjatve::on_lineEdit_iskalnikTerjatve_textChanged()
{
    ui->dateEdit_terjatveOd->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEdit_terjatveDo->setDate(QDate::currentDate());
    ui->treeWidget_terjatve->clear();
    QString search = ui->lineEdit_iskalnikTerjatve->text();
    Search(search, m_terjatve, true);
}

void Terjatve::on_lineEdit_iskalnikObveznosti_textChanged()
{
    ui->dateEdit_obveznostiOd->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEdit_obveznostiDo->setDate(QDate::currentDate());
    ui->treeWidget_obveznosti->clear();
    QString search = ui->lineEdit_iskalnikObveznosti->text();
    Search(search, m_obveznosti, false);
}
