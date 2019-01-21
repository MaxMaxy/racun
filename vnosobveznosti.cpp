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
    QRegularExpression regealfabet("^[a-zA-Z0-9,@. -/&#čšžŠČŽ()+=]*$");
    QValidator *validatoralfabet = new QRegularExpressionValidator(regealfabet, this);
    ui->lineEdit_opombe->setValidator(validatoralfabet);
    ui->lineEdit_listina->setValidator(validatoralfabet);
    ui->lineEdit_dobavitelj->setValidator(validatoralfabet);
}

VnosObveznosti::~VnosObveznosti()
{
    delete ui;
}

void VnosObveznosti::closeEvent(QCloseEvent *) {
    emit close_me();
}

void VnosObveznosti::VnosUpnika() {
    QFile mFile(m_upnikSeznam);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening mFile in VnosObveznosti VnosUpnika";
        return;
    } else {
        QTextStream in(&mFile);
        in.setCodec("UTF-8");
        QString mText("");
        QStringList list;
        QRegularExpression exp(";");
        bool foundItem(false);
        while(!in.atEnd()) {
            mText = in.readLine();
            list = mText.split(exp, QString::SkipEmptyParts);
            mText = list.at(0);
            if(ui->lineEdit_dobavitelj->text().toUpper() == mText) {
                foundItem = true;
                break;
            }
        }
        mFile.close();
        if(!foundItem) {
            if(!mFile.open(QFile::WriteOnly | QFile::Append)) {
                qDebug() << "Error opening mFile in VnosObveznosti VnosUpnika writin part";
                return;
            } else {
                QString novUpnik = ui->lineEdit_dobavitelj->text().toUpper();
                while(novUpnik.at(novUpnik.length()) == ' ') {
                    novUpnik.remove(-1, 1);
                }
                QTextStream out(&mFile);
                out.setCodec("UTF-8");
                out << novUpnik << ";\n";
                mFile.flush();
                mFile.close();
            }
        }
    }
}

void VnosObveznosti::VnosObv() {
    QFile mFile(m_vnosObv);
    if(!mFile.open(QFile::WriteOnly | QFile::Append)) {
        qDebug() << "Error opening file in VnosObv";
        return;
    } else {
        QTextStream out(&mFile);
        out.setCodec("UTF-8");
        QDateTime date = QDateTime::currentDateTime();
        QString listina(ui->lineEdit_listina->text().toUpper());
        while(listina.at(listina.length() - 1) == ' ') {
            listina.remove(-1, 1);
        }
        QString dobavitelj(ui->lineEdit_dobavitelj->text().toUpper());
        while(dobavitelj.at(dobavitelj.length() - 1) == ' ') {
            dobavitelj.remove(-1, 1);
        }
        QString znesek(ui->lineEdit_znesek->text().replace(',', '.'));
        while(znesek.at(znesek.length() - 1) == ' ') {
            znesek.remove(-1, 1);
        }
        QString opombe(ui->lineEdit_opombe->text());
        if(opombe != "") {
            opombe.replace(0,1,opombe.at(0).toUpper());
            while(opombe.at(opombe.length() - 1) == ' ') {
                opombe.remove(-1, 1);
            }
        }
        QString mText = "Nova obveznost ; " +
                        date.toString("d. M. yyyy ; hh:mm:ss.zzz") +
                        " ; Listina: " + listina +
                        " ; Dobavitelj: " + dobavitelj +
                        " ; Znesek: " + znesek +
                        " ; Dat_valute: " + ui->dateEdit_valute->text() +
                        " ; Placilo: " +
                        " ; Dat_placila: " +
                        " ; Opombe: " + opombe +
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
    close();
}

void VnosObveznosti::on_lineEdit_listina_textChanged(const QString &arg1) {
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
        ui->lineEdit_listina->insert(",¸");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_listina->backspace();
    }
}

void VnosObveznosti::on_lineEdit_dobavitelj_textChanged(const QString &arg1) {
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
        ui->lineEdit_dobavitelj->insert(",¸");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_dobavitelj->backspace();
    }
}

void VnosObveznosti::on_lineEdit_znesek_textChanged(const QString &arg1) {
    if(arg1.length() == 1 && (arg1.at(arg1.length()-1) == '.' || arg1.at(arg1.length()-1) == ',')) {
        ui->lineEdit_znesek->backspace();
    }
    if(arg1.contains("..") || arg1.contains(",,") || arg1.contains(",.") || arg1.contains(".,")) {
        ui->lineEdit_znesek->backspace();
    }
}

void VnosObveznosti::on_lineEdit_opombe_textChanged(const QString &arg1) {
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
