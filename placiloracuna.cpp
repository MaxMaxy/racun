#include "placiloracuna.h"
#include "ui_placiloracuna.h"

PlaciloRacuna::PlaciloRacuna(QWidget *parent) :
    QDialog(parent), deleteRacun(false), m_quit(false),
    ui(new Ui::PlaciloRacuna), m_obveznost(false)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    ui->dateEdit->setDisplayFormat("d. M. yyyy");
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEdit->setFocus();
    this->setWindowTitle("Plačilo");
    this->setWindowFlags(Qt::Window);
    QRegularExpression regex("^[.,0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit->setValidator(validator);
    ui->lineEdit_TrenutnoPlacilo->setValidator(validator);
    QRegularExpression regealfabet("^[a-zA-Z0-9,@. -/&#čšžŠČŽ()+=]*$");
    QValidator *validatoralfabet = new QRegularExpressionValidator(regealfabet, this);
    ui->lineEdit_opombe->setValidator(validatoralfabet);
    ui->radioButton_placiloSkupaj->setChecked(true);
    ui->lineEdit_TrenutnoPlacilo->setEnabled(false);
}

PlaciloRacuna::~PlaciloRacuna() {
    delete ui;
}

void PlaciloRacuna::closeEvent(QCloseEvent *) {
    emit close_me();
}

void PlaciloRacuna::setOpombe(QString &opomba, QString &cena, QString &datum, QString &terObv, bool obveznost) {
    ui->label_terObv->setText("€" + terObv);
    ui->lineEdit_opombe->setText(opomba);
    ui->lineEdit->setText(cena.replace(',','.'));
    ui->dateEdit->setDate(QDate::fromString(datum));
    if(!obveznost)
        m_obveznost = true;
}

QStringList PlaciloRacuna::on_pushButton_clicked() {
    QString opomba(ui->lineEdit_opombe->text());
    if(opomba != "") {
        opomba.replace(0,1,opomba.at(0).toUpper());
        while(opomba.at(opomba.length() - 1) == ' ') {
            opomba.remove(-1, 1);
        }
    }
    if(ui->radioButton_placiloEnkratno->isChecked()) {
        double cena = ui->lineEdit->text().remove("€").toDouble() + ui->lineEdit_TrenutnoPlacilo->text().replace(",",".").toDouble();
        m_list.append(QString::number(cena, 'f', 2));
    }
    else if(ui->radioButton_placiloSkupaj->isChecked()) {
        m_list.append(QString::number(ui->lineEdit->text().replace(',','.').toDouble(), 'f', 2));
    }
    m_list.append(ui->dateEdit->text());
    m_list.append(opomba);
    close();
    m_quit = false;
    return m_list;
}

void PlaciloRacuna::on_lineEdit_textChanged(const QString &arg1) {
    if(arg1.length() == 1 && (arg1.at(arg1.length()-1) == '.' || arg1.at(arg1.length()-1) == ',')) {
        ui->lineEdit->backspace();
    }
    if(arg1.contains("..") || arg1.contains(",,") || arg1.contains(",.") || arg1.contains(".,")) {
        ui->lineEdit->backspace();
    }
}

void PlaciloRacuna::on_lineEdit_opombe_textChanged(const QString &arg1) {
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

void PlaciloRacuna::on_pushButton_izbris_clicked() {
    if(m_obveznost) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("IZBRIS RAČUNA");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setBaseSize(QSize(250, 100));
        msgBox.setText(tr("Želiš izbrisati račun?"));
        QAbstractButton* pButtonYes = msgBox.addButton(tr("IZBRIS RAČUNA!"), QMessageBox::YesRole);
        msgBox.addButton(tr("Izhod"), QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton()==pButtonYes) {
            deleteRacun = true;
            PlaciloRacuna::close();
            m_quit = false;
        } else {
            PlaciloRacuna::close();
            m_quit = false;
        }
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("IZBRIS TERJATVE");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setBaseSize(QSize(250, 100));
        msgBox.setText(tr("Želiš izbrisati terjatev?"));
        QAbstractButton* pButtonYes = msgBox.addButton(tr("IZBRIS TERJATVE!"), QMessageBox::YesRole);
        msgBox.addButton(tr("Izhod"), QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton()==pButtonYes) {
            deleteRacun = true;
            PlaciloRacuna::close();
            m_quit = false;
        } else {
            PlaciloRacuna::close();
            m_quit = false;
        }
    }
    close();
}

void PlaciloRacuna::reject() {
    m_quit = true;
    QDialog::reject();
}

void PlaciloRacuna::on_radioButton_placiloSkupaj_clicked()
{
    ui->lineEdit->setEnabled(true);
    ui->lineEdit_TrenutnoPlacilo->setEnabled(false);
}

void PlaciloRacuna::on_radioButton_placiloEnkratno_clicked()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_TrenutnoPlacilo->setEnabled(true);
}

void PlaciloRacuna::on_pushButton_2_clicked() {
    close();
}
