#include "numofitems.h"
#include "ui_numofitems.h"

NumOfItems::NumOfItems(QWidget *parent) :
    QDialog(parent),
    m_numKosov("1"), ui(new Ui::NumOfItems)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Število kosov");
    ui->lineEdit_steviloKosov->setFocus();
    QRegularExpression regex("^[0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_steviloKosov->setValidator(validator);
}

NumOfItems::~NumOfItems()
{
    delete ui;
}

void NumOfItems::on_pushButton_vnos_clicked()
{
    close();
    if(ui->lineEdit_steviloKosov->text() == "")
        m_numKosov = "1";
    else
        m_numKosov = ui->lineEdit_steviloKosov->text();
}

void NumOfItems::reject() {
    m_numKosov = "0";
    QDialog::reject();
}
