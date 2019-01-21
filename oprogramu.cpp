#include "oprogramu.h"
#include "ui_oprogramu.h"

Oprogramu::Oprogramu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Oprogramu)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("O programu");
}

Oprogramu::~Oprogramu()
{
    delete ui;
}

void Oprogramu::closeEvent(QCloseEvent *) {
    emit close_me();
}
