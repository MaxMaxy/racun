#include "statistic.h"
#include "ui_statistic.h"

Statistic::Statistic(QWidget *parent) :
    QDialog(parent), ui(new Ui::Statistic), m_currentDir(QDir::currentPath()) , m_fileNameCompanys(m_currentDir + "/arhiv_stranke.txt"), m_fileRacun(m_currentDir + "/arhiv_novRacun.txt"), m_numInCombo(1)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Statistika");
    QHeaderView *h_header_brez = ui->tableWidget_brez->horizontalHeader();
    QHeaderView *v_header_brez = ui->tableWidget_brez->verticalHeader();
    h_header_brez->setSectionResizeMode(QHeaderView::Stretch);
    v_header_brez->setSectionResizeMode(QHeaderView::Stretch);
    QHeaderView *h_header_vse = ui->tableWidget_vse->horizontalHeader();
    QHeaderView *v_header_vse = ui->tableWidget_vse->verticalHeader();
    h_header_vse->setSectionResizeMode(QHeaderView::Stretch);
    v_header_vse->setSectionResizeMode(QHeaderView::Stretch);
    AddToComboBox(m_fileNameCompanys);
    AddToTableWidget(m_fileRacun);
    ui->tableWidget_vse->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_brez->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->plot->clearPlottables();
    Plot();
}

Statistic::~Statistic()
{
    delete ui;
}

void Statistic::AddToComboBox(QString mFileName)
{
    ui->comboBox_podjetja->addItem("Vse terjatve");
    QFile mFile(mFileName);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in AddToComboBox() in statistic.cpp";
        return;
    }
    else
    {
        QTextStream mText(&mFile);
        QString mLine("");
        QStringList mList;
        QRegularExpression exp(" ; ");
        while(!mText.atEnd())
        {
            mLine = mText.readLine();
            mList = mLine.split(exp, QString::SkipEmptyParts);
            mLine = mList.at(3);
            ui->comboBox_podjetja->addItem(mLine);
            m_numInCombo++;
        }
        mFile.close();
    }
}

void Statistic::AddToTableWidget(QString fileName)
{
    for(int row(0); row < 12; row++)
    {
        for(int col(0); col < 3; col++)
        {
            QTableWidgetItem *itm = new QTableWidgetItem();
            itm->setText("");
            QTableWidgetItem *itm_sec = new QTableWidgetItem();
            itm_sec->setText("");
            ui->tableWidget_vse->setItem(row,col,itm);
            ui->tableWidget_brez->setItem(row,col,itm_sec);
        }
    }
    QFile mFile(fileName);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in AddToTableWidget() in statistic.cpp";
        return;
    }
    else
    {
        QTextStream mText(&mFile);
        QString mLine("");
        QStringList mList;
        QRegularExpression exp(" ; ");
        QRegularExpression exp_dot(". ");
        QStringList tmpList;
        double mSumSkupaj(0);
        double mSumDDV(0);
        double mSumOsnova(0);
        QString num("");
        QString tmp("");
        while(!mText.atEnd())
        {
            QTableWidgetItem *itm_setSkupaj = new QTableWidgetItem();
            QTableWidgetItem *itm_setDDV = new QTableWidgetItem();
            QTableWidgetItem *itm_setOsnova = new QTableWidgetItem();

            QTableWidgetItem *itm_setSkupajBrezIF = new QTableWidgetItem();
            QTableWidgetItem *itm_setDDVBrezIF = new QTableWidgetItem();
            QTableWidgetItem *itm_setOsnovaBrezIF = new QTableWidgetItem();

            mLine = mText.readLine();
            mList = mLine.split(exp, QString::SkipEmptyParts);
            if(ui->comboBox_podjetja->currentText() == "Vse terjatve")
            {
                tmp = mList.at(5);
                tmp.remove("Dat_izdaje: ");
                tmp.remove(-6,6);
                tmpList = tmp.split(exp_dot, QString::SkipEmptyParts);
                tmp = tmpList.at(1);

                switch (tmp.toInt()) {
                case 1:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(0,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(0,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(0,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(0,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(0,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(0,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(0,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(0,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(0,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(0,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(0,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(0,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(0,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(0,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(0,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(0,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(0,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(0,2,itm_setSkupajBrezIF);
                    break;
                }
                case 2:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(1,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(1,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(1,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(1,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(1,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(1,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(1,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(1,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(1,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(1,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(1,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(1,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(1,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(1,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(1,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(1,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(1,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(1,2,itm_setSkupajBrezIF);
                    break;
                }
                case 3:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(2,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(2,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(2,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(2,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(2,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(2,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(2,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(2,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));

                        ui->tableWidget_brez->setItem(2,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(2,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(2,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(2,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(2,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(2,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(2,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(2,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(2,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(2,2,itm_setSkupajBrezIF);
                    break;
                }
                case 4:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(3,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(3,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(3,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(3,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(3,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(3,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(3,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(3,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(3,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(3,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(3,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(3,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(3,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(3,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(3,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(3,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(3,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(3,2,itm_setSkupajBrezIF);
                    break;
                }
                case 5:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(4,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(4,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(4,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(4,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(4,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(4,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(4,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(4,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(4,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(4,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(4,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(4,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(4,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(4,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(4,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(4,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(4,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(4,2,itm_setSkupajBrezIF);
                    break;
                }
                case 6:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(5,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(5,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(5,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(5,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(5,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(5,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(5,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(5,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(5,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(5,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(5,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(5,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(5,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(5,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(5,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(5,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(5,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(5,2,itm_setSkupajBrezIF);
                    break;
                }
                case 7:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(6,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(6,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(6,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(6,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(6,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(6,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(6,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(6,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(6,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(6,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(6,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(6,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(6,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(6,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(6,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(6,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(6,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(6,2,itm_setSkupajBrezIF);
                    break;
                }
                case 8:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(7,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(7,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(7,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(7,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(7,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(7,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(7,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(7,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(7,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(7,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(7,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(7,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(7,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(7,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(7,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(7,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(7,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(7,2,itm_setSkupajBrezIF);
                    break;
                }
                case 9:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(8,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(8,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(8,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(8,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(8,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(8,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(8,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(8,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(8,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(8,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(8,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(8,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(8,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(8,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(8,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(8,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(8,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(8,2,itm_setSkupajBrezIF);
                    break;
                }
                case 10:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(9,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(9,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(9,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(9,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(9,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(9,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(9,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(9,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(9,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(9,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(9,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(9,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(9,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(9,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(9,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(9,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(9,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(9,2,itm_setSkupajBrezIF);
                    break;
                }
                case 11:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(10,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(10,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(10,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(10,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(10,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(10,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(10,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(10,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(10,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(10,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(10,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(10,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(10,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(10,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(10,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(10,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(10,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(10,2,itm_setSkupajBrezIF);
                    break;
                }
                case 12:
                {
                    // OSNOVA
                    tmp = mList.at(9);
                    tmp.remove("Sum_osnova: ");
                    tmp.remove(-1,1);
                    num = ui->tableWidget_vse->item(11,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumOsnova += tmp.toDouble();
                    itm_setOsnova->setText(QString::number(mSumOsnova) + " €");
                    ui->tableWidget_vse->setItem(11,0,itm_setOsnova);

                    // DDV
                    num = ui->tableWidget_vse->item(11,0)->text();
                    num.remove(-1,2);
                    mSumOsnova = num.toDouble();
                    mSumDDV = mSumOsnova * 0.22;
                    itm_setDDV->setText(QString::number(mSumDDV) + " €");
                    ui->tableWidget_vse->setItem(11,1,itm_setDDV);

                    // SKUPAJ
                    num = ui->tableWidget_vse->item(11,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_vse->item(11,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    itm_setSkupaj->setText(QString::number(mSumSkupaj) + " €");
                    ui->tableWidget_vse->setItem(11,2,itm_setSkupaj);

                    // OSNOVA BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(11,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(11,0,itm_setOsnovaBrezIF);
                    }
                    else
                    {
                        tmp = mList.at(9);
                        tmp.remove("Sum_osnova: ");
                        tmp.remove(-1,1);
                        num = ui->tableWidget_brez->item(11,0)->text();
                        num.remove(-1,2);
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnovaBrezIF->setText(QString::number(mSumOsnova) + " €");
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(11,0,itm_setOsnovaBrezIF);
                    }

                    // DDV BREZ
                    tmp = mList.at(14);
                    tmp.remove("Placilo: ");
                    if(tmp != "")
                    {
                        num = ui->tableWidget_brez->item(11,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        mSumDDV = num.toDouble() - mSumDDV;
                        if(mSumDDV < 0)
                            mSumSkupaj = 0;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(11,1,itm_setDDVBrezIF);
                    }
                    else
                    {
                        num = ui->tableWidget_brez->item(11,0)->text();
                        num.remove(-1,2);
                        mSumDDV = num.toDouble() * 0.22;
                        itm_setDDVBrezIF->setText(QString::number(mSumDDV) + " €");
                        if(mSumDDV > 0)
                            itm_setDDVBrezIF->setTextColor(QColor("green"));
                        ui->tableWidget_brez->setItem(11,1,itm_setDDVBrezIF);
                    }

                    // SKUPAJ BREZ
                    num = ui->tableWidget_brez->item(11,0)->text();
                    num.remove(-1,2);
                    tmp = ui->tableWidget_brez->item(11,1)->text();
                    tmp.remove(-1,2);
                    mSumSkupaj = num.toDouble() + tmp.toDouble();
                    if(mSumSkupaj < 0)
                        mSumSkupaj = 0;
                    itm_setSkupajBrezIF->setText(QString::number(mSumSkupaj) + " €");
                    if(mSumSkupaj > 0)
                        itm_setSkupajBrezIF->setTextColor(QColor("green"));
                    ui->tableWidget_brez->setItem(11,2,itm_setSkupajBrezIF);
                    break;
                }
                default:
                    break;
                }
            }
            else
            {
                qDebug() << "error";
            }
        }
    }
}

void Statistic::Plot()
{
    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(180, 180, 180));
    gradient.setColorAt(0.38, QColor(200, 200, 200));
    gradient.setColorAt(1, QColor(130, 130, 130));
    ui->plot->setBackground(QBrush(gradient));

    // set all companys name and color from comboBox
    QCPBars *podjetja[m_numInCombo];
    int col_num;
    int col_sec;
    int col_thr;
    // create empty bar chart objects:
    for(int itr(0); itr < m_numInCombo; itr++)
    {
        podjetja[itr] = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
        podjetja[itr]->setAntialiased(false); // gives more crisp, pixel aligned bar borders
        podjetja[itr]->setStackingGap(1);
        // set names and colors:
        podjetja[itr]->setName(ui->comboBox_podjetja->itemText(itr));
        col_num = qrand() % 255;
        col_sec = (qrand() % 255);
        col_thr = (qrand() % 50);
        podjetja[itr]->setPen(QPen(QColor(col_num, col_sec, col_thr).lighter(130)));
        podjetja[itr]->setBrush(QColor(col_thr, col_sec, col_num));
        // stack bars on top of each other:
        if(itr == 0)
            continue;
        else
            podjetja[itr]->moveAbove(podjetja[itr-1]);
    }

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    labels << "Januar" << "Februar" << "Marec" << "April" << "Maj" << "Junij" << "Julij" << "Avgust" << "September" << "Oktober" << "November" << "December";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->plot->xAxis->setTicker(textTicker);
    ui->plot->xAxis->setTickLabelRotation(60);
    ui->plot->xAxis->setSubTicks(false);
    ui->plot->xAxis->setTickLength(0, 4);
    ui->plot->xAxis->setRange(0, 10);
    ui->plot->xAxis->setBasePen(QPen(Qt::black));
    ui->plot->xAxis->setTickPen(QPen(Qt::black));
    ui->plot->xAxis->grid()->setVisible(true);
    ui->plot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->plot->xAxis->setTickLabelColor(Qt::black);
    ui->plot->xAxis->setLabelColor(Qt::black);

    // prepare y axis:
    ui->plot->yAxis->setRange(0, 15000);
    ui->plot->yAxis->setPadding(5); // a bit more space to the left border
    ui->plot->yAxis->setLabel("Promet po mesecih\nOSNOVA");
    ui->plot->yAxis->setBasePen(QPen(Qt::black));
    ui->plot->yAxis->setTickPen(QPen(Qt::black));
    ui->plot->yAxis->setSubTickPen(QPen(Qt::black));
    ui->plot->yAxis->grid()->setSubGridVisible(true);
    ui->plot->yAxis->setTickLabelColor(Qt::black);
    ui->plot->yAxis->setLabelColor(Qt::black);
    ui->plot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->plot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> allData[m_numInCombo];
    if(ui->comboBox_podjetja->itemText(ui->comboBox_podjetja->currentIndex()) == "Vse terjatve")
    {
        for(int itr(1); itr < m_numInCombo; itr++)
        {
            GetData(allData, itr, "Vse terjatve");
            podjetja[itr]->setData(ticks, allData[itr]);
        }
    }
    else
    {
        int itr = ui->comboBox_podjetja->currentIndex();
        QString name = ui->comboBox_podjetja->currentText();
        GetData(allData, itr, name);
        podjetja[itr]->setData(ticks, allData[itr]);
        for(int i(1); i < m_numInCombo ; i++)
        {
            if(i == itr)
                continue;
            else
                podjetja[i]->removeFromLegend();
        }
    }
    podjetja[0]->removeFromLegend();

    // setup legend:
    ui->plot->legend->setVisible(true);
    ui->plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->plot->legend->setBrush(QColor(255, 255, 255, 100));
    ui->plot->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->plot->legend->setFont(legendFont);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->plot->replot();
    ui->plot->update();
}

void Statistic::GetData(QVector<double> *allData, int itr, QString name)
{
    QFile mFile(m_fileRacun);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in GetData in statistic.cpp";
        return;
    }
    else
    {
        if(name == "Vse terjatve")
        {
            QTextStream in(&mFile);
            QString lineText("");
            QString tmp("");
            QStringList listText;
            QStringList tmpList;
            QRegularExpression expDot(". ");
            QRegularExpression exp(" ; ");
            double sumPerMonth(0);
            for(int itr_month(0); itr_month < 12; itr_month++)
            {
                while(!in.atEnd())
                {
                    lineText = in.readLine();
                    listText = lineText.split(exp, QString::SkipEmptyParts);
                    tmp = listText.at(7);
                    tmp.remove("Narocnik: ");
                    if(ui->comboBox_podjetja->itemText(itr) == tmp)
                    {
                        tmp = listText.at(5);
                        tmp.remove("Dat_izdaje: ");
                        tmpList = tmp.split(expDot, QString::SkipEmptyParts);
                        tmp = tmpList.at(1);
                        if(tmp.toInt() == (itr_month+1))
                        {
                            tmp = listText.at(9);
                            tmp.remove("Sum_osnova: ");
                            tmp.remove(-1,1);
                            sumPerMonth += tmp.toDouble();
                        }
                    }
                }
                in.seek(0);
                allData[itr].append(sumPerMonth);
                sumPerMonth = 0;
            }
        }
        else
        {
            QTextStream in(&mFile);
            QString lineText("");
            QString tmp("");
            QStringList listText;
            QStringList tmpList;
            QRegularExpression expDot(". ");
            QRegularExpression exp(" ; ");
            double sumPerMonth(0);
            for(int itr_month(0); itr_month < 12; itr_month++)
            {
                while(!in.atEnd())
                {
                    lineText = in.readLine();
                    listText = lineText.split(exp, QString::SkipEmptyParts);
                    tmp = listText.at(7);
                    tmp.remove("Narocnik: ");
                    if(name == tmp)
                    {
                        tmp = listText.at(5);
                        tmp.remove("Dat_izdaje: ");
                        tmpList = tmp.split(expDot, QString::SkipEmptyParts);
                        tmp = tmpList.at(1);
                        if(tmp.toInt() == (itr_month+1))
                        {
                            tmp = listText.at(9);
                            tmp.remove("Sum_osnova: ");
                            tmp.remove(-1,1);
                            sumPerMonth += tmp.toDouble();
                        }
                    }
                }
                in.seek(0);
                allData[itr].append(sumPerMonth);
                sumPerMonth = 0;
            }
        }
        mFile.close();
    }
}

void Statistic::on_pushButton_izpis_clicked()
{

}

void Statistic::on_comboBox_podjetja_currentTextChanged()
{
    ui->plot->clearPlottables();
    Plot();
}
