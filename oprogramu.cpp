#include "oprogramu.h"
#include "ui_oprogramu.h"

Oprogramu::Oprogramu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Oprogramu)
{
    ui->setupUi(this);

}

Oprogramu::~Oprogramu()
{
    delete ui;
}
