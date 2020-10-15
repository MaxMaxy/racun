#include "proizvodniproces.h"
#include "ui_proizvodniproces.h"

ProizvodniProces::ProizvodniProces(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProizvodniProces)
{
    ui->setupUi(this);
    this->setWindowTitle("Proizvodni proces");
    ui->treeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(6, QHeaderView::Stretch);
    QFont font("Arial", 20, QFont::Bold);
    ui->nazivProdukta->setFont(font);
}

ProizvodniProces::~ProizvodniProces()
{
    delete ui;
}

// ISKALNIK
void ProizvodniProces::Search(const QString& naziv)
{
    QFile file("proizvodniproces.txt");

    if(!file.open(QFile::Text | QFile::ReadOnly))
        qDebug() << "Errorrrrrrr";

    QTextStream out(&file);
    out.setCodec("UTF-8");
    while(!out.atEnd())
    {
        m_searchLine = out.readLine();
        if((m_searchLine.contains(naziv, Qt::CaseInsensitive) && naziv != ""))
        {
            if(m_searchLine == "Proizvodni proces:")
                continue;
            if(m_searchLine.contains("DELETE"))
                continue;
            else
            {
                m_searchList = m_searchLine.split(';', QString::SkipEmptyParts);
                if(m_searchList.at(0) == "S")
                {
                    ui->nazivProdukta->setText("VODNIK\n" + m_searchList.at(1));
                    QTreeWidgetItem* itm = new QTreeWidgetItem();
                    AddRootToTreeWidget(m_searchList, itm);
                }
                else if(m_searchList.at(0) == "M")
                {
                    ui->nazivProdukta->setText("KABEL / VERIGA / SNOP\n" + m_searchList.at(1));
                    int index(0);
                    m_searchList = m_searchLine.split('&', QString::SkipEmptyParts);
                    QStringList partList;
                    QString tmp = m_searchList.at(index);
                    partList = tmp.split(";", QString::SkipEmptyParts);
                    while(tmp != "***")
                    {
                        QTreeWidgetItem* itm = new QTreeWidgetItem();
                        if(partList.length() > 7)
                        {
                            partList.removeAt(0);
                            partList.removeAt(0);
                        }
                        AddRootToTreeWidget(partList, itm);
                        index++;
                        tmp = m_searchList.at(index);
                        partList = tmp.split(";", QString::SkipEmptyParts);
                    }
                }
            }
        }
    }
    file.close();
}

// ITEM V TREEWIDGET
void ProizvodniProces::AddRootToTreeWidget(const QStringList& list, QTreeWidgetItem* itm)
{
    itm->setText(0, list.at(0));
    itm->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);
    itm->setText(1, list.at(1));
    itm->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
    itm->setText(2, list.at(2));
    itm->setTextAlignment(2, Qt::AlignHCenter | Qt::AlignVCenter);
    itm->setText(3, list.at(3));
    itm->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);
    itm->setText(4, list.at(4));
    itm->setTextAlignment(4, Qt::AlignHCenter | Qt::AlignVCenter);
    itm->setText(5, list.at(5));
    itm->setTextAlignment(5, Qt::AlignHCenter | Qt::AlignVCenter);
    itm->setText(6, list.at(6));
    itm->setTextAlignment(6, Qt::AlignHCenter | Qt::AlignVCenter);
    ui->treeWidget->addTopLevelItem(itm);
}

void ProizvodniProces::on_lineEdit_isciProdukt_textChanged(const QString &arg1)
{
    ui->treeWidget->clear();
    ui->nazivProdukta->clear();
    if(arg1.length() > 6)
        Search(arg1);
}
