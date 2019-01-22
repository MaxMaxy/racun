#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent), ui(new Ui::Settings), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/settings.txt"), m_dir(m_currentDir)
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
    ui->label_lokacija->setText(mList.at(1));
}

Settings::~Settings()
{
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
                mList.insert(0, "/");
            } else {
                mList = mTextAll.split(exp, QString::SkipEmptyParts);
            }
            if(mList.at(0) == "Shrani v: ") {
                QString mLine = "Shrani v: ; " + m_dir + " ;\n";
                mTextAll.replace(mTextAll, mLine);
                in << mTextAll;
                mFile.flush();
                mFile.close();
            } else {
                QString mLine = "Shrani v: ; " + m_dir + " ;\n";
                in << mLine;
                mFile.flush();
                mFile.close();
            }
        }
    }
    close();
}

void Settings::on_pushButton_nastaviShrani_clicked()
{
    m_dir = QFileDialog::getExistingDirectory(this, tr("Moj racunalnik"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}
