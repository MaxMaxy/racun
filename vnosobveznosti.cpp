#include "vnosobveznosti.h"
#include "ui_vnosobveznosti.h"

VnosObveznosti::VnosObveznosti(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VnosObveznosti), m_currentDir(QDir::currentPath()), m_upnikSeznam(m_currentDir + "/arhiv_upnikiSeznam.txt"), m_vnosObv(m_currentDir + "/arhiv_upniki.txt")
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Vnos obveznosti");
    ui->dateEdit_valute->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_valute->setDate(QDate::currentDate());
    ui->lineEdit_listina->setFocus();
    QRegularExpression regex("^[.0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_znesek->setValidator(validator);
    QRegularExpression regealfabet("^[a-zA-Z0-9,@. -/&#čšžŠČŽ]*$");
    QValidator *validatoralfabet = new QRegularExpressionValidator(regealfabet, this);
    ui->lineEdit_opombe->setValidator(validatoralfabet);
    ui->lineEdit_listina->setValidator(validatoralfabet);
    ui->lineEdit_dobavitelj->setValidator(validatoralfabet);
}

VnosObveznosti::~VnosObveznosti()
{
    delete ui;
}

void VnosObveznosti::VnosUpnika()
{
    QFile mFile(m_upnikSeznam);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening mFile in VnosObveznosti VnosUpnika";
        return;
    }
    else
    {
        QTextStream in(&mFile);
        QString mText("");
        QStringList list;
        QRegularExpression exp(";");
        bool foundItem(false);
        while(!in.atEnd())
        {
            mText = in.readLine();
            list = mText.split(exp, QString::SkipEmptyParts);
            mText = list.at(0);
            if(ui->lineEdit_dobavitelj->text() == mText)
            {
                foundItem = true;
                break;
            }
        }
        mFile.close();
        if(!foundItem)
        {
            if(!mFile.open(QFile::WriteOnly | QFile::Append))
            {
                qDebug() << "Error opening mFile in VnosObveznosti VnosUpnika writin part";
                return;
            }
            else
            {
                QString novUpnik = ui->lineEdit_dobavitelj->text();
                QTextStream out(&mFile);
                out << novUpnik << "; \n";
                mFile.flush();
                mFile.close();
            }
        }
    }
}

void VnosObveznosti::VnosObv()
{
    QFile mFile(m_vnosObv);
    if(!mFile.open(QFile::WriteOnly | QFile::Append))
    {
        qDebug() << "Error opening file in VnosObv";
        return;
    }
    else
    {
        QTextStream out(&mFile);
        QDateTime date = QDateTime::currentDateTime();
        QString mText = "Nova obveznost ; " +
                        date.toString("d. M. yyyy ; hh:mm:ss.zzz") +
                        " ; Listina: " + ui->lineEdit_listina->text() +
                        " ; Dobavitelj: " + ui->lineEdit_dobavitelj->text() +
                        " ; Znesek: " + ui->lineEdit_znesek->text() +
                        " ; Dat_valute: " + ui->dateEdit_valute->text() +
                        " ; Placilo: " +
                        " ; Dat_placila: " +
                        " ; Opombe: " + ui->lineEdit_opombe->text() +
                        " ; (end)";
        out << mText << "\n";
        mFile.flush();
        mFile.close();
    }
}

void VnosObveznosti::on_pushButton_vnos_clicked()
{
    VnosObv();
    VnosUpnika();
    VnosObveznosti::close();
}
