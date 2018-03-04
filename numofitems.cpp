#include "numofitems.h"
#include "ui_numofitems.h"

NumOfItems::NumOfItems(QWidget *parent) :
    QDialog(parent),
    m_numKosov("1"), ui(new Ui::NumOfItems)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    ui->lineEdit_steviloKosov->setValidator(new QIntValidator(0, 1000000, this));
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
