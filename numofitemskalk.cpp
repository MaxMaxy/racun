#include "numofitemskalk.h"
#include "ui_numofitemskalk.h"

numOfItemsKalk::numOfItemsKalk(QWidget *parent) :
    QDialog(parent), m_kolicina(""),
    ui(new Ui::numOfItemsKalk)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Vstavi količino");
    QRegularExpression regex("^[.0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_dolzina->setValidator(validator);
    ui->lineEdit_kos->setValidator(validator);
}

numOfItemsKalk::~numOfItemsKalk()
{
    delete ui;
}

void numOfItemsKalk::LineEditorState(bool dolzina) {
    if(dolzina) ui->lineEdit_kos->setDisabled(true);
    else ui->lineEdit_dolzina->setDisabled(true);
}

void numOfItemsKalk::on_pushButton_vstavi_clicked()
{
    close();
    if(ui->lineEdit_dolzina->isEnabled() && ui->lineEdit_dolzina->text() != "")
        m_kolicina = QString::number(ui->lineEdit_dolzina->text().toDouble() / 1000);
    else if(ui->lineEdit_kos->isEnabled() && ui->lineEdit_kos->text() != "")
        m_kolicina = ui->lineEdit_kos->text();
    else
        m_kolicina = "0";
}

void numOfItemsKalk::reject() {
    m_kolicina = "0";
    QDialog::reject();
}
