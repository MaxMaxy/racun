#include "numofitems.h"
#include "ui_numofitems.h"

NumOfItems::NumOfItems(QWidget *parent) :
    QDialog(parent),
    m_numKosov("0"), m_popust("0"), m_kronoterm(false), ui(new Ui::NumOfItems)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Število kosov");
    ui->lineEdit_steviloKosov->setFocus();
    QRegularExpression regex("^[,.0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_steviloKosov->setValidator(validator);
    ui->lineEdit->setValidator(validator);
    ui->lineEdit->setText(m_popust);
}

NumOfItems::~NumOfItems() {
    delete ui;
}

void NumOfItems::closeEvent(QCloseEvent *) {
    emit close_me();
}

void NumOfItems::on_pushButton_vnos_clicked() {
    if(ui->lineEdit_steviloKosov->text() == "") {
        m_numKosov = "1";
        m_popust = ui->lineEdit->text();
    }
    else {
        m_numKosov = ui->lineEdit_steviloKosov->text().replace(",",".");
        m_popust = ui->lineEdit->text().replace(",",".");
    }
    close();
}

void NumOfItems::reject() {
    m_numKosov = "0";
    m_popust = "0";
    QDialog::reject();
}

void NumOfItems::on_lineEdit_steviloKosov_textChanged(const QString &arg1) {
    if(m_kronoterm) {
        if(arg1.toDouble() >= 1000.0) {
            ui->lineEdit->setText("5");
        }
        else if(arg1.toDouble() >= 100.0)
            ui->lineEdit->setText("3");
        else {
            ui->lineEdit->setText("0");
        }
    }
}
