#include "numofitemskalk.h"
#include "ui_numofitemskalk.h"

numOfItemsKalk::numOfItemsKalk(QWidget *parent) :
    QDialog(parent), m_kolicina(""), m_kalo(""),
    ui(new Ui::numOfItemsKalk)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Vstavi količino");
    QRegularExpression regex("^[.,0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_dolzina->setValidator(validator);
    ui->lineEdit_kos->setValidator(validator);
    ui->lineEdit_kalo->setValidator(validator);
    ui->lineEdit_kalo->setText("5");
}

numOfItemsKalk::~numOfItemsKalk() {
    delete ui;
}

void numOfItemsKalk::closeEvent(QCloseEvent *) {
    emit close_me();
}

void numOfItemsKalk::LineEditorState(bool dolzina) {
    if(dolzina) ui->lineEdit_kos->setDisabled(true);
    else ui->lineEdit_dolzina->setDisabled(true);
}

void numOfItemsKalk::on_pushButton_vstavi_clicked() {
    if(ui->lineEdit_dolzina->isEnabled() && ui->lineEdit_dolzina->text() != "")
        m_kolicina = QString::number(ui->lineEdit_dolzina->text().replace(",",".").toDouble() + ((ui->lineEdit_dolzina->text().replace(",",".").toDouble() * ui->lineEdit_kalo->text().replace(",",".").toDouble())/100), 'f', 2);
    else if(ui->lineEdit_kos->isEnabled() && ui->lineEdit_kos->text() != "")
        m_kolicina = QString::number((ui->lineEdit_kos->text().replace(",",".").toDouble() + ((ui->lineEdit_kos->text().replace(",",".").toDouble() * ui->lineEdit_kalo->text().replace(",",".").toDouble())/100)) * 1000, 'f', 2);
    else
        m_kolicina = "0";
    m_kalo = ui->lineEdit_kalo->text();
    close();
}

void numOfItemsKalk::reject() {
    m_kolicina = "0";
    QDialog::reject();
}
