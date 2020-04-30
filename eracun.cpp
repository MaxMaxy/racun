#include "eracun.h"
#include "ui_eracun.h"

eRacun::eRacun(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eRacun)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("eRačun");
    this->setWindowFlags(Qt::Window);
    ui->lineEdit_nazivStranke->setFocus();
}

eRacun::~eRacun()
{
    delete ui;
}

void eRacun::setEditLines(QString m_cNaziv, QString m_naslov, QString m_posta, QString m_drzavaStranke, QString m_kodaDrzave, QString m_bancniRacunStranke, QString m_bicStranke, QString m_vatStranke) {
    ui->lineEdit_nazivStranke->setText(m_cNaziv);
    ui->lineEdit_naslovStranke->setText(m_naslov);
    QString tmp(m_posta);
    ui->lineEdit_postnaStStranke->setText(tmp.remove(4,m_posta.length()));
    ui->lineEdit_postaStranke->setText(m_posta.remove(0,5));
    ui->lineEdit_drzavaStranke->setText(m_drzavaStranke);
    ui->lineEdit_kodaDrzaveStranke->setText(m_kodaDrzave);
    ui->lineEdit_bancniRacunStranke->setText(m_bancniRacunStranke);
    ui->lineEdit_bicStranke->setText(m_bicStranke);
    ui->lineEdit_vatStranke->setText(m_vatStranke);
}

void eRacun::closeEvent(QCloseEvent *) {
    emit close_me();
}

void eRacun::on_pushButton_vnos_clicked() {
    m_cNaziv = ui->lineEdit_nazivStranke->text();
    m_naslov = ui->lineEdit_naslovStranke->text();
    m_posta = ui->lineEdit_postaStranke->text();
    m_drzavaStranke = ui->lineEdit_drzavaStranke->text();
    m_kodaDrzave = ui->lineEdit_kodaDrzaveStranke->text();
    m_bancniRacunStranke = ui->lineEdit_bancniRacunStranke->text();
    m_bicStranke = ui->lineEdit_bicStranke->text();
    m_vatStranke = ui->lineEdit_vatStranke->text();
    m_stevilkaPoste = ui->lineEdit_postnaStStranke->text();
    close();
}
