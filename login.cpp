#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/login_file.txt"), m_arhivLogin(m_currentDir + "/arhiv_login.txt")
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Prijava");
    QDir::setCurrent(m_currentDir);
}

Login::~Login()
{
    delete ui;
}


void Login::Arhiv(QString arhiv_file, QString stream)
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


void Login::on_pushButton_clicked()
{
    QApplication::quit();
}

void Login::on_pushButton_prijava_clicked()
{
    QDateTime date = QDateTime::currentDateTime();
    QFile fileName(m_fileName);
    if(!fileName.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Cannot open file for reading in Login";
        return;
    }
    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    QString name;
    while(!in.atEnd())
    {
        name = in.readLine();
        if(ui->lineEdit_login->text() == name)
        {
            QString stream = "Prijava v sistem: " + date.toString("dd.MM.yyyy - hh:mm:ss.zzz ; ") + ui->lineEdit_login->text();
            Arhiv(m_arhivLogin, stream);
            close();
            break;
        }
        else
        {
            QString stream = "Nepravilna prijava: " + date.toString("dd.MM.yyyy - hh:mm:ss.zzz ; ") + ui->lineEdit_login->text();
            Arhiv(m_arhivLogin, stream);
            ui->lineEdit_login->clear();
            return;
        }
    }
}
