#include "terjatve.h"
#include "ui_terjatve.h"

Terjatve::Terjatve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Terjatve), m_currentDir(QDir::currentPath()), m_stranke(m_currentDir + "/company_file.txt"), m_terjatve(m_currentDir + "/arhiv_novRacun.txt"),
    m_obveznosti(m_currentDir + "/arhiv_upniki.txt"), m_upnikiSeznam(m_currentDir + "/arhiv_upnikiSeznam.txt"), m_shrani(m_currentDir + "/settings.txt"), m_fileShrani(m_currentDir), m_totalTerjatve(0), m_totalObveznosti(0),
    m_totalTerMinObv(0), m_totalTerjatveStaro(0), m_totalObveznostiStaro(0), m_show_child(false)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Terjatve in obveznosti");
    this->setWindowFlags(Qt::Window);
    this->showMaximized();
    ui->dateEdit_obveznostiOd->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_obveznostiDo->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_terjatveOd->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_terjatveDo->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_terjatveOd->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit_terjatveDo->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit_obveznostiOd->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit_obveznostiDo->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit_terjatveOd->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEdit_terjatveDo->setDate(QDate(QDate::currentDate().year(),12,31));
    ui->dateEdit_obveznostiOd->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEdit_obveznostiDo->setDate(QDate(QDate::currentDate().year(),12,31));
    ui->treeWidget_obveznosti->setSortingEnabled(true);
    ui->treeWidget_terjatve->setSortingEnabled(true);
    ui->treeWidget_obveznosti->setRootIsDecorated(false);
    ui->treeWidget_terjatve->setRootIsDecorated(false);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(3, QHeaderView::Interactive);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(4, QHeaderView::Interactive);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(5, QHeaderView::Interactive);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(6, QHeaderView::Interactive);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(7, QHeaderView::Interactive);
    ui->treeWidget_terjatve->header()->setSectionResizeMode(8, QHeaderView::Interactive);
    ui->treeWidget_terjatve->header()->setStretchLastSection(false);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(3, QHeaderView::Interactive);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(4, QHeaderView::Interactive);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(5, QHeaderView::Interactive);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(6, QHeaderView::Interactive);
    ui->treeWidget_obveznosti->header()->setSectionResizeMode(7, QHeaderView::Interactive);
    ui->treeWidget_obveznosti->header()->setStretchLastSection(false);
    AddItemsToComboBox();
    LabelsUpdate();
    Shrani();
}

Terjatve::~Terjatve()
{
    delete ui;
}

void Terjatve::CloseChild() {
    m_show_child = false;
}

void Terjatve::closeEvent(QCloseEvent *) {
    emit close_me();
}

void Terjatve::Shrani()
{
    QFile mFile(m_shrani);
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QRegularExpression exp(" ;");
    QString mLine("");
    QStringList mList;
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Shrani() in novracun.cpp";
        return;
    }
    else
    {
        mLine = in.readLine();
        mList = mLine.split(exp, QString::SkipEmptyParts);
        if(mList.at(0) == "Shrani v:")
        {
            m_fileShrani = mList.at(1);
            m_fileShrani.remove(0,1);
        }
    }
    mFile.close();
}

void Terjatve::AddItemsToComboBox() {
    /* OBVEZNOSTI */
    ui->comboBox->clear();
    ui->comboBox->addItem(QString::fromUtf8("Vse obveznosti"));
    QFile mFileUpniki(m_upnikiSeznam);
    if(!mFileUpniki.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in Terjatve AddItemsToComboBox";
        return;
    } else {
        QTextStream in(&mFileUpniki);
        in.setCodec("UTF-8");
        QString mText("");
        QRegularExpression exp(";");
        QStringList list;
        while(!in.atEnd()) {
            mText = in.readLine();
            list = mText.split(exp, QString::SkipEmptyParts);
            ui->comboBox->addItems(list);
        }
    }
    mFileUpniki.close();

    /* TERJATVE */
    ui->comboBox_stranke->clear();
    ui->comboBox_stranke->addItem(QString::fromUtf8("Vse terjatve"));
    QFile mFile(m_stranke);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in Terjatve AddItemsToComboBox";
        return;
    } else {
        QTextStream in(&mFile);
        in.setCodec("UTF-8");
        QString mText("");
        QRegularExpression exp(";");
        QStringList list;
        while(!in.atEnd()) {
            mText = in.readLine();
            list = mText.split(exp, QString::SkipEmptyParts);
            ui->comboBox_stranke->addItem(list.at(1));
        }
    }
    mFile.close();
}

void Terjatve::AddRootTerjatve(QStringList textList, bool staro) {
    if(!staro) {
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
        if(znesek.remove("€").toDouble() > placilo.remove("€").toDouble()) {
            for(int i(0); i < itm->columnCount(); i++)
                itm->setTextColor(i, QColor("red"));
        }
        else if(znesek.remove("€").toDouble() < placilo.remove("€").toDouble()) {
            for(int i(0); i < itm->columnCount(); i++)
                itm->setBackgroundColor(i, QColor("red"));
        } else {
            for(int i(0); i < itm->columnCount(); i++)
                itm->setTextColor(i, QColor("green"));
        }
        ui->treeWidget_terjatve->addTopLevelItem(itm);
    } else {
        QStringList list;
        QRegularExpression exp(":");
        QString znesek = textList.at(11);
        list = znesek.split(exp, QString::SkipEmptyParts);
        znesek = list.at(1);
        znesek.remove('?');
        m_totalTerjatveStaro += znesek.toDouble();
        QString placilo = textList.at(14);
        list = placilo.split(exp, QString::SkipEmptyParts);
        placilo = list.at(1);
        m_totalTerjatveStaro -= placilo.toDouble();
    }
}

void Terjatve::ReadTerjatve(bool neplacani) {
    ui->treeWidget_terjatve->clear();
    m_totalTerjatve = 0;
    m_totalTerjatveStaro = 0;
    QFile mFile(m_terjatve);
    QString mText("");
    QRegularExpression exp(";");
    QRegularExpression exp_second(": ");
    QStringList textList;
    QStringList tmp;
    QString stranka("");
    QString date("");
    QString opomba("");
    QString placano("");
    QString cena("");
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
            opomba = textList.at(12);
            placano = textList.at(14);
            placano.remove("Placilo: ");
            cena = textList.at(11);
            cena.remove(" Sum_skupaj:");
            if(opomba == " Opomba: racun_je_bil_spremenjen!!!???? " || opomba == " Opomba: racun_je_bil_odstranjen!!!???? ") {
                continue;
            }
            else if(ui->comboBox_stranke->currentText() == "Vse terjatve" && date_Od_Do >= ui->dateEdit_terjatveOd->date() && date_Od_Do <= ui->dateEdit_terjatveDo->date()) {
                if(neplacani) {
                    AddRootTerjatve(textList, false);
                    continue;
                } else {
                    if(QString::number(placano.toDouble(), 'f', 2) != QString::number(cena.toDouble(), 'f', 2)) {
                        AddRootTerjatve(textList, false);
                        continue;
                    } else {
                        continue;
                    }
                }
            }
            else if(stranka == ui->comboBox_stranke->currentText() && date_Od_Do >= ui->dateEdit_terjatveOd->date() && date_Od_Do <= ui->dateEdit_terjatveDo->date()) {
                if(neplacani) {
                    AddRootTerjatve(textList, false);
                } else {
                    if(QString::number(placano.toDouble(), 'f', 2) != QString::number(cena.toDouble(), 'f', 2)) {
                        AddRootTerjatve(textList, false);
                    } else {
                        continue;
                    }
                }
            }
            else if(ui->comboBox_stranke->currentText() == "Vse terjatve" && date_Od_Do.year() < ui->dateEdit_terjatveOd->date().year()) {
                if(neplacani) {
                    AddRootTerjatve(textList, true);
                    continue;
                } else {
                    if(QString::number(placano.toDouble(), 'f', 2) != QString::number(cena.toDouble(), 'f', 2)) {
                        AddRootTerjatve(textList, true);
                        continue;
                    } else {
                        continue;
                    }
                }
            }
            else if(stranka == ui->comboBox_stranke->currentText() && date_Od_Do.year() < ui->dateEdit_terjatveOd->date().year()) {
                if(neplacani) {
                    AddRootTerjatve(textList, true);
                } else {
                    if(QString::number(placano.toDouble(), 'f', 2) != QString::number(cena.toDouble(), 'f', 2)) {
                        AddRootTerjatve(textList, true);
                    } else {
                        continue;
                    }
                }
            }
        }
    }
    mFile.close();
    ui->treeWidget_terjatve->sortByColumn(0, Qt::AscendingOrder);
    m_totalTerMinObv = (m_totalTerjatve + m_totalTerjatveStaro) - (m_totalObveznosti + m_totalObveznostiStaro);
}

void Terjatve::AddRootObveznosti(QStringList textList, bool staro)
{
    if(!staro) {
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
        itm->setTextAlignment(0, Qt::AlignLeading);
        itm->setText(1, stranka);
        itm->setTextAlignment(0, Qt::AlignCenter);
        itm->setText(2, znesek);
        itm->setText(3, dat_valute);
        itm->setText(4, placilo);
        itm->setText(5, dat_placila);
        itm->setText(6, opomba);
        itm->setText(7, ident);
        if(znesek.remove("€").toDouble() > placilo.remove("€").toDouble()) {
            for(int i(0); i < itm->columnCount(); i++)
                itm->setTextColor(i, QColor("red"));
        }
        else if(znesek.remove("€").toDouble() < placilo.remove("€").toDouble()) {
            for(int i(0); i < itm->columnCount(); i++)
                itm->setBackgroundColor(i, QColor("red"));
        } else {
            for(int i(0); i < itm->columnCount(); i++)
                itm->setTextColor(i, QColor("green"));
        }
        ui->treeWidget_obveznosti->addTopLevelItem(itm);
    } else {
        QStringList list;
        QRegularExpression exp(": ");
        QString znesek = textList.at(5);
        list = znesek.split(exp, QString::SkipEmptyParts);
        znesek = list.at(1);
        znesek.remove('?');
        m_totalObveznostiStaro += znesek.toDouble();
        QString placilo = textList.at(7);
        list = placilo.split(exp, QString::SkipEmptyParts);
        placilo = list.at(1);
        m_totalObveznostiStaro -= placilo.toDouble();
    }
}

void Terjatve::ReadObveznosti(bool neplacani)
{
    ui->treeWidget_obveznosti->clear();
    m_totalObveznosti = 0;
    m_totalObveznostiStaro = 0;
    QFile mFile(m_obveznosti);
    QString mText("");
    QRegularExpression exp("; ");
    QRegularExpression exp_second(": ");
    QStringList textList;
    QStringList tmp;
    QString stranka("");
    QString date("");
    QString opomba("");
    QString placano("");
    QString cena("");
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
            date = textList.at(6);
            date.remove("Dat_valute: ");
            date.remove(-1,1);
            date_Od_Do = date_Od_Do.fromString(date, "d. M. yyyy");
            opomba = textList.at(9);
            placano = textList.at(7);
            placano.remove("Placilo: ");
            cena = textList.at(5);
            cena.remove("Znesek: ");
            if(opomba == "Opombe: terjatev_je_bila_odstranjena!!!???? ") {
                continue;
            }
            else if(ui->comboBox->currentText() == "Vse obveznosti" && date_Od_Do >= ui->dateEdit_obveznostiOd->date() && date_Od_Do <= ui->dateEdit_obveznostiDo->date()) {
                if(neplacani) {
                    AddRootObveznosti(textList, false);
                    continue;
                } else {
                    if(QString::number(placano.toDouble(), 'f', 2) != QString::number(cena.toDouble(), 'f', 2)) {
                        AddRootObveznosti(textList, false);
                        continue;
                    } else {
                        continue;
                    }
                }
            }
            else if(stranka == ui->comboBox->currentText() && date_Od_Do >= ui->dateEdit_obveznostiOd->date() && date_Od_Do <= ui->dateEdit_obveznostiDo->date()) {
                if(neplacani) {
                    AddRootObveznosti(textList, false);
                } else {
                    if(QString::number(placano.toDouble(), 'f', 2) != QString::number(cena.toDouble(), 'f', 2)) {
                        AddRootObveznosti(textList, false);
                    } else {
                        continue;
                    }
                }
            }
            else if(ui->comboBox->currentText() == "Vse obveznosti" && date_Od_Do.year() < ui->dateEdit_obveznostiOd->date().year()) {
                if(neplacani) {
                    AddRootObveznosti(textList, true);
                    continue;
                } else {
                    if(QString::number(placano.toDouble(), 'f', 2) != QString::number(cena.toDouble(), 'f', 2)) {
                        AddRootObveznosti(textList, true);
                        continue;
                    } else {
                        continue;
                    }
                }
            }
            else if(stranka == ui->comboBox->currentText() && date_Od_Do.year() < ui->dateEdit_obveznostiOd->date().year()) {
                if(neplacani) {
                    AddRootObveznosti(textList, true);
                } else {
                    if(QString::number(placano.toDouble(), 'f', 2) != QString::number(cena.toDouble(), 'f', 2)) {
                        AddRootObveznosti(textList, true);
                    } else {
                        continue;
                    }
                }
            }
        }
    }
    mFile.close();
    ui->treeWidget_obveznosti->sortByColumn(3, Qt::DescendingOrder);
    m_totalTerMinObv = (m_totalTerjatve + m_totalTerjatveStaro) - (m_totalObveznosti + m_totalObveznostiStaro);
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
    out.setCodec("UTF-8");
    // prebere celoten dokument in shrani v var
    QString line;
    while(!out.atEnd()){
        line = out.readLine();
        if(line.contains(searchName, Qt::CaseInsensitive) && !(line.contains("terjatev_je_bila_odstranjena!!!????", Qt::CaseInsensitive) || line.contains("racun_je_bil_odstranjen!!!????", Qt::CaseInsensitive) || line.contains("racun_je_bil_spremenjen!!!????", Qt::CaseInsensitive))) {
            // locilo med podatki podjetja v filu ime_podjetja;naslov;ddv;email itd itd
            QRegExp rx("[;]");
            // naredimo listo da posamezne podatke locimo in shranimo kot posamezno kategorijo
            QStringList list;
            list = line.split(rx, QString::SkipEmptyParts);
            // v treeWidget vnesemo vsa podjetja (posamezni podatki iz liste so list.at(?))
            if(ter_obv)
                AddRootTerjatve(list, false);
            else
                AddRootObveznosti(list, false);
        }
    }
    // zapre file
    mFile.close();
}

void Terjatve::SetStatisticParameterTerjatve(int comboIndex, QDate dateOd, QDate dateDo, bool neplacani) {
    ui->comboBox_stranke->setCurrentIndex(comboIndex);
    ui->dateEdit_terjatveOd->setDate(dateOd);
    ui->dateEdit_terjatveDo->setDate(dateDo);
    ui->checkBox_neplacaneTerjatve->setChecked(neplacani);
}

void Terjatve::SetStatisticParameterObveznosti(int comboIndex, QDate dateOd, QDate dateDo, bool neplacani) {
    ui->comboBox->setCurrentIndex(comboIndex);
    ui->dateEdit_obveznostiOd->setDate(dateOd);
    ui->dateEdit_obveznostiDo->setDate(dateDo);
    ui->checkBox_neplacaneObveznosti->setChecked(neplacani);
}

void Terjatve::LabelsUpdate() {
    ui->label_skupajTerjatve->setText("€" + QString::number(m_totalTerjatve + m_totalTerjatveStaro, 'f', 2));
    ui->label_skupajObveznosti->setText("€" + QString::number(m_totalObveznosti + m_totalObveznostiStaro, 'f', 2));
    ui->label_terjatveStaro->setText("Terjatve za leto " + QString::number(ui->dateEdit_terjatveOd->date().year()-1));
    ui->label_terjatveStaroCena->setText("€" + QString::number(m_totalTerjatveStaro, 'f', 2));
    ui->label_obveznostiStaro->setText("Obveznosti za leto " + QString::number(ui->dateEdit_obveznostiOd->date().year()-1));
    ui->label_obveznostiStaroCena->setText("€" + QString::number(m_totalObveznostiStaro, 'f', 2));
    if(m_totalTerMinObv < 0)
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: red;}");
    else
        ui->label_terjatve_obveznosti->setStyleSheet("QLabel {color: green}");
    ui->label_terjatve_obveznosti->setText("€" + QString::number(m_totalTerMinObv, 'f', 2));
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
    ReadTerjatve(true);
    LabelsUpdate();
}

void Terjatve::on_dateEdit_terjatveOd_editingFinished()
{
    ReadTerjatve(true);
    LabelsUpdate();
}

void Terjatve::on_dateEdit_terjatveDo_editingFinished()
{
     ReadTerjatve(true);
     LabelsUpdate();
}

// vnos obveznosti
void Terjatve::on_pushButton_clicked()
{
    VnosObveznosti vnos;
    vnos.setModal(true);
    this->hide();
    QObject::connect(&vnos,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        vnos.exec();
    }
    this->show();
    ui->treeWidget_obveznosti->clear();
    AddItemsToComboBox();
    ReadObveznosti(true);
    LabelsUpdate();
}

void Terjatve::on_comboBox_currentIndexChanged()
{
    if(ui->comboBox->currentText() == "Vse obveznosti")
    {
        ui->lineEdit_iskalnikObveznosti->setEnabled(true);
        ui->lineEdit_iskalnikObveznosti->clear();
    }
    else
    {
        ui->lineEdit_iskalnikObveznosti->setEnabled(false);
        ui->lineEdit_iskalnikObveznosti->clear();
    }
    ReadObveznosti(true);
    LabelsUpdate();
}

void Terjatve::on_dateEdit_obveznostiOd_editingFinished()
{
    ReadObveznosti(true);
    LabelsUpdate();
}

void Terjatve::on_dateEdit_obveznostiDo_editingFinished()
{
    ReadObveznosti(true);
    LabelsUpdate();
}

// vnos nov racun
void Terjatve::on_pushButton_2_clicked()
{
    NovRacun racun;
    racun.setModal(true);
    this->hide();
    QObject::connect(&racun,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        racun.exec();
    }
    this->show();
    AddItemsToComboBox();
    ReadTerjatve(true);
    LabelsUpdate();
}

void Terjatve::on_treeWidget_terjatve_itemDoubleClicked(QTreeWidgetItem *item)
{
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
        in.setCodec("UTF-8");
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
            QString opomba(tmp_list.at(12));
            opomba.remove("Opomba: ");
            opomba.remove(0,1);
            QString cena(tmp_list.at(14));
            cena.remove("Placilo: ");
            cena.remove(0,1);
            QString datum(tmp_list.at(15));
            datum.remove("Dat_placila: ");
            datum.remove(0,1);
            if(opomba == " ") opomba = "";
            if(cena == " ") cena = "";
            QString terObv(tmp_list.at(11));
            terObv.remove("Sum_skupaj: ");
            terObv.remove(" ");
            placilo.setOpombe(opomba, cena, datum, terObv, false);
            placilo.setModal(true);
            this->hide();
            QObject::connect(&placilo,SIGNAL(close_me()),this,SLOT(CloseChild()));
            m_show_child = true;
            while (m_show_child) {
                placilo.exec();
            }
            this->show();
            if(!placilo.m_quit) {
                if(placilo.deleteRacun) {
                    QFile mFile(m_terjatve);
                    if(!mFile.open(QFile::Text | QFile::ReadOnly)) {
                        qDebug() << "Error opening mFile for reading in popravi button";
                        return;
                    }
                    QTextStream in(&mFile);
                    in.setCodec("UTF-8");
                    QString allText = in.readAll();
                    mFile.close();
                    QString produkt_star = text_stRacuna;
                    QString opomba = tmp_list.at(12);
                    QString produkt_nov = text_stRacuna.replace(opomba, " Opomba: racun_je_bil_odstranjen!!!????");
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
                    in << allText;
                    mFile.flush();
                    mFile.close();
                    QString st_racuna = tmp_list.at(3);
                    st_racuna.remove("St_rac: ");
                    st_racuna.remove(0,1);
                    QString stranka = tmp_list.at(7);
                    stranka.remove("Narocnik: ");
                    QRegExp rx("[ ]");
                    QStringList list;
                    list = stranka.split(rx, QString::SkipEmptyParts);
                    if(list.length() == 1)
                        list.append("n.n.");
                    if(list.at(1) == "n.n." || list.at(1) == "d.o.o." || list.at(1) == "doo" || list.at(1) == "D.O.O."  || list.at(1) == "DOO" || list.at(1) == "s.p." || list.at(1) == "sp" || list.at(1) == "s.p" || list.at(1) == "S.P." || list.at(1) == "S.P" || list.at(1) == "SP")
                        stranka = list.at(0);
                    else
                        stranka = list.at(0) + "_" + list.at(1);
                    if (QFile::exists(m_fileShrani + "/RACUN_IN_DOBAVNICA_" + stranka + "_" + st_racuna + ".pdf")) {
                          QFile::copy(m_fileShrani + "/RACUN_IN_DOBAVNICA_" + stranka + "_" + st_racuna + ".pdf", m_fileShrani + "/IZBRISANI/RACUN_IN_DOBAVNICA_" + stranka + "_" + st_racuna + "_IZBRISAN.pdf");
                          QFile::remove(m_fileShrani + "/RACUN_IN_DOBAVNICA_" + stranka + "_" + st_racuna + ".pdf");
                    }
                } else {
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
        }
    }
    qDebug() << "error";
    AddItemsToComboBox();
    ReadTerjatve(true);
    LabelsUpdate();
}

void Terjatve::on_treeWidget_obveznosti_itemDoubleClicked(QTreeWidgetItem *item)
{
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
        in.setCodec("UTF-8");
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
            QString terObv(tmp_list.at(5));
            terObv.remove("Znesek: ");
            terObv.remove(" ");
            placilo.setOpombe(opomba, cena, datum, terObv, true);
            this->hide();
            QObject::connect(&placilo,SIGNAL(close_me()),this,SLOT(CloseChild()));
            m_show_child = true;
            while (m_show_child) {
                placilo.exec();
            }
            this->show();
            if(!placilo.m_quit) {
                if(placilo.deleteRacun) {
                    QFile mFile(m_obveznosti);
                    if(!mFile.open(QFile::Text | QFile::ReadOnly)) {
                        qDebug() << "Error opening mFile for reading in popravi button";
                        return;
                    }
                    QTextStream in(&mFile);
                    in.setCodec("UTF-8");
                    QString allText = in.readAll();
                    mFile.close();
                    QString produkt_star = text_obveznost;
                    QString opomba = tmp_list.at(9);
                    QString produkt_nov = text_obveznost.replace(opomba, " Opombe: terjatev_je_bila_odstranjena!!!????");
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
                    in << allText;
                    mFile.flush();
                    mFile.close();
                } else {
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
        }
    }
    AddItemsToComboBox();
    ReadObveznosti(true);
    LabelsUpdate();
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

void Terjatve::on_comboBox_stranke_currentIndexChanged(int index) {
    qDebug() << index;
    ReadTerjatve(true);
    LabelsUpdate();
}

void Terjatve::on_comboBox_currentIndexChanged(int index) {
    qDebug() << index;
    ReadObveznosti(true);
    LabelsUpdate();
}

void Terjatve::on_dateEdit_terjatveOd_dateChanged(const QDate &date) {
    qDebug() << date;
    ReadTerjatve(true);
    LabelsUpdate();
}

void Terjatve::on_dateEdit_terjatveDo_dateChanged(const QDate &date) {
    qDebug() << date;
    ReadTerjatve(true);
    LabelsUpdate();
}

void Terjatve::on_dateEdit_obveznostiOd_dateChanged(const QDate &date) {
    qDebug() << date;
    ReadObveznosti(true);
    LabelsUpdate();
}

void Terjatve::on_dateEdit_obveznostiDo_dateChanged(const QDate &date) {
    qDebug() << date;
    ReadObveznosti(true);
    LabelsUpdate();
}

void Terjatve::on_checkBox_neplacaneTerjatve_stateChanged(int arg1) {
    qDebug() << arg1;
    if(ui->checkBox_neplacaneTerjatve->isChecked()) {
        ReadTerjatve(false);
        LabelsUpdate();
    } else {
        ReadTerjatve(true);
        LabelsUpdate();
    }
}

void Terjatve::on_checkBox_neplacaneObveznosti_stateChanged(int arg1) {
    qDebug() << arg1;
    if(ui->checkBox_neplacaneObveznosti->isChecked()) {
        ReadObveznosti(false);
        LabelsUpdate();
    } else {
        ReadObveznosti(true);
        LabelsUpdate();
    }
}

void Terjatve::on_pushButton_isciTerjatve_clicked() {
    if(ui->checkBox_neplacaneTerjatve->isChecked()) {
        ReadTerjatve(false);
        LabelsUpdate();
    } else {
        ReadTerjatve(true);
        LabelsUpdate();
    }
}

void Terjatve::on_pushButton_isciObveznosti_clicked() {
    if(ui->checkBox_neplacaneObveznosti->isChecked()) {
        ReadObveznosti(false);
        LabelsUpdate();
    } else {
        ReadObveznosti(true);
        LabelsUpdate();
    }
}
