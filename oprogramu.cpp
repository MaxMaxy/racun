#include "oprogramu.h"
#include "ui_oprogramu.h"

Oprogramu::Oprogramu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Oprogramu)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
}

Oprogramu::~Oprogramu()
{
    delete ui;
}
