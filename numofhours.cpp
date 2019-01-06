#include "numofhours.h"
#include "ui_numofhours.h"

NumOfHours::NumOfHours(QWidget *parent) :
    QDialog(parent), m_numOfHours(""), ui(new Ui::NumOfHours)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Število ur");
    ui->lineEdit->setFocus();
    QRegularExpression regex("^[.0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit->setValidator(validator);
}

NumOfHours::~NumOfHours() {
    delete ui;
}

void NumOfHours::on_pushButton_clicked() {
    close();
    if(ui->lineEdit->text() == "")
        m_numOfHours = "0";
    else
        m_numOfHours = ui->lineEdit->text();
}

void NumOfHours::reject() {
    m_numOfHours = "0";
    QDialog::reject();
}
