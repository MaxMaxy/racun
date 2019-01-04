#include "dodajpodjetje.h"
#include "ui_dodajpodjetje.h"

DodajPodjetje::DodajPodjetje(QWidget *parent) :
    QDialog(parent), ui(new Ui::DodajPodjetje), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/company_file.txt"), m_numOfCompany(m_currentDir + "/num_company.txt"), m_arhivStranke(m_currentDir + "/arhiv_stranke.txt"), m_zapSt("") ,m_cNaziv(""), m_kNaziv(""), m_naslov(""),m_posta(""), m_ddv(""), m_email(""), m_stranka(""), m_itr(0), m_totalCompanys(0), m_count(true)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowFlags(Qt::Window);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setColumnWidth(0,40);
    QRegularExpression regex("^[a-zA-Z0-9,@. -/&#čšžŠČŽ]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_kNaziv->setValidator(validator);
    ui->lineEdit_cNaziv->setValidator(validator);
    ui->lineEdit_naslov->setValidator(validator);
    ui->lineEdit_posta->setValidator(validator);
    ui->lineEdit_ddv->setValidator(validator);
    ui->lineEdit_email->setValidator(validator);
    this->setWindowTitle("Dodaj - popravi stranko");
    Read();
    TotalCompany();
}

DodajPodjetje::~DodajPodjetje() {
    delete ui;
}

// total companys(clients) function
void DodajPodjetje::TotalCompany() {
    QFile mNum(m_numOfCompany);
    if(!mNum.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in TotalCompany() function in dodajpodjetje.cpp";
        return;
    }
    QTextStream in(&mNum);
    QString number;
    while(!in.atEnd()) {
        number = in.readLine();
    }
    m_zapSt = number;
    mNum.close();
}

// add root to treeWidget
void DodajPodjetje::AddRoot(QString name, QString num, QString cNaziv, QString naslov,QString posta, QString ddv, QString email) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0, num);
    itm->setText(1, name);
    ui->treeWidget->addTopLevelItem(itm);
    QColor color(210,210,210);
    QColor wcolor(250,250,250);
    if(m_count) {
        itm->setBackgroundColor(0,color);
        itm->setBackgroundColor(1,color);
        m_count = false;
    } else {
        itm->setBackgroundColor(0,wcolor);
        itm->setBackgroundColor(1,wcolor);
        m_count = true;
    }
    AddChild(itm, cNaziv);
    AddChild(itm, naslov);
    AddChild(itm, posta);
    AddChild(itm, ddv);
    AddChild(itm, email);
}

// add child to treeWidget
void DodajPodjetje::AddChild(QTreeWidgetItem* parent, QString name) {
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(1, name);
    QFont font;
    font.setBold(true);
    font.setItalic(true);
    itm->setFont(1,font);
    parent->addChild(itm);
}

// add to arhive
void DodajPodjetje::Arhiv(QString arhiv_file, QString stream) {
    QFile mFile(arhiv_file);
    if(!mFile.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "Error opening file for writing in Arhiv() function in dodajpodjetje.cpp";
        return;
    }
    QTextStream out(&mFile);
    out << stream << "\n";
    mFile.flush();
    mFile.close();
}

// doda novo stranko
void DodajPodjetje::Write(QString file_podjetje, QString file_num) {
    QFile mFile(file_podjetje);
    if(!mFile.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "Error opening file for writing in Write() function in dodajpodjetje.cpp (first opening)";
        return;
    }
    QFile mNum(file_num);
    if(!mNum.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in Write() function in dodajpodjetje.cpp";
        return;
    }
    QTextStream in(&mNum);
    QString number;
    while(!in.atEnd()) {
        number = in.readLine();
    }
    m_zapSt = number;
    mNum.close();
    QTextStream out(&mFile);
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
    out << number << ";" << m_kNaziv << ";" << m_cNaziv << ";" << m_naslov << ";" << m_posta << ";" << m_ddv << ";" << m_email << ";" << "\n";
    mFile.flush();
    mFile.close();
    if(!mNum.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "Error opening file for writing in Write() function in dodajpodjetje.cpp (second opening)";
        return;
    }
    int numInt = number.toInt();
    numInt++;
    number = QString::number(numInt);
    in << "\n" << number;
    mNum.flush();
    mNum.close();
}

// read function for treeWidget
void DodajPodjetje::Read() {
    ui->treeWidget->clear();
    QFile mFile(m_fileName);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in Read() function in dodajpodjetje.cpp";
        return;
    }
    QTextStream in(&mFile);
    m_itr = 1;
    QString mText("");
    QRegExp rx("[;]");
    QStringList list;
    while(!in.atEnd()) {
        mText = in.readLine();
        list = mText.split(rx, QString::SkipEmptyParts);
        if(mText == "") {
            continue;
        } else {
            AddRoot(list.at(1), list.at(0), list.at(2), list.at(3), list.at(4), list.at(5), list.at(6));
            m_itr++;
        }
    }
    mFile.close();
    QFile mNum(m_numOfCompany);
    if(!mNum.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "Error opening file for writing in Read() function in dodajpodjetje.cpp";
        return;
    }
    mNum.flush();
    mNum.close();
    if(!mNum.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "Error opening file for writing in Read() function in dodajpodjetje.cpp #2";
        return;
    }
    QTextStream out(&mNum);
    for(int i(1); i <= m_itr; i++) {
        out << i << "\n";
    }
    mNum.flush();
    mNum.close();
}

// button for add new client
void DodajPodjetje::on_pushButton_dodajPodjetje_clicked() {
    Write(m_fileName, m_numOfCompany);
    QFile file;
    file.setFileName(m_zapSt + ".txt");
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "File not opened in pushButton_dodajPodjetje";
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

// button to fix already added clients
void DodajPodjetje::on_pushButton_popraviPodatke_clicked() {
    if(ui->treeWidget->currentItem() == nullptr) {
        return;
    }
    QFile mFile(m_fileName);
    if(!mFile.open(QFile::Text | QFile::ReadOnly)) {
        qDebug() << "Error opening file for reading in pushButton_popraviPodatke in dodajpodjetje.cpp";
        return;
    }
    QTextStream out(&mFile);
    QString allText = out.readAll();
    mFile.close();
    QRegularExpression stranka(m_stranka);
    QString rep_stranka(m_zapSt + ";" + ui->lineEdit_kNaziv->text() + ";" + ui->lineEdit_cNaziv->text() + ";" + ui->lineEdit_naslov->text() + ";" + ui->lineEdit_posta->text() + ";" + ui->lineEdit_ddv->text() + ";" + ui->lineEdit_email->text() + ";");
    allText.replace(stranka, rep_stranka);
    if(!mFile.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "Error opening mFile for truncate in pushButton_popraviPodatke in dodajpodjetje.cpp";
        return;
    }
    mFile.flush();
    mFile.close();
    if(!mFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Error opening file for writing in pushButton_popraviPodatke in dodajpodjetje.cpp";
        return;
    }
    out << allText;
    mFile.flush();
    mFile.close();
    Read();
    QDateTime date = QDateTime::currentDateTime();
    QString popravljeno = "Popravljeno";
    QString arhiv = popravljeno + " ; " + date.toString("dd.MM.yyyy ; hh:mm:ss.zzz") + " ; " + m_kNaziv + " ; " + m_cNaziv + " ; " + m_naslov + " ; " + m_posta + " ; " + m_ddv + " ; " + m_email + " ; " + "Rajh" + " !?!";
    Arhiv(m_arhivStranke, arhiv);
}

// duble clicked treeWidget function
void DodajPodjetje::on_treeWidget_doubleClicked(const QModelIndex &index) {
    ui->treeWidget->collapseAll();
    if(index.parent().row() == -1) {
        QFile mFile(m_fileName);
        if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "Error opening file for reading in treeWidget_doubleClicked function in dodajpodjetje.cpp";
            return;
        }
        QTextStream in(&mFile);
        m_itr = -1;
        QString text("");
        while(!(m_itr == index.row())) {
            text = in.readLine();
            m_itr++;
            if(m_itr > 100) break;
        }
        m_stranka = text;
        QRegExp rx("[;]");
        QStringList list = text.split(rx, QString::SkipEmptyParts);
        ui->lineEdit_kNaziv->setText(list.at(1));
        ui->lineEdit_cNaziv->setText(list.at(2));
        ui->lineEdit_naslov->setText(list.at(3));
        ui->lineEdit_posta->setText(list.at(4));
        ui->lineEdit_ddv->setText(list.at(5));
        ui->lineEdit_email->setText(list.at(6));
        m_zapSt = list.at(0);
        m_cNaziv = ui->lineEdit_cNaziv->text();
        m_kNaziv = ui->lineEdit_kNaziv->text();
        m_naslov = ui->lineEdit_naslov->text();
        m_posta = ui->lineEdit_posta->text();
        m_ddv = ui->lineEdit_ddv->text();
        m_email = ui->lineEdit_email->text();
    }
}

void DodajPodjetje::on_lineEdit_kNaziv_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_kNaziv->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit_kNaziv->backspace();
        ui->lineEdit_kNaziv->backspace();
        ui->lineEdit_kNaziv->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit_kNaziv->backspace();
        ui->lineEdit_kNaziv->backspace();
        ui->lineEdit_kNaziv->insert(",");
    }
}

void DodajPodjetje::on_lineEdit_cNaziv_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_cNaziv->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit_cNaziv->backspace();
        ui->lineEdit_cNaziv->backspace();
        ui->lineEdit_cNaziv->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit_cNaziv->backspace();
        ui->lineEdit_cNaziv->backspace();
        ui->lineEdit_cNaziv->insert(",");
    }
}

void DodajPodjetje::on_lineEdit_naslov_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_naslov->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit_naslov->backspace();
        ui->lineEdit_naslov->backspace();
        ui->lineEdit_naslov->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit_naslov->backspace();
        ui->lineEdit_naslov->backspace();
        ui->lineEdit_naslov->insert(",");
    }
}

void DodajPodjetje::on_lineEdit_posta_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_posta->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit_posta->backspace();
        ui->lineEdit_posta->backspace();
        ui->lineEdit_posta->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit_posta->backspace();
        ui->lineEdit_posta->backspace();
        ui->lineEdit_posta->insert(",");
    }
}

void DodajPodjetje::on_lineEdit_ddv_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_ddv->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit_ddv->backspace();
        ui->lineEdit_ddv->backspace();
        ui->lineEdit_ddv->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit_ddv->backspace();
        ui->lineEdit_ddv->backspace();
        ui->lineEdit_ddv->insert(",");
    }
}

void DodajPodjetje::on_lineEdit_email_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_email->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit_email->backspace();
        ui->lineEdit_email->backspace();
        ui->lineEdit_email->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit_email->backspace();
        ui->lineEdit_email->backspace();
        ui->lineEdit_email->insert(",");
    }
}
