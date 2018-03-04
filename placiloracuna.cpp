#include "placiloracuna.h"
#include "ui_placiloracuna.h"

PlaciloRacuna::PlaciloRacuna(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaciloRacuna)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    ui->dateEdit->setDisplayFormat("d. M. yyyy");
    ui->dateEdit->setDate(QDate::currentDate());
}

PlaciloRacuna::~PlaciloRacuna()
{
    delete ui;
}

void PlaciloRacuna::setOpombe(QString &opomba, QString &cena, QString &datum)
{
    ui->lineEdit_opombe->setText(opomba);
    ui->lineEdit->setText(cena);
    ui->dateEdit->setDate(QDate::fromString(datum));
}

QStringList PlaciloRacuna::on_pushButton_clicked()
{
    m_list.append(ui->lineEdit->text());
    m_list.append(ui->dateEdit->text());
    m_list.append(ui->lineEdit_opombe->text());
    PlaciloRacuna::close();
    return m_list;
}