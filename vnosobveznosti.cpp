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
    this->setWindowFlags(Qt::Window);
    ui->dateEdit_valute->setDisplayFormat("d. M. yyyy");
    ui->dateEdit_valute->setDate(QDate::currentDate());
    ui->lineEdit_listina->setFocus();
    QRegularExpression regex("^[.,0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_znesek->setValidator(validator);
    QRegularExpression regealfabet("^[a-zA-Z0-9,@. -/&#čšžŠČŽ=]*$");
    QValidator *validatoralfabet = new QRegularExpressionValidator(regealfabet, this);
    ui->lineEdit_opombe->setValidator(validatoralfabet);
    ui->lineEdit_listina->setValidator(validatoralfabet);
    ui->lineEdit_dobavitelj->setValidator(validatoralfabet);
    ui->pushButton_vnos->setEnabled(true);
    ui->pushButton_vnos->setVisible(true);
    ui->pushButton_popraviObv->setEnabled(false);
    ui->pushButton_popraviObv->setVisible(false);
}

VnosObveznosti::~VnosObveznosti()
{
    delete ui;
}

void VnosObveznosti::closeEvent(QCloseEvent *) {
    emit close_me();
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
        in.setCodec("UTF-8");
        QString mText("");
        QStringList list;
        QRegularExpression exp(";");
        bool foundItem(false);
        while(!in.atEnd())
        {
            mText = in.readLine();
            list = mText.split(exp, QString::SkipEmptyParts);
            mText = list.at(0);
            if(ui->lineEdit_dobavitelj->text().toUpper() == mText)
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
                QString novUpnik = ui->lineEdit_dobavitelj->text().toUpper();
                QTextStream out(&mFile);
                out.setCodec("UTF-8");
                out << novUpnik << ";\n";
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
        out.setCodec("UTF-8");
        QDateTime date = QDateTime::currentDateTime();
        QString cena = QString::number(ui->lineEdit_znesek->text().replace(',', '.').toDouble(), 'f', 2);
        QString mText = "Nova obveznost ; " +
                        date.toString("d. M. yyyy ; hh:mm:ss.zzz") +
                        " ; Listina: " + ui->lineEdit_listina->text().toUpper() +
                        " ; Dobavitelj: " + ui->lineEdit_dobavitelj->text().toUpper() +
                        " ; Znesek: " + cena +
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

void VnosObveznosti::PopraviObveznost(QString dobavitelj, QString listina, QString znesek, QString datum, QString opomba, QString staraObveznost) {
    ui->lineEdit_dobavitelj->setText(dobavitelj);
    ui->lineEdit_listina->setText(listina);
    ui->lineEdit_znesek->setText(znesek);
    ui->dateEdit_valute->setDate(QDate::fromString(datum));
    ui->lineEdit_opombe->setText(opomba);
    ui->pushButton_vnos->setEnabled(false);
    ui->pushButton_vnos->setVisible(false);
    ui->pushButton_popraviObv->setEnabled(true);
    ui->pushButton_popraviObv->setVisible(true);
    m_staraObveznost = staraObveznost;
}

void VnosObveznosti::on_pushButton_vnos_clicked()
{
    VnosObv();
    VnosUpnika();
    VnosObveznosti::close();
}

void VnosObveznosti::on_lineEdit_listina_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_listina->backspace();
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == '.') {
        ui->lineEdit_listina->backspace();
        ui->lineEdit_listina->backspace();
        ui->lineEdit_listina->insert(".");
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ',') {
        ui->lineEdit_listina->backspace();
        ui->lineEdit_listina->backspace();
        ui->lineEdit_listina->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_listina->backspace();
    }
}

void VnosObveznosti::on_lineEdit_dobavitelj_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_dobavitelj->backspace();
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == '.') {
        ui->lineEdit_dobavitelj->backspace();
        ui->lineEdit_dobavitelj->backspace();
        ui->lineEdit_dobavitelj->insert(".");
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ',') {
        ui->lineEdit_dobavitelj->backspace();
        ui->lineEdit_dobavitelj->backspace();
        ui->lineEdit_dobavitelj->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_dobavitelj->backspace();
    }
}

void VnosObveznosti::on_lineEdit_znesek_textChanged(const QString &arg1)
{
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == '.') {
        ui->lineEdit_znesek->backspace();
    }
    if(arg1.contains("..")) {
        ui->lineEdit_znesek->backspace();
    }
}

void VnosObveznosti::on_lineEdit_opombe_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_opombe->backspace();
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == '.') {
        ui->lineEdit_opombe->backspace();
        ui->lineEdit_opombe->backspace();
        ui->lineEdit_opombe->insert(".");
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ',') {
        ui->lineEdit_opombe->backspace();
        ui->lineEdit_opombe->backspace();
        ui->lineEdit_opombe->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_opombe->backspace();
    }
}

void VnosObveznosti::on_pushButton_izhod_clicked() {
    close();
}

void VnosObveznosti::on_pushButton_popraviObv_clicked() {
    QFile m_file(m_vnosObv);
    QString allText("");
    if(!m_file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error pri popravljanju obveznosti";
        return;
    } else {
        QTextStream in(&m_file);
        in.setCodec("UTF-8");
        allText = in.readAll();
        m_file.close();
        QRegularExpression exp(";");
        QStringList list = m_staraObveznost.split(exp, QString::SkipEmptyParts);
        list.replace(3, " Listina: " + ui->lineEdit_listina->text());
        list.replace(4, " Dobavitelj: " + ui->lineEdit_dobavitelj->text());
        list.replace(5, " Znesek: " + ui->lineEdit_znesek->text().replace(",", "."));
        list.replace(6, " Dat_valute: " + ui->dateEdit_valute->text());
        list.replace(9, " Opombe: " + ui->lineEdit_opombe->text());
        QString novaObveznost(list.at(0) + ";" + list.at(1) + ";" + list.at(2) + ";" + list.at(3) + " ;" + list.at(4) + " ;" + list.at(5) + " ;" + list.at(6) + " ;" + list.at(7) + ";" + list.at(8) + ";" + list.at(9) + " ;" + list.at(10));
        allText.replace(m_staraObveznost, novaObveznost);
        if(!m_file.open(QFile::WriteOnly | QFile::Truncate)) {
            qDebug() << "Error opening mFile for truncate in popravi button";
            return;
        }
        m_file.flush();
        m_file.close();
        if(!m_file.open(QFile::WriteOnly | QFile::Text)) {
            qDebug() << "Error opening mFile for writing in popravi button";
            return;
        }
        in << allText;
        m_file.flush();
        m_file.close();
    }
    close();
}

void VnosObveznosti::on_lineEdit_dobavitelj_editingFinished() {
    if(ui->lineEdit_dobavitelj->text().at(ui->lineEdit_dobavitelj->text().length() - 1) == ' ')
        ui->lineEdit_dobavitelj->backspace();
}
