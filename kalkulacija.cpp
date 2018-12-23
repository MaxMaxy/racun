#include "kalkulacija.h"
#include "ui_kalkulacija.h"

Kalkulacija::Kalkulacija(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kalkulacija), m_currentDir(QDir::currentPath()), m_companyFile(m_currentDir + "/company_file.txt"), m_material(m_currentDir + "/material.txt"), m_delavniProces(m_currentDir + "/delavni_proces.txt"), m_cNaziv(""), m_materialCena(0.0), m_deloCena(0.0), m_count(true)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Kalkulacija");
    this->setWindowFlags(Qt::Window);
    ui->label_cenaDela->setText("€0.00");
    ui->label_cenaMateriala->setText("€0.00");
    ui->label_cenaMaterialaDela->setText("€0.00");
    ui->lineEdit_kalo->setText("5");
    ui->treeWidget_material->setRootIsDecorated(false);
    ui->treeWidget_materialProdukta->setRootIsDecorated(false);
    ui->treeWidget_delo->setRootIsDecorated(false);
    ui->treeWidget_deloProdukta->setRootIsDecorated(false);
    ui->treeWidget_material->setColumnCount(3);
    ui->treeWidget_material->setColumnWidth(0,250);
    ui->treeWidget_material->setColumnWidth(1,60);
    ui->treeWidget_materialProdukta->setColumnCount(3);
    ui->treeWidget_materialProdukta->setColumnWidth(0,250);
    ui->treeWidget_materialProdukta->setColumnWidth(1,60);
    ui->treeWidget_delo->setColumnCount(2);
    ui->treeWidget_delo->setColumnWidth(0,380);
    ui->treeWidget_deloProdukta->setColumnCount(3);
    ui->treeWidget_deloProdukta->setColumnWidth(0,250);
    ui->treeWidget_deloProdukta->setColumnWidth(1,60);
    ui->pushButton_isci->setVisible(false);
    QRegularExpression regex("^[a-zA-Z0-9,@. -/&#čšžŠČŽ]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_nazivProdukta->setValidator(validator);
    ui->lineEdit_operacija->setValidator(validator);
    ui->lineEdit_nazivMateriala->setValidator(validator);
    QRegularExpression regenum("^[0123456789]*$");
    QValidator *validatornum = new QRegularExpressionValidator(regenum, this);
    ui->lineEdit_ID->setValidator(validatornum);
    ui->lineEdit_kalo->setValidator(validatornum);
    ui->lineEdit_kolicinaMateriala->setValidator(validatornum);
    ui->lineEdit_ID->setMaxLength(7);
    QRegularExpression regenumDot("^[0123456789.]*$");
    QValidator *validatornumDot = new QRegularExpressionValidator(regenumDot, this);
    ui->lineEdit_cenaProdukta->setValidator(validatornumDot);
    ui->lineEdit_cenaMateriala->setValidator(validatornumDot);
    ui->lineEdit_cenaOperacije->setValidator(validatornumDot);
    AddItemsToCombo();
    Read(m_material, ui->treeWidget_material);
    Read(m_delavniProces, ui->treeWidget_delo);
}

Kalkulacija::~Kalkulacija() {
    delete ui;
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
void Kalkulacija::AddRoot(QString naziv, QString cena, QString kolicina, QTreeWidget *tree) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(tree);
    itm->setText(0, naziv);
    itm->setText(1, "€" + cena);
    itm->setTextAlignment(1, Qt::AlignCenter);
    itm->setText(2, kolicina);
    itm->setTextAlignment(2, Qt::AlignCenter);
    ui->treeWidget_material->addTopLevelItem(itm);
    QColor color(210,210,210);
    QColor wcolor(250,250,250);
    if(m_count) {
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

// preberi txt datoteke
void Kalkulacija::Read(QString file, QTreeWidget *tree) {
    tree->clear();
    QFile mFile(file);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in Read() function in dodajpodjetje.cpp";
        return;
    }
    QTextStream in(&mFile);
    QString mText("");
    QRegExp rx("[;]");
    QStringList list;
    while(!in.atEnd()) {
        mText = in.readLine();
        list = mText.split(rx, QString::SkipEmptyParts);
        if(mText == "") {
            continue;
        } else {
            AddRoot(list.at(0), list.at(1), list.at(2), tree);
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
                AddRoot(list.at(0), list.at(1), list.at(2), tree);
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
        ui->lineEdit_nazivMateriala->setText(ui->treeWidget_material->currentItem()->text(0));
        ui->lineEdit_cenaMateriala->setText(ui->treeWidget_material->currentItem()->text(1).remove("€").remove(" "));
        ui->lineEdit_kolicinaMateriala->setText(ui->treeWidget_material->currentItem()->text(2).remove(" "));
    } else {
        NumOfItems numItems;
        numItems.setModal(true);
        numItems.exec();
        QString numMaterial = numItems.m_numKosov;
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_materialProdukta);
        itm->setText(0, ui->treeWidget_material->currentItem()->text(0));
        itm->setText(1, numMaterial);
        itm->setTextAlignment(1, Qt::AlignHCenter);
        QString price = QString::number((ui->treeWidget_material->currentItem()->text(1).remove("€").toDouble() / ui->treeWidget_material->currentItem()->text(2).toDouble()), 'f', 4);
        itm->setText(2, "€" + QString::number((price.toDouble() * numMaterial.toDouble()), 'f', 4));
        itm->setTextAlignment(2, Qt::AlignHCenter);
        QColor color(210,210,210);
        QColor wcolor(250,250,250);
        if(m_count) {
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
        m_materialCena += (price.toDouble() * numMaterial.toDouble());
        if(m_materialCena <= 0)
            m_materialCena = 0;
        ui->label_cenaMateriala->setText("€" + QString::number((m_materialCena + ((m_materialCena * ui->lineEdit_kalo->text().toDouble())/100)) / 1000, 'f', 4));
        ui->treeWidget_materialProdukta->setUniformRowHeights(true);
        ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
        ui->lineEdit_cenaProdukta->setText(ui->label_cenaMaterialaDela->text().remove("€"));
    }
}

// odstrani material
void Kalkulacija::on_treeWidget_materialProdukta_itemDoubleClicked() {
    QString price = ui->treeWidget_materialProdukta->currentItem()->text(2);
    m_materialCena -= price.remove("€").toDouble();
    ui->label_cenaMateriala->setText("€" + QString::number((m_materialCena + ((m_materialCena * ui->lineEdit_kalo->text().toDouble())/100)) / 1000, 'f', 4));
    if(m_materialCena <= 0)
        m_materialCena = 0;
    delete ui->treeWidget_materialProdukta->takeTopLevelItem(ui->treeWidget_materialProdukta->currentIndex().row());
    ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
    ui->lineEdit_cenaProdukta->setText(ui->label_cenaMaterialaDela->text().remove("€"));
}

// doda delo
void Kalkulacija::on_treeWidget_delo_itemDoubleClicked() {
    if(ui->checkBox_popraviOperacijo->isChecked()) {
        ui->lineEdit_operacija->setText(ui->treeWidget_delo->currentItem()->text(0));
        ui->lineEdit_cenaOperacije->setText(ui->treeWidget_delo->currentItem()->text(1).remove("€").remove(" "));
    } else {
        NumOfItems numItems;
        numItems.setModal(true);
        numItems.exec();
        QString numMaterial = numItems.m_numKosov;
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_deloProdukta);
        itm->setText(0, ui->treeWidget_delo->currentItem()->text(0));
        itm->setText(1, numMaterial);
        itm->setTextAlignment(1, Qt::AlignHCenter);
        QString price = ui->treeWidget_delo->currentItem()->text(1);
        itm->setText(2, "€" + QString::number((price.remove("€").toDouble() * numMaterial.toDouble()), 'f', 4));
        itm->setTextAlignment(2, Qt::AlignHCenter);
        QColor color(210,210,210);
        QColor wcolor(250,250,250);
        if(m_count) {
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
        m_deloCena += (price.toDouble() * numMaterial.toDouble());
        if(m_deloCena <= 0)
            m_deloCena = 0;
        ui->label_cenaDela->setText("€" + QString::number(m_deloCena / 1000, 'f', 4));
        ui->treeWidget_deloProdukta->setUniformRowHeights(true);
        ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
        ui->lineEdit_cenaProdukta->setText(ui->label_cenaMaterialaDela->text().remove("€"));
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
    ui->lineEdit_cenaProdukta->setText(ui->label_cenaMaterialaDela->text().remove("€"));
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
        QString allText = out.readAll();
        fileName.close();
        QString nazivProdukta = ui->lineEdit_nazivMateriala->text();
        if(nazivProdukta == "") nazivProdukta = "ni podatka";
        QString cenaProdukta = ui->lineEdit_cenaMateriala->text();
        if(cenaProdukta == "") cenaProdukta = "0.00";
        QString kolicinaProdukta = ui->lineEdit_kolicinaMateriala->text();
        if(kolicinaProdukta == "") kolicinaProdukta = "0";
        QRegularExpression trenutniMaterial(ui->treeWidget_material->currentItem()->text(0) + ";" + ui->treeWidget_material->currentItem()->text(1).remove("€") + ";" + ui->treeWidget_material->currentItem()->text(2) + ";", QRegularExpression::CaseInsensitiveOption);
        QString popravljenMaterial = nazivProdukta + ";" + cenaProdukta + ";" + kolicinaProdukta + ";";
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
        QString nazivProdukta = ui->lineEdit_nazivMateriala->text();
        if(nazivProdukta == "") nazivProdukta = "ni podatka";
        QString cenaProdukta = ui->lineEdit_cenaMateriala->text();
        if(cenaProdukta == "") cenaProdukta = "0.00";
        QString kolicinaProdukta = ui->lineEdit_kolicinaMateriala->text();
        if(kolicinaProdukta == "") kolicinaProdukta = "0";
        out << nazivProdukta << ";" << cenaProdukta << ";" << kolicinaProdukta << ";" << "\n";
        fileName.flush();
        fileName.close();
        Read(m_material, ui->treeWidget_material);
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
        QString allText = out.readAll();
        fileName.close();
        QString operacija = ui->lineEdit_operacija->text();
        if(operacija == "") operacija = "ni podatka";
        QString cenaOperacije = ui->lineEdit_cenaOperacije->text();
        if(cenaOperacije == "") cenaOperacije = "0.00";
        QString praznaCelica = " ";
        if(praznaCelica == "") praznaCelica = " ";
        QRegularExpression trenutnaOperacija(ui->treeWidget_delo->currentItem()->text(0) + ";" + ui->treeWidget_delo->currentItem()->text(1).remove("€") + ";" + ui->treeWidget_delo->currentItem()->text(2) + ";", QRegularExpression::CaseInsensitiveOption);
        QString popravljenaOperacija = operacija + ";" + cenaOperacije + ";" + praznaCelica + ";";
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
        QString operacija = ui->lineEdit_operacija->text();
        if(operacija == "") operacija = "ni podatka";
        QString cenaOperacije = ui->lineEdit_cenaOperacije->text();
        if(cenaOperacije == "") cenaOperacije = "0.00";
        QString praznaCelica = " ";
        if(praznaCelica == "") praznaCelica = " ";
        out << operacija << ";" << cenaOperacije << ";" << praznaCelica << ";" << "\n";
        fileName.flush();
        fileName.close();
        Read(m_delavniProces, ui->treeWidget_delo);
        ui->lineEdit_operacija->clear();
        ui->lineEdit_cenaOperacije->clear();
    }
}

// doda kalo v ceno
void Kalkulacija::on_lineEdit_kalo_textChanged() {
    ui->label_cenaMateriala->setText("€" + QString::number((m_materialCena + ((m_materialCena * ui->lineEdit_kalo->text().toDouble())/100)) / 1000, 'f', 4));
    ui->label_cenaMaterialaDela->setText("€" + QString::number(ui->label_cenaMateriala->text().remove("€").toDouble() + ui->label_cenaDela->text().remove("€").toDouble(), 'f', 4));
    ui->lineEdit_cenaProdukta->setText(ui->label_cenaMaterialaDela->text().remove("€"));
}

// pobrise lineedite za operacijo
void Kalkulacija::on_checkBox_popraviOperacijo_stateChanged() {
    ui->lineEdit_operacija->clear();
    ui->lineEdit_cenaOperacije->clear();
}

// pobrise lineedite za material
void Kalkulacija::on_checkBox_popravekMateriala_stateChanged() {
    ui->lineEdit_cenaMateriala->clear();
    ui->lineEdit_nazivMateriala->clear();
    ui->lineEdit_kolicinaMateriala->clear();
}
