#include "dodajprodukt.h"
#include "ui_dodajprodukt.h"

DodajProdukt::DodajProdukt(QWidget *parent) :
    QDialog(parent), ui(new Ui::DodajProdukt), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/company_file.txt"),
    m_arhivProdukti(m_currentDir + "/arhiv_produkti.txt"), m_id(""), m_naziv(""), m_cena(""), m_produkt(""), m_count(true), m_itr(0)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowFlags(Qt::Window);
    this->showMaximized();
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setStretchLastSection(false);
    ui->lineEdit_id->setMaxLength(7);
    ui->lineEdit_nazivProdukta->setMaxLength(64);
    QRegularExpression regenum("^[0123456789/]*$");
    QValidator *validatornum = new QRegularExpressionValidator(regenum, this);
    ui->lineEdit_id->setValidator(validatornum);
    QRegularExpression regex("^[.,0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_cena->setValidator(validator);
    QRegularExpression regealfabet("^[a-zA-Z0-9,@. -/&#čšžŠČŽ()+=]*$");
    QValidator *validatoralfabet = new QRegularExpressionValidator(regealfabet, this);
    ui->lineEdit_nazivProdukta->setValidator(validatoralfabet);
    this->setWindowTitle("Dodaj - popravi produkt");
    ui->pushButton_isci->setFocus();
    ui->pushButton_isci->setVisible(false);
    ui->treeWidget->setRootIsDecorated(false);
    ui->pushButton_popravi->setEnabled(false);
    AddItemsToCombo();
    Read();
}

DodajProdukt::~DodajProdukt()
{
    delete ui;
}

void DodajProdukt::closeEvent(QCloseEvent *) {
    emit close_me();
}

void DodajProdukt::Arhiv(QString arhiv_file, QString stream)
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
    out.setCodec("UTF-8");
    out << stream << "\n";
    mFile.flush();
    mFile.close();
}

void DodajProdukt::SetIndex(int index)
{
    ui->comboBox_podjetje->setCurrentIndex(index);
}

// dodat root v treeWidget
void DodajProdukt::AddRoot(QString id, QString naziv, QString cena)
{
    // pointer na treeWidget
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    // nastavimo stevilko podjetja in ime podjetja v colom 0 in 1
    itm->setText(0, id);
    itm->setTextAlignment(0, Qt::AlignHCenter);
    itm->setText(1, naziv);
    itm->setTextAlignment(1, Qt::AlignHCenter);
    itm->setText(2, "€" + cena);
    itm->setTextAlignment(2, Qt::AlignHCenter);
    // dodamo podjetje v treeWidget
    ui->treeWidget->addTopLevelItem(itm);
    // nastavimo barvo za vsako drugo podjetje
    QColor color(210,210,210);
    QColor wcolor(250,250,250);
    // m_count je member bool ce je tru je barva siva drugace bela
    if(ui->treeWidget->topLevelItemCount() % 2 == 0) m_count = true;
    else m_count = false;
    if(m_count)
    {
        itm->setBackgroundColor(0,color);
        itm->setBackgroundColor(1,color);
        itm->setBackgroundColor(2,color);
    } else {
        itm->setBackgroundColor(0,wcolor);
        itm->setBackgroundColor(1,wcolor);
        itm->setBackgroundColor(2,wcolor);
    }
}

// Preberi produkt iz file-a
void DodajProdukt::Read()
{
    // zbrise vse iz treeWidgeta
    ui->treeWidget->clear();
    int izbPod = ui->comboBox_podjetje->currentIndex() + 1;
    QString podjetje = QString::number(izbPod) + ".txt";

    QFile fileName(podjetje);
    // test ce je odprt za branje
    if(!fileName.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read()";
        return;
    }
    // stream za branje fila
    QTextStream in(&fileName);
    in.setCodec("UTF-8");
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
void DodajProdukt::AddItemsToCombo()
{
    // zbrise vse iz combo boxa
    ui->comboBox_podjetje->clear();
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
    in.setCodec("UTF-8");
    // preberi vsako vrstico ce je prazna izpusti drugace dodaj vrstico v combo box
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
            // doda podjetje v combo box
            ui->comboBox_podjetje->addItem(list.at(1));
        }
    }
    // zapremo file
    mFile.close();
    Read();
}

void DodajProdukt::on_pushButton_dodaj_clicked()
{
    int izbPod = ui->comboBox_podjetje->currentIndex() + 1;
    QString podjetje = QString::number(izbPod) + ".txt";

    QFile fileName(podjetje);
    if(!fileName.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "Error opening fileName for writing in dodaj produkt gumb";
        return;
    }
    QTextStream out(&fileName);
    out.setCodec("UTF-8");
    m_id = ui->lineEdit_id->text();
    if(m_id == "")
        m_id = "/";
    m_naziv = ui->lineEdit_nazivProdukta->text();
    m_naziv.replace(0,1,m_naziv.at(0).toUpper());
    while(m_naziv.at(m_naziv.length() - 1) == ' ') {
        m_naziv.remove(-1, 1);
    }
    if(m_naziv == "")
        m_naziv = "ni podatka";
    m_cena = ui->lineEdit_cena->text();
    if(m_cena == "")
        m_cena = "0";
    m_cena.replace(',', '.');
    out << m_id << ";" << m_naziv << ";" << m_cena << ";" << "\n";
    fileName.flush();
    fileName.close();
    Read();
    QDateTime date = QDateTime::currentDateTime();
    QString dodano = "Dodano";
    QString arhiv = dodano + " ; " + date.toString("dd.MM.yyyy ; hh:mm:ss.zzz") + " ; " + m_id + " ; " + m_naziv + " ; " + m_cena + " ; " + "Rajh" + " !?!";
    Arhiv(m_arhivProdukti, arhiv);
    ui->lineEdit_id->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->lineEdit_cena->clear();
}

void DodajProdukt::on_comboBox_podjetje_currentIndexChanged()
{
    ui->pushButton_dodaj->setEnabled(true);
    Read();
}

void DodajProdukt::on_treeWidget_doubleClicked(const QModelIndex &index) {
    if(index.column() != 2) {
        ui->pushButton_popravi->setEnabled(true);
        int izbPod = ui->comboBox_podjetje->currentIndex() + 1;
        QString podjetje = QString::number(izbPod) + ".txt";
        QFile mFile(podjetje);
        if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "Error opening mFile for reading in list clicked";
            return;
        }
        QTextStream in(&mFile);
        in.setCodec("UTF-8");
        QString text("");
        while(!in.atEnd()) {
            text = in.readLine();
            if(text.contains(index.data().toString(), Qt::CaseInsensitive)) {
                m_produkt = text;
                QRegExp rx("[;]");
                QStringList list = text.split(rx, QString::SkipEmptyParts);
                ui->lineEdit_id->setText(list.at(0));
                ui->lineEdit_nazivProdukta->setText(list.at(1));
                ui->lineEdit_cena->setText(list.at(2));
                m_id = ui->lineEdit_id->text();
                m_naziv = ui->lineEdit_nazivProdukta->text();
                m_cena = ui->lineEdit_cena->text();
                ui->pushButton_dodaj->setEnabled(false);
            }
        }
        mFile.close();
    }
}

void DodajProdukt::on_pushButton_popravi_clicked()
{
    if(ui->treeWidget->currentItem() == nullptr)
    {
        return;
    }
    int izbPod = ui->comboBox_podjetje->currentIndex() + 1;
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
    out.setCodec("UTF-8");
    // prebere celoten dokument in shrani v var
    QString allText = out.readAll();
    // zapre file
    mFile.close();
    // zamenja spremenjene dele besedila
    QRegularExpression produkt(m_produkt);
    QString rep_produkt(ui->lineEdit_id->text() + ";" + ui->lineEdit_nazivProdukta->text() + ";" + ui->lineEdit_cena->text() + ";");
    allText.replace(produkt, rep_produkt);
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
    QString arhiv = popravljeno + " ; " + date.toString("dd.MM.yyyy ; hh:mm:ss.zzz") + " ; " + m_id + " ; " + m_naziv + " ; " + m_cena + " ; " + "Rajh" + " !?!";
    Arhiv(m_arhivProdukti, arhiv);
    ui->lineEdit_id->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->lineEdit_cena->clear();
    ui->lineEdit_isci->clear();
    ui->pushButton_popravi->setEnabled(false);
    ui->pushButton_dodaj->setEnabled(true);
}

void DodajProdukt::Search(QString searchName) {
    int izbPod = ui->comboBox_podjetje->currentIndex() + 1;
    QString podjetje = QString::number(izbPod) + ".txt";
    QFile mFile(podjetje);
    if(!mFile.open(QFile::Text | QFile::ReadOnly)) {
        qDebug() << "Error opening mFile for reading in popravi button";
        return;
    }
    QTextStream out(&mFile);
    out.setCodec("UTF-8");
    QString line;
    while(!out.atEnd()){
        line = out.readLine();
        if(line.contains(searchName, Qt::CaseInsensitive)) {
            QRegExp rx("[;]");
            QStringList list;
            list = line.split(rx, QString::SkipEmptyParts);
            if(list.at(0) == "Dodano: ")
                break;
            else {
                AddRoot(list.at(0), list.at(1), list.at(2));
            }
        }
    }
    mFile.close();
}

void DodajProdukt::on_lineEdit_isci_textChanged()
{
    QString search = ui->lineEdit_isci->text();
    if(search == "")
        Read();
    else {
        ui->treeWidget->clear();
        Search(search);
    }
}

void DodajProdukt::on_lineEdit_nazivProdukta_textChanged(const QString &arg1) {
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
    if(ui->lineEdit_id->text() == "" && ui->lineEdit_nazivProdukta->text() == "" && ui->lineEdit_cena->text() == "") {
        ui->pushButton_dodaj->setEnabled(true);
        ui->pushButton_popravi->setEnabled(false);
    }
}

void DodajProdukt::on_lineEdit_cena_textChanged(const QString &arg1) {
    if(arg1.length() == 1 && (arg1.at(arg1.length()-1) == '.' || arg1.at(arg1.length()-1) == ',')) {
        ui->lineEdit_cena->backspace();
    }
    if(arg1.contains("..") || arg1.contains(",,") || arg1.contains(",.") || arg1.contains(".,")) {
        ui->lineEdit_cena->backspace();
    }
    if(ui->lineEdit_id->text() == "" && ui->lineEdit_nazivProdukta->text() == "" && ui->lineEdit_cena->text() == "") {
        ui->pushButton_dodaj->setEnabled(true);
        ui->pushButton_popravi->setEnabled(false);
    }
}

void DodajProdukt::on_lineEdit_id_textChanged() {
    if(ui->lineEdit_id->text() == "" && ui->lineEdit_nazivProdukta->text() == "" && ui->lineEdit_cena->text() == "") {
        ui->pushButton_dodaj->setEnabled(true);
        ui->pushButton_popravi->setEnabled(false);
    }
}

void DodajProdukt::on_pushButton_clicked() {
    close();
}
