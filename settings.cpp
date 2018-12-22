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
}

Settings::~Settings()
{
    delete ui;
}
void Settings::on_pushButton_shraniV_clicked()
{
    QFile mFile(m_fileName);
    QTextStream in(&mFile);
    QString mTextAll("");
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in pushButton_shraniV in settings.cpp";
        return;
    }
    else
    {
        mTextAll = in.readAll();
        mFile.close();
    }

    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for read-write in pushButton_shraniV in settings.cpp";
        return;
    }
    else
    {
        QRegularExpression exp(";");
        QString mTextLine("");
        QStringList mList;
        while(!in.atEnd())
        {
            mTextLine = in.readLine();
            mList = mTextLine.split(exp, QString::SkipEmptyParts);
            if(mList.at(0) == "Shrani v: ")
            {
                QString mLine = "Shrani v: ; " + m_dir + " ;\n";
                mTextAll.replace(mTextLine, mLine);
                break;
            }
        }
        mFile.close();

        if(!mFile.open(QFile::WriteOnly | QFile::Truncate))
        {
            qDebug() << "Error opening file for writing in pushButton_shraniV in settings.cpp";
            return;
        }
        else
        {
            mFile.flush();
            mFile.close();
        }
        if(!mFile.open(QFile::WriteOnly | QFile::Text))
        {
            qDebug() << "Error opening file for writing in pushButton_shraniV in settings.cpp";
            return;
        }
        else
        {
            in << mTextAll;
            mFile.flush();
            mFile.close();
        }
    }
    close();
}

void Settings::on_pushButton_nastaviShrani_clicked()
{
    m_dir = QFileDialog::getExistingDirectory(this, tr("Moj racunalnik"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}
