#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent), ui(new Ui::Settings), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/settings.txt"),
    m_dir(m_currentDir), m_backupDir(m_currentDir), m_newDir(""), m_newBackupDir(""), m_racun(false), m_backup(false)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Nastavitve");
    ui->pushButton_shraniV->setFocus();
    QFile mFile(m_fileName);
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QString mTextAll("");
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in pushButton_shraniV in settings.cpp";
        return;
    } else {
        mTextAll = in.readAll();
        mFile.close();
    }
    QRegularExpression exp(";");
    QStringList mList;
    if(mTextAll == "") {
        mList.insert(0, "/");
    } else {
        mList = mTextAll.split(exp, QString::SkipEmptyParts);
    }
    m_backupDir = mList.at(3);
    m_backupDir.remove(0,1);
    m_backupDir.remove(-1,1);
    m_dir = mList.at(1);
    m_dir.remove(0,1);
    m_dir.remove(-1,1);
    ui->label_lokacija->setText(m_dir);
    ui->label_backupLokacija->setText(m_backupDir);
    ui->label_shranjene->setText("Nastavitve so shranjene");
    ui->label_shranjene->setStyleSheet("QLabel { color : green; }");
}

Settings::~Settings() {
    delete ui;
}

void Settings::closeEvent(QCloseEvent *) {
    emit close_me();
}

void Settings::on_pushButton_shraniV_clicked()
{
    QFile mFile(m_fileName);
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QString mTextAll("");
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in pushButton_shraniV in settings.cpp";
        return;
    } else {
        mTextAll = in.readAll();
        mFile.close();
        if(!mFile.open(QFile::WriteOnly | QFile::Truncate)) {
            qDebug() << "Error opening file for writing in pushButton_shraniV in settings.cpp";
            return;
        } else {
            QRegularExpression exp(";");
            QStringList mList;
            if(mTextAll == "") {
                QString mLine = "Shrani v: ; " + m_newDir + " ;\n" + "Backup : ; " + m_newBackupDir + " ;\n";
                in << mLine;
                mFile.flush();
                mFile.close();
                ui->label_shranjene->setText("Nastavitve so shranjene");
                ui->label_shranjene->setStyleSheet("QLabel { color : green; }");
            }
            else if(m_backup && m_racun) {
                mList = mTextAll.split(exp, QString::SkipEmptyParts);
                QString mLine = "Shrani v: ; " + m_newDir + " ;\n" + "Backup : ; " + m_newBackupDir + " ;\n";
                mTextAll.replace(mTextAll, mLine);
                in << mTextAll;
                mFile.flush();
                mFile.close();
                ui->label_shranjene->setText("Nastavitve so shranjene");
                ui->label_shranjene->setStyleSheet("QLabel { color : green; }");
            }
            else if(m_racun) {
                mList = mTextAll.split(exp, QString::SkipEmptyParts);
                QString mLine = "Shrani v: ; " + m_newDir + " ;\n" + "Backup : ; " + m_backupDir + " ;\n";
                mTextAll.replace(mTextAll, mLine);
                in << mTextAll;
                mFile.flush();
                mFile.close();
                ui->label_shranjene->setText("Nastavitve so shranjene");
                ui->label_shranjene->setStyleSheet("QLabel { color : green; }");
            }
            else if(m_backup) {
                mList = mTextAll.split(exp, QString::SkipEmptyParts);
                QString mLine = "Shrani v: ; " + m_dir + " ;\n" + "Backup : ; " + m_newBackupDir + " ;\n";
                mTextAll.replace(mTextAll, mLine);
                in << mTextAll;
                mFile.flush();
                mFile.close();
                ui->label_shranjene->setText("Nastavitve so shranjene");
                ui->label_shranjene->setStyleSheet("QLabel { color : green; }");
            }
            if((!QDir(m_newDir + "/Racuni_in_dobavnice").exists()) && QDir(m_dir + "/Racuni_in_dobavnice").exists() && m_racun) {
                QFile file(m_dir + "/Racuni_in_dobavnice");
                file.rename(m_newDir + "/Racuni_in_dobavnice");
            }
            if((!QDir(m_newBackupDir + "/ElraSetiBackup").exists()) && QDir(m_backupDir + "/ElraSetiBackup").exists() && m_backup) {
                QFile file(m_backupDir + "/ElraSetiBackup");
                file.rename(m_newBackupDir + "/ElraSetiBackup");
            }
        }
    }
    m_dir = m_newDir;
    m_backupDir = m_newBackupDir;
    m_backup = false;
    m_racun = false;
}

void Settings::on_pushButton_nastaviShrani_clicked() {
    m_newDir = QFileDialog::getExistingDirectory(this, tr("Moj racunalnik"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->label_lokacija->setText(m_newDir + "/Racuni_in_dobavnice");
    ui->label_backupLokacija->setText(m_backupDir);
    ui->label_shranjene->setText("Nastavitve niso shranjene");
    ui->label_shranjene->setStyleSheet("QLabel { color : red; }");
    m_racun = true;
}

void Settings::on_pushButton_clicked() {
    m_newBackupDir = QFileDialog::getExistingDirectory(this, tr("Moj racunalnik"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->label_lokacija->setText(m_dir  + "/Racuni_in_dobavnice");
    ui->label_backupLokacija->setText(m_newBackupDir + "/ElraSetiBackup");
    ui->label_shranjene->setText("Nastavitve niso shranjene");
    ui->label_shranjene->setStyleSheet("QLabel { color : red; }");
    m_backup = true;
}

void Settings::on_pushButton_izhod_clicked() {
    close();
}
