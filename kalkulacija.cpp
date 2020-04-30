#include "kalkulacija.h"
#include "ui_kalkulacija.h"

Kalkulacija::Kalkulacija(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kalkulacija), m_currentDir(QDir::currentPath()), m_companyFile(m_currentDir + "/company_file.txt"), m_material(m_currentDir + "/material.txt"),
    m_delavniProces(m_currentDir + "/delavni_proces.txt"), m_arhivProdukti(m_currentDir + "/arhiv_produkti.txt"), m_cNaziv(""), m_materialCena(0.0), m_deloCena(0.0),
    m_count(true), m_show_child(false)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Kalkulacija");
    this->setWindowFlags(Qt::Window);
    this->showMaximized();
    ui->label_cenaDela->setText("€0.00");
    ui->label_cenaMateriala->setText("€0.00");
    ui->label_cenaMaterialaDela->setText("€0.00");
    ui->treeWidget_material->setRootIsDecorated(false);
    ui->treeWidget_materialProdukta->setRootIsDecorated(false);
    ui->treeWidget_delo->setRootIsDecorated(false);
    ui->treeWidget_deloProdukta->setRootIsDecorated(false);
    ui->treeWidget_material->setColumnCount(5);
    ui->treeWidget_material->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->treeWidget_material->header()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->treeWidget_material->header()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->treeWidget_material->header()->setSectionResizeMode(3, QHeaderView::Interactive);
    ui->treeWidget_material->header()->setSectionResizeMode(4, QHeaderView::Interactive);
    ui->treeWidget_material->header()->setStretchLastSection(false);
    ui->treeWidget_materialProdukta->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->treeWidget_materialProdukta->header()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->treeWidget_materialProdukta->header()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->treeWidget_materialProdukta->header()->setStretchLastSection(false);
    ui->treeWidget_delo->setColumnCount(2);
    ui->treeWidget_delo->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->treeWidget_delo->header()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->treeWidget_delo->header()->setStretchLastSection(false);
    ui->treeWidget_deloProdukta->setColumnCount(3);
    ui->treeWidget_deloProdukta->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->treeWidget_deloProdukta->header()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->treeWidget_deloProdukta->header()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->treeWidget_deloProdukta->header()->setStretchLastSection(false);
    ui->pushButton_isci->setVisible(false);
    ui->radioButton_meter->setChecked(true);
    ui->lineEdit_dodatekNaDelo->setText("50");
    QRegularExpression regex("^[a-zA-Z0-9,@. -/&#čšžŠČŽ()+=]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_nazivProdukta->setValidator(validator);
    ui->lineEdit_operacija->setValidator(validator);
    ui->lineEdit_nazivMateriala->setValidator(validator);
    QRegularExpression regenum("^[0123456789/]*$");
    QValidator *validatornum = new QRegularExpressionValidator(regenum, this);
    ui->lineEdit_ID->setValidator(validatornum);
    ui->lineEdit_kolicinaMateriala->setValidator(validatornum);
    ui->lineEdit_ID->setMaxLength(7);
    QRegularExpression regenumDot("^[0123456789.,]*$");
    QValidator *validatornumDot = new QRegularExpressionValidator(regenumDot, this);
    ui->lineEdit_cenaProdukta->setValidator(validatornumDot);
    ui->lineEdit_cenaMateriala->setValidator(validatornumDot);
    ui->lineEdit_cenaOperacije->setValidator(validatornumDot);
    QIntValidator *intvalidator = new QIntValidator(0, 1000, this);
    ui->lineEdit_dodatekNaDelo->setValidator(intvalidator);
    AddItemsToCombo();
    Read(m_material, ui->treeWidget_material);
    Read(m_delavniProces, ui->treeWidget_delo);
    QScrollArea *scrollArea = new QScrollArea(this);
    QGroupBox *groupBox = new QGroupBox(scrollArea);
    groupBox->setLayout(ui->gridLayout);
    scrollArea->setWidget(groupBox);
    scrollArea->setWidgetResizable(true);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(scrollArea);
    setLayout(layout);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

Kalkulacija::~Kalkulacija() {
    delete ui;
}

void Kalkulacija::CloseChild() {
    m_show_child = false;
}

void Kalkulacija::closeEvent(QCloseEvent *) {
    emit close_me();
}

// vstavi podjetja v combo box
void Kalkulacija::AddItemsToCombo() {
    ui->comboBox_stranka->clear();
    QFile mFile(m_companyFile);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error pri vstavljanju podjetji v combobox v kalkulaciji";
        return;
    }
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QString mText("");
    QRegExp rx("[;]");
    QStringList list;
    bool first(true);
    while(!in.atEnd()) {
        mText = in.readLine();
        list = mText.split(rx, QString::SkipEmptyParts);
        if(first) {
            m_cNaziv = list.at(1);
            first = false;
        }
        if(mText == "") {
            continue;
        } else {
            ui->comboBox_stranka->addItem(list.at(1));
        }
    }
    mFile.close();
}

// vstavi material v treewidgete
void Kalkulacija::AddRoot(QString id, QString naziv, QString cena, QString kolicina, QString vrstaKolicine, QTreeWidget *tree) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(tree);
    itm->setText(0, id.toUtf8());
    itm->setTextAlignment(0, Qt::AlignHCenter);
    itm->setText(1, naziv.toUtf8());
    itm->setTextAlignment(1, Qt::AlignHCenter);
    itm->setText(2, "€" + cena.toUtf8());
    itm->setTextAlignment(2, Qt::AlignHCenter);
    itm->setText(3, kolicina.toUtf8());
    itm->setTextAlignment(3, Qt::AlignHCenter);
    itm->setText(4, vrstaKolicine.toUtf8());
    itm->setTextAlignment(4, Qt::AlignHCenter);
    ui->treeWidget_material->addTopLevelItem(itm);
    QColor color(210,210,210);
    QColor wcolor(250,250,250);
    if(ui->treeWidget_material->topLevelItemCount() % 2 == 0) m_count = true;
    else m_count = false;
    if(m_count) {
        itm->setBackground(0,color);
        itm->setBackground(1,color);
        itm->setBackground(2,color);
        itm->setBackground(3,color);
        itm->setBackground(4,color);
    } else {
        itm->setBackground(0,wcolor);
        itm->setBackground(1,wcolor);
        itm->setBackground(2,wcolor);
        itm->setBackground(3,wcolor);
        itm->setBackground(4,wcolor);
    }
}

// preberi txt datoteke
void Kalkulacija::Read(QString file, QTreeWidget *tree) {
    tree->clear();
    QFile mFile(file);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in Read() function in dodajpodjetje.cpp";
        return;
    }
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QString mText("");
    QRegExp rx("[;]");
    QStringList list;
    while(!in.atEnd()) {
        mText = in.readLine();
        list = mText.split(rx, QString::SkipEmptyParts);
        if(mText == "") {
            continue;
        } else {
            AddRoot(list.at(0), list.at(1), list.at(2), list.at(3), list.at(4), tree);
        }
    }
    mFile.close();
}

// iskalnik
void Kalkulacija::Search(QString searchName, QString file, QTreeWidget *tree) {
    QFile mFile(file);
    if(!mFile.open(QFile::Text | QFile::ReadOnly)) {
        qDebug() << "Error pri odpiranju datoteke v Kalkulacija::Search";
        return;
    }
    QTextStream out(&mFile);
    QString line("");
    while(!out.atEnd()) {
        line = out.readLine();
        if(line.contains(searchName, Qt::CaseInsensitive)) {
            QRegExp rx("[;]");
            QStringList list;
            list = line.split(rx, QString::SkipEmptyParts);
            AddRoot(list.at(0), list.at(1), list.at(2), list.at(3), list.at(4), tree);
        }
    }
    mFile.close();
}

// iskalnik v materialu
void Kalkulacija::on_lineEdit_iskalnik_textChanged(const QString &searchName) {
    if(searchName == "")
        Read(m_material, ui->treeWidget_material);
    else {
        ui->treeWidget_material->clear();
        Search(searchName, m_material, ui->treeWidget_material);
    }
}

// dodajanje materiala
void Kalkulacija::on_treeWidget_material_itemDoubleClicked() {
    if(ui->checkBox_popravekMateriala->isChecked()) {
        ui->lineEdit_IDMateriala->setText(ui->treeWidget_material->currentItem()->text(0));
        ui->lineEdit_nazivMateriala->setText(ui->treeWidget_material->currentItem()->text(1));
        ui->lineEdit_cenaMateriala->setText(ui->treeWidget_material->currentItem()->text(2).remove("€").remove(' ').replace(',','.'));
        ui->lineEdit_kolicinaMateriala->setText(ui->treeWidget_material->currentItem()->text(3).remove(' '));
        if(ui->treeWidget_material->currentItem()->text(4).remove(' ') == "meter")
            ui->radioButton_meter->setChecked(true);
        else
            ui->radioButton_kos->setChecked(true);
    } else {
        bool dolzina(false);
        if(ui->treeWidget_material->currentItem()->text(4) == "meter")
            dolzina = true;
        numOfItemsKalk numItems;
        numItems.LineEditorState(dolzina);
        numItems.setModal(true);
        this->hide();
        QObject::connect(&numItems,SIGNAL(close_me()),this,SLOT(CloseChild()));
        m_show_child = true;
        while (m_show_child) {
            numItems.exec();
        }
        this->show();
        QString numMaterial = numItems.m_kolicina;
        QString kalo = numItems.m_kalo;
        bool insertCorrect(true);
        if(numMaterial == "0")
            insertCorrect = false;
        while(insertCorrect) {
            QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_materialProdukta);
            itm->setText(0, ui->treeWidget_material->currentItem()->text(1));
            itm->setText(1, numMaterial);
            itm->setTextAlignment(1, Qt::AlignHCenter);
            QString price = QString::number((ui->treeWidget_material->currentItem()->text(2).remove("€").toDouble() / ui->treeWidget_material->currentItem()->text(3).toDouble()), 'f', 4);
            itm->setText(2, "€" + QString::number((price.toDouble() * numMaterial.toDouble()), 'f', 4));
            itm->setTextAlignment(2, Qt::AlignHCenter);
            itm->setText(3, kalo);
            itm->setTextAlignment(3, Qt::AlignHCenter);
            QColor color(210,210,210);
            QColor wcolor(250,250,250);
            if(ui->treeWidget_materialProdukta->topLevelItemCount() % 2 == 0) m_count = true;
            else m_count = false;
            if(m_count) {
                itm->setBackground(0,color);
                itm->setBackground(1,color);
                itm->setBackground(2,color);
                itm->setBackground(3,color);
            } else {
                itm->setBackground(0,wcolor);
                itm->setBackground(1,wcolor);
                itm->setBackground(2,wcolor);
                itm->setBackground(3,wcolor);
            }
            m_materialCena += (price.toDouble() * numMaterial.toDouble());
            if(m_materialCena <= 0) m_materialCena = 0;
            ui->label_cenaMateriala->setText("€" + QString::number(m_materialCena, 'f', 4));
            ui->treeWidget_materialProdukta->setUniformRowHeights(true);
            ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
            ui->lineEdit_cenaProdukta->setText(QString::number(ui->label_cenaMaterialaDela->text().remove("€").replace(',','.').toDouble() / 1000, 'f', 4));
            insertCorrect = false;
        }
    }
}

// odstrani material
void Kalkulacija::on_treeWidget_materialProdukta_itemDoubleClicked() {
    QString price = ui->treeWidget_materialProdukta->currentItem()->text(2);
    m_materialCena -= price.remove("€").toDouble();
    ui->label_cenaMateriala->setText("€" + QString::number(m_materialCena, 'f', 4));
    if(m_materialCena <= 0) m_materialCena = 0;
    delete ui->treeWidget_materialProdukta->takeTopLevelItem(ui->treeWidget_materialProdukta->currentIndex().row());
    ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
    ui->lineEdit_cenaProdukta->setText(ui->label_cenaMaterialaDela->text().remove("€").replace(',','.'));
}

// doda delo
void Kalkulacija::on_treeWidget_delo_itemDoubleClicked() {
    if(ui->checkBox_popraviOperacijo->isChecked()) {
        ui->lineEdit_operacija->setText(ui->treeWidget_delo->currentItem()->text(0));
        ui->lineEdit_cenaOperacije->setText(ui->treeWidget_delo->currentItem()->text(1).remove("€").remove(' ').replace(',','.'));
    } else {
        NumOfHours numhours;
        numhours.setModal(true);
        this->hide();
        QObject::connect(&numhours,SIGNAL(close_me()),this,SLOT(CloseChild()));
        m_show_child = true;
        while (m_show_child) {
            numhours.exec();
        }
        this->show();
        Kalkulacija::show();
        QString numHours = numhours.m_numOfHours;
        bool insertCorrect(true);
        if(numHours == "0")
            insertCorrect = false;
        while(insertCorrect) {
            QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_deloProdukta);
            itm->setText(0, ui->treeWidget_delo->currentItem()->text(0));
            itm->setText(1, numHours);
            itm->setTextAlignment(1, Qt::AlignHCenter);
            QString price = ui->treeWidget_delo->currentItem()->text(1);
            itm->setText(2, "€" + QString::number((price.remove("€").toDouble() * numHours.toDouble()), 'f', 4));
            itm->setTextAlignment(2, Qt::AlignHCenter);
            QColor color(210,210,210);
            QColor wcolor(250,250,250);
            if(ui->treeWidget_deloProdukta->topLevelItemCount() % 2 == 0) m_count = true;
            else m_count = false;
            if(m_count) {
                itm->setBackground(0,color);
                itm->setBackground(1,color);
                itm->setBackground(2,color);
            } else {
                itm->setBackground(0,wcolor);
                itm->setBackground(1,wcolor);
                itm->setBackground(2,wcolor);
            }
            m_deloCena += (price.remove("€").toDouble() * numHours.toDouble());
            if(m_deloCena <= 0) m_deloCena = 0;
            ui->label_cenaDela->setText("€" + QString::number(m_deloCena + ((m_deloCena * ui->lineEdit_dodatekNaDelo->text().toDouble())/100), 'f', 4));
            ui->treeWidget_deloProdukta->setUniformRowHeights(true);
            ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
            ui->lineEdit_cenaProdukta->setText(QString::number(ui->label_cenaMaterialaDela->text().remove("€").replace(',','.').toDouble() / 1000, 'f', 4));
            insertCorrect = false;
        }
    }
}

// odstrani delo
void Kalkulacija::on_treeWidget_deloProdukta_itemDoubleClicked() {
    QString price = ui->treeWidget_deloProdukta->currentItem()->text(2);
    m_deloCena -= price.remove("€").toDouble();
    ui->label_cenaDela->setText("€" + QString::number(m_deloCena / 1000, 'f', 4));
    if(m_deloCena <= 0)
        m_deloCena = 0;
    delete ui->treeWidget_deloProdukta->takeTopLevelItem(ui->treeWidget_deloProdukta->currentIndex().row());
    ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
    ui->lineEdit_cenaProdukta->setText(ui->label_cenaMaterialaDela->text().remove("€").replace(',','.'));
}

// doda material v datoteko
void Kalkulacija::on_pushButton_dodajMaterial_clicked()
{
    if(ui->checkBox_popravekMateriala->isChecked()) {
        QFile fileName(m_material);
        if(!fileName.open(QFile::Text | QFile::ReadOnly)) {
            qDebug() << "Error pri odpiranju datoteke material.txt v Kalkulaciji v if()";
            return;
        }
        QTextStream out(&fileName);
        out.setCodec("UTF-8");
        QString allText = out.readAll();
        fileName.close();
        QString idMateriala = ui->lineEdit_IDMateriala->text();
        while(idMateriala.at(-1) == ' ') idMateriala.remove(-1,1);
        while(idMateriala.at(0) == ' ') idMateriala.remove(0,1);
        if(idMateriala == "") idMateriala = "ni podatka";
        QString nazivProdukta = ui->lineEdit_nazivMateriala->text();
        nazivProdukta.replace(0,1,nazivProdukta.at(0).toUpper());
        while(nazivProdukta.at(nazivProdukta.length() - 1) == ' ') {
            nazivProdukta.remove(-1, 1);
        }
        if(nazivProdukta == "") nazivProdukta = "ni podatka";
        QString cenaProdukta = ui->lineEdit_cenaMateriala->text().replace(',','.');
        if(cenaProdukta == "") cenaProdukta = "0.00";
        QString kolicinaProdukta = ui->lineEdit_kolicinaMateriala->text();
        if(kolicinaProdukta == "") kolicinaProdukta = "0";
        QString vrstaMateriala("meter");
        if(ui->radioButton_kos->isChecked())
            vrstaMateriala = "kos";
        QRegularExpression trenutniMaterial(ui->treeWidget_material->currentItem()->text(0) + ";" + ui->treeWidget_material->currentItem()->text(1) + ";" + ui->treeWidget_material->currentItem()->text(2).remove("€") + ";" + ui->treeWidget_material->currentItem()->text(3) + ";" + ui->treeWidget_material->currentItem()->text(4) + ";", QRegularExpression::CaseInsensitiveOption);
        QString popravljenMaterial = idMateriala + ";" + nazivProdukta + ";" + cenaProdukta + ";" + kolicinaProdukta + ";" + vrstaMateriala + ";";
        allText.replace(trenutniMaterial, popravljenMaterial);
        if(!fileName.open(QFile::WriteOnly | QFile::Truncate)) {
            qDebug() << "Error pri odpiranju datoteke material.txt v Kalkulaciji v if() pri zamenjavi texta";
            return;
        }
        fileName.flush();
        fileName.close();
        if(!fileName.open(QFile::WriteOnly | QFile::Text))
        {
            qDebug() << "Error pri odpiranju datoteke material.txt v Kalkulaciji v if() pri zamenjavi texta";
            return;
        }
        out << allText;
        fileName.flush();
        fileName.close();
        Read(m_material, ui->treeWidget_material);
        ui->lineEdit_IDMateriala->clear();
        ui->lineEdit_cenaMateriala->clear();
        ui->lineEdit_nazivMateriala->clear();
        ui->lineEdit_kolicinaMateriala->clear();
    } else {
        QFile fileName(m_material);
        if(!fileName.open(QFile::WriteOnly | QFile::Append)) {
            qDebug() << "Error pri odpiranju datoteke material.txt v Kalkulaciji v else()";
            return;
        }
        QTextStream out(&fileName);
        out.setCodec("UTF-8");
        QString idMateriala = ui->lineEdit_IDMateriala->text();
        while(idMateriala.at(-1) == ' ') idMateriala.remove(-1,1);
        while(idMateriala.at(0) == ' ') idMateriala.remove(0,1);
        if(idMateriala == "") idMateriala = "ni podatka";
        QString nazivProdukta = ui->lineEdit_nazivMateriala->text();
        nazivProdukta.replace(0,1,nazivProdukta.at(0).toUpper());
        while(nazivProdukta.at(nazivProdukta.length() - 1) == ' ') {
            nazivProdukta.remove(-1, 1);
        }
        if(nazivProdukta == "") nazivProdukta = "ni podatka";
        QString cenaProdukta = ui->lineEdit_cenaMateriala->text().replace(',','.');
        if(cenaProdukta == "") cenaProdukta = "0.00";
        QString kolicinaProdukta = ui->lineEdit_kolicinaMateriala->text();
        if(kolicinaProdukta == "") kolicinaProdukta = "0";
        QString vrstaMateriala("meter");
        if(ui->radioButton_kos->isChecked())
            vrstaMateriala = "kos";
        out << idMateriala + ";" + nazivProdukta << ";" << cenaProdukta << ";" << kolicinaProdukta << ";" << vrstaMateriala << ";\n";
        fileName.flush();
        fileName.close();
        Read(m_material, ui->treeWidget_material);
        ui->lineEdit_IDMateriala->clear();
        ui->lineEdit_cenaMateriala->clear();
        ui->lineEdit_nazivMateriala->clear();
        ui->lineEdit_kolicinaMateriala->clear();
    }
}

// doda operacijo v datoteko
void Kalkulacija::on_pushButton_dodajOperacijo_clicked()
{
    if(ui->checkBox_popraviOperacijo->isChecked()) {
        QFile fileName(m_delavniProces);
        if(!fileName.open(QFile::Text | QFile::ReadOnly)) {
            qDebug() << "Error pri odpiranju datoteke delavni_proces.txt v Kalkulaciji v if()";
            return;
        }
        QTextStream out(&fileName);
        out.setCodec("UTF-8");
        QString allText = out.readAll();
        fileName.close();
        QString operacija = ui->lineEdit_operacija->text();
        operacija.replace(0,1,operacija.at(0).toUpper());
        while(operacija.at(operacija.length() - 1) == ' ') {
            operacija.remove(-1, 1);
        }
        if(operacija == "") operacija = "ni podatka";
        QString cenaOperacije = ui->lineEdit_cenaOperacije->text().replace(',','.').remove("€");
        if(cenaOperacije == "") cenaOperacije = "0.00";
        QRegularExpression trenutnaOperacija(ui->treeWidget_delo->currentItem()->text(0) + ";" + ui->treeWidget_delo->currentItem()->text(1).remove("€") + ";" + ui->treeWidget_delo->currentItem()->text(2).remove("€") + ";" + ui->treeWidget_delo->currentItem()->text(3) + ";" + ui->treeWidget_delo->currentItem()->text(4) + ";", QRegularExpression::CaseInsensitiveOption);
        QString popravljenaOperacija = operacija + ";" + cenaOperacije.remove("€") + ";" + " ;" + " ;" + " ;";
        allText.replace(trenutnaOperacija, popravljenaOperacija);
        if(!fileName.open(QFile::WriteOnly | QFile::Truncate)) {
            qDebug() << "Error pri odpiranju datoteke delavni_proces.txt v Kalkulaciji v if() pri zamenjavi texta";
            return;
        }
        fileName.flush();
        fileName.close();
        if(!fileName.open(QFile::WriteOnly | QFile::Text))
        {
            qDebug() << "Error pri odpiranju datoteke delavni_proces.txt v Kalkulaciji v if() pri zamenjavi texta";
            return;
        }
        out << allText;
        fileName.flush();
        fileName.close();
        Read(m_delavniProces, ui->treeWidget_delo);
        ui->lineEdit_operacija->clear();
        ui->lineEdit_cenaOperacije->clear();
    } else {
        QFile fileName(m_delavniProces);
        if(!fileName.open(QFile::WriteOnly | QFile::Append)) {
            qDebug() << "Error pri odpiranju datoteke delavni_proces.txt v Kalkulaciji v else()";
            return;
        }
        QTextStream out(&fileName);
        out.setCodec("UTF-8");
        QString operacija = ui->lineEdit_operacija->text();
        operacija.replace(0,1,operacija.at(0).toUpper());
        while(operacija.at(operacija.length() - 1) == ' ') {
            operacija.remove(-1, 1);
        }
        if(operacija == "") operacija = "ni podatka";
        QString cenaOperacije = ui->lineEdit_cenaOperacije->text().replace(',','.').remove("€");
        if(cenaOperacije == "") cenaOperacije = "0.00";
        out << operacija << ";" << cenaOperacije << ";" <<" ;" << " ;" << " ;\n";
        fileName.flush();
        fileName.close();
        Read(m_delavniProces, ui->treeWidget_delo);
        ui->lineEdit_operacija->clear();
        ui->lineEdit_cenaOperacije->clear();
    }
}

// pobrise lineedite za operacijo
void Kalkulacija::on_checkBox_popraviOperacijo_stateChanged() {
    ui->lineEdit_operacija->clear();
    ui->lineEdit_cenaOperacije->clear();
}

// pobrise lineedite za material
void Kalkulacija::on_checkBox_popravekMateriala_stateChanged() {
    ui->lineEdit_IDMateriala->clear();
    ui->lineEdit_cenaMateriala->clear();
    ui->lineEdit_nazivMateriala->clear();
    ui->lineEdit_kolicinaMateriala->clear();
}

// arhiv
void Kalkulacija::Arhiv(QString arhiv_file, QString stream) {
    QFile mFile(arhiv_file);
    if(!mFile.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "Error pri odpiranju datoteke v Kalkulaciji v Arhiv()";
        return;
    }
    QTextStream out(&mFile);
    out.setCodec("UTF-8");
    out << stream << "\n";
    mFile.flush();
    mFile.close();
}

// dodajanje produkta k stranki
void Kalkulacija::on_pushButton_vnosProdukta_clicked() {
    int izbPod = ui->comboBox_stranka->currentIndex() + 1;
    QString podjetje = QString::number(izbPod) + ".txt";
    QFile fileName(podjetje);
    if(!fileName.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "Error odpiranje datoteke pri vnosu produkta Kalkulacija::on_pushButton_vnosProdukta_clicked()";
        return;
    }
    QTextStream out(&fileName);
    out.setCodec("UTF-8");
    QString id = ui->lineEdit_ID->text();
    if(id == "") id = "/";
    QString naziv = ui->lineEdit_nazivProdukta->text();
    naziv.replace(0,1,naziv.at(0).toUpper());
    while(naziv.at(naziv.length() - 1) == ' ') {
        naziv.remove(-1, 1);
    }
    if(naziv == "") naziv = "ni podatka";
    QString cena = ui->lineEdit_cenaProdukta->text().replace(',','.');
    if(cena == "") cena = "0";
    out << id << ";" << naziv << ";" << cena << ";" << "\n";
    fileName.flush();
    fileName.close();
    QDateTime date = QDateTime::currentDateTime();
    QString dodano = "Dodano";
    QString arhiv = dodano + " ; " + date.toString("dd.MM.yyyy ; hh:mm:ss.zzz") + " ; " + id + " ; " + naziv + " ; " + cena + " ; " + "Rajh" + " !?!";
    Arhiv(m_arhivProdukti, arhiv);
    ui->lineEdit_ID->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->checkBox_popravekMateriala->setCheckState(Qt::CheckState(Qt::Unchecked));
    ui->checkBox_popraviOperacijo->setCheckState(Qt::CheckState(Qt::Unchecked));
    ui->treeWidget_materialProdukta->clear();
    ui->treeWidget_deloProdukta->clear();
    ui->label_cenaDela->setText("€0.00");
    ui->label_cenaMateriala->setText("€0.00");
    ui->label_cenaMaterialaDela->setText("€0.00");
    ui->lineEdit_dodatekNaDelo->setText("50");
    ui->lineEdit_cenaProdukta->setText("0.0");
    m_deloCena = 0.0;
    m_materialCena = 0.0;
    close();
}

void Kalkulacija::on_lineEdit_nazivProdukta_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_nazivProdukta->backspace();
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == '.') {
        ui->lineEdit_nazivProdukta->backspace();
        ui->lineEdit_nazivProdukta->backspace();
        ui->lineEdit_nazivProdukta->insert(".");
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ',') {
        ui->lineEdit_nazivProdukta->backspace();
        ui->lineEdit_nazivProdukta->backspace();
        ui->lineEdit_nazivProdukta->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_nazivProdukta->backspace();
    }
}

void Kalkulacija::on_lineEdit_nazivMateriala_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_nazivMateriala->backspace();
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == '.') {
        ui->lineEdit_nazivMateriala->backspace();
        ui->lineEdit_nazivMateriala->backspace();
        ui->lineEdit_nazivMateriala->insert(".");
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ',') {
        ui->lineEdit_nazivMateriala->backspace();
        ui->lineEdit_nazivMateriala->backspace();
        ui->lineEdit_nazivMateriala->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_nazivMateriala->backspace();
    }
}

void Kalkulacija::on_lineEdit_cenaMateriala_textChanged(const QString &arg1)
{
    if(arg1.length() == 1 && (arg1.at(arg1.length()-1) == '.' || arg1.at(arg1.length()-1) == ',')) {
        ui->lineEdit_cenaMateriala->backspace();
    }
    if(arg1.contains("..") || arg1.contains(",,") || arg1.contains(",.") || arg1.contains(".,")) {
        ui->lineEdit_cenaMateriala->backspace();
    }
}

void Kalkulacija::on_lineEdit_operacija_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_operacija->backspace();
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == '.') {
        ui->lineEdit_operacija->backspace();
        ui->lineEdit_operacija->backspace();
        ui->lineEdit_operacija->insert(".");
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ',') {
        ui->lineEdit_operacija->backspace();
        ui->lineEdit_operacija->backspace();
        ui->lineEdit_operacija->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_operacija->backspace();
    }
}

void Kalkulacija::on_lineEdit_cenaOperacije_textChanged(const QString &arg1)
{
    if(arg1.length() == 1 && (arg1.at(arg1.length()-1) == '.' || arg1.at(arg1.length()-1) == ',')) {
        ui->lineEdit_cenaOperacije->backspace();
    }
    if(arg1.contains("..") || arg1.contains(",,") || arg1.contains(",.") || arg1.contains(".,")) {
        ui->lineEdit_cenaOperacije->backspace();
    }
}

void Kalkulacija::on_lineEdit_cenaProdukta_textChanged(const QString &arg1)
{
    if(arg1.length() == 1 && (arg1.at(arg1.length()-1) == '.' || arg1.at(arg1.length()-1) == ',')) {
        ui->lineEdit_cenaProdukta->backspace();
    }
    if(arg1.contains("..") || arg1.contains(",,") || arg1.contains(",.") || arg1.contains(".,")) {
        ui->lineEdit_cenaProdukta->backspace();
    }
}

void Kalkulacija::on_lineEdit_dodatekNaDelo_textChanged(const QString &arg1) {
    ui->label_cenaDela->setText("€" + QString::number((m_deloCena + ((m_deloCena * arg1.toDouble())/100)), 'f', 4));
    ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
    ui->lineEdit_cenaProdukta->setText(QString::number(ui->label_cenaMaterialaDela->text().remove("€").replace(',','.').toDouble() / 1000, 'f', 4));
}
