#include "dodajprodukt.h"
#include "ui_dodajprodukt.h"

DodajProdukt::DodajProdukt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DodajProdukt)
{
    ui->setupUi(this);
    this->setWindowTitle("Dodaj produkt");
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->comboBox_stevilozil->insertItem(0, "1");
}

DodajProdukt::~DodajProdukt()
{
    delete ui;
}

void DodajProdukt::on_pushButton_clicked()
{
    QFile file("nalepke.txt");

    if(!file.open(QFile::WriteOnly | QFile::Append))
        qDebug() << "ERORRRRR";

    QTextStream out(&file);
    out.setCodec("UTF-8");

    QString naziv = ui->lineEdit->text().toUpper();
    while(naziv.at(naziv.length() - 1) == ' ')
        naziv.remove(-1, 1);
    //naziv += ui->tableWidget->itemAt()


    //out << m_id << ";" << m_naziv << "\n";
    file.flush();
    file.close();

}

void DodajProdukt::on_comboBox_vrstaProdukta_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->comboBox_stevilozil->clear();
        ui->comboBox_stevilozil->insertItem(0, "1");
    }
    else
    {
        ui->comboBox_stevilozil->clear();
        for(int i(0); i < 20; i++)
            ui->comboBox_stevilozil->insertItem(i, QString ::number(i + 1));
    }
}

void DodajProdukt::on_comboBox_stevilozil_currentIndexChanged(int index)
{
    if(index == 0)
        ui->tableWidget->insertRow(0);
    else
    {
        ui->tableWidget->removeRow(0)
        for(int i(0); i <= index; i++)
            ui->tableWidget->insertRow(i);
    }
}
