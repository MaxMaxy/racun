#include "statistikaobveznosti.h"
#include "ui_statistikaobveznosti.h"

StatistikaObveznosti::StatistikaObveznosti(QWidget *parent) :
    QDialog(parent), ui(new Ui::StatistikaObveznosti), m_currentDir(QDir::currentPath()) , m_fileNameCompanys(m_currentDir + "/arhiv_upnikiSeznam.txt"),
    m_fileRacun(m_currentDir + "/arhiv_upniki.txt"), m_upnikiSeznam(m_currentDir + "/arhiv_upnikiSeznam.txt"), m_numInCombo(1), m_show_child(false)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Statistika obveznosti");
    this->setWindowFlags(Qt::Window);
    this->showMaximized();
    int color(0);
    for(int i(0); i < 150; i++) {
        color = qrand() % 255;
        m_colorArray[i] = color;
    }
    QHeaderView *h_header_brez = ui->tableWidget_brez->horizontalHeader();
    QHeaderView *v_header_brez = ui->tableWidget_brez->verticalHeader();
    ui->dateEdit->setDisplayFormat("yyyy");
    ui->dateEdit->setMinimumDate(QDate(2018,1,1));
    ui->dateEdit->setDate(QDate(QDate::currentDate()));
    h_header_brez->setSectionResizeMode(QHeaderView::Stretch);
    v_header_brez->setSectionResizeMode(QHeaderView::Stretch);
    v_header_brez->setDefaultSectionSize(21);
    QHeaderView *h_header_vse = ui->tableWidget_vse->horizontalHeader();
    QHeaderView *v_header_vse = ui->tableWidget_vse->verticalHeader();
    h_header_vse->setSectionResizeMode(QHeaderView::Stretch);
    v_header_vse->setSectionResizeMode(QHeaderView::Stretch);
    v_header_vse->setDefaultSectionSize(21);
    AddToComboBox(m_fileNameCompanys);
    AddToTableWidget(m_fileRacun);
    ui->tableWidget_vse->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_brez->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->plot->legend->setWrap(5);
    ui->plot->clearPlottables();
    Plot();
    ui->pushButton_isci->setFocus();
}

StatistikaObveznosti::~StatistikaObveznosti()
{
    delete ui;
}

void StatistikaObveznosti::CloseChild() {
    m_show_child = false;
}

void StatistikaObveznosti::closeEvent(QCloseEvent *) {
    emit close_me();
}

void StatistikaObveznosti::AddToComboBox(QString mFileName) {
    // OBVEZNOSTI
    ui->comboBox_podjetja->clear();
    ui->comboBox_podjetja->addItem(QString::fromUtf8("Vse obveznosti"));
    QFile mFileUpniki(mFileName);
    if(!mFileUpniki.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in Terjatve AddItemsToComboBox";
        return;
    } else {
        QTextStream in(&mFileUpniki);
        in.setCodec("UTF-8");
        QString mText("");
        QRegularExpression exp(";");
        QStringList list;
        while(!in.atEnd()) {
            mText = in.readLine();
            list = mText.split(exp, QString::SkipEmptyParts);
            ui->comboBox_podjetja->addItems(list);
            m_numInCombo++;
        }
    }
    mFileUpniki.close();
}

void StatistikaObveznosti::AddToTableWidget(QString fileName) {
    for(int row(0); row < 12; row++) {
        for(int col(0); col < 3; col++) {
            QTableWidgetItem *itm = new QTableWidgetItem();
            itm->setText("");
            QTableWidgetItem *itm_sec = new QTableWidgetItem();
            itm_sec->setText("");
            ui->tableWidget_vse->setItem(row,col,itm);
            ui->tableWidget_brez->setItem(row,col,itm_sec);
        }
    }
    QFile mFile(fileName);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in AddToTableWidget() in statistic.cpp";
        return;
    } else {
        QTextStream mText(&mFile);
        mText.setCodec("UTF-8");
        QString mLine("");
        QStringList mList;
        QRegularExpression exp(" ; ");
        QRegularExpression exp_dot(". ");
        QStringList tmpList;
        double mSumOsnova(0);
        QString date = QString::number(ui->dateEdit->date().year());
        QString num("");
        QString tmp("");
        QString opomba("");
        while(!mText.atEnd()) {
            QTableWidgetItem *itm_setOsnova = new QTableWidgetItem();
            itm_setOsnova->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *itm_setOsnovaBrez = new QTableWidgetItem();
            itm_setOsnovaBrez->setTextAlignment(Qt::AlignCenter);
            mLine = mText.readLine();
            mList = mLine.split(exp, QString::SkipEmptyParts);
            tmp = mList.at(4);
            tmp.remove("Dobavitelj: ");
            opomba = mList.at(9);
            opomba.remove("Opombe: ");
            if(opomba == "terjatev_je_bila_odstranjena!!!????") {
                continue;
            }
            if(ui->comboBox_podjetja->currentIndex() == 0)
            {
                tmp = mList.at(6);
                tmp.remove("Dat_valute: ");
                if(tmp.endsWith(date)) {
                    tmp.remove(-6,6);
                    tmpList = tmp.split(exp_dot, QString::SkipEmptyParts);
                    tmp = tmpList.at(1);
                    switch (tmp.toInt()) {
                        case 1:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(0,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(0,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(0,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 2:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(1,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(1,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(1,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 3:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(2,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(2,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(2,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 4:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(3,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(3,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(3,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 5:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(4,0)->text();
                        ui->tableWidget_brez->setItem(0,0,itm_setOsnova);
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(4,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(4,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 6:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(5,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(5,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(5,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 7:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(6,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(6,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(6,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 8:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(7,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(7,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(7,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 9:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(8,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(8,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(8,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 10:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(9,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(9,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(9,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 11:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(10,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(10,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(10,0,itm_setOsnovaBrez);
                        break;
                    }
                        case 12:
                        {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(11,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(11,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(11,0,itm_setOsnovaBrez);
                        break;
                    }
                        default:
                            break;
                    }
                } else {
                    continue;
                }
            } else if(tmp == ui->comboBox_podjetja->currentText()) {
                tmp = mList.at(6);
                tmp.remove("Dat_valute: ");
                if(tmp.endsWith(date)) {
                    tmp.remove(-6,6);
                    tmpList = tmp.split(exp_dot, QString::SkipEmptyParts);
                    tmp = tmpList.at(1);
                    switch (tmp.toInt()) {
                        case 1: {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(0,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(0,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(0,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 2:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(1,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(1,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(1,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 3:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(2,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(2,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(2,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 4:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(3,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(3,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(3,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 5:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(4,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(4,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(4,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 6:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(5,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(5,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(5,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 7:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(6,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(6,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(6,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 8:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(7,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(7,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(7,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 9:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(8,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(8,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(8,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 10:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(9,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(9,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(9,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 11:
                        {
                            // OSNOVA
                            tmp = mList.at(5);
                            tmp.remove("Znesek: ");
                            tmp = QString::number(tmp.toDouble(), 'f', 2);
                            num = ui->tableWidget_vse->item(10,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova += tmp.toDouble();
                            itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            ui->tableWidget_vse->setItem(10,0,itm_setOsnova);
                            ui->tableWidget_brez->setItem(10,0,itm_setOsnovaBrez);
                            break;
                        }
                        case 12:
                    {
                        // OSNOVA
                        tmp = mList.at(5);
                        tmp.remove("Znesek: ");
                        tmp = QString::number(tmp.toDouble(), 'f', 2);
                        num = ui->tableWidget_vse->item(11,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova += tmp.toDouble();
                        itm_setOsnova->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        itm_setOsnovaBrez->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        ui->tableWidget_vse->setItem(11,0,itm_setOsnova);
                        ui->tableWidget_brez->setItem(11,0,itm_setOsnovaBrez);
                        break;
                    }
                        default:
                            break;
                    }
                } else {
                    continue;
                }
            }
        }
        mFile.close();
    }

    mFile.setFileName(fileName);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error opening file for reading in AddToTableWidget() in statistic.cpp";
        return;
    } else {
        QTextStream mText(&mFile);
        mText.setCodec("UTF-8");
        QString mLine("");
        QStringList mList;
        QRegularExpression exp(" ; ");
        QRegularExpression exp_dot(". ");
        QStringList tmpList;
        double mSumOsnova(0);
        QString date = QString::number(ui->dateEdit->date().year());
        QString num("");
        QString tmp("");
        QString opomba("");
        while(!mText.atEnd()) {
        QTableWidgetItem *itm_setOsnovaBrezIF = new QTableWidgetItem();
        itm_setOsnovaBrezIF->setTextAlignment(Qt::AlignCenter);
        mLine = mText.readLine();
        mList = mLine.split(exp, QString::SkipEmptyParts);
        tmp = mList.at(4);
        tmp.remove("Dobavitelj: ");
        opomba = mList.at(9);
        opomba.remove("Opombe: ");
        if(opomba == "terjatev_je_bila_odstranjena!!!????") {
            continue;
        }
        if(ui->comboBox_podjetja->currentIndex() == 0) {
            tmp = mList.at(6);
            tmp.remove("Dat_valute: ");
            if(tmp.endsWith(date)) {
                tmp.remove(-6,6);
                tmpList = tmp.split(exp_dot, QString::SkipEmptyParts);
                tmp = tmpList.at(1);
                switch (tmp.toInt()) {
                    case 1:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(0,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(0,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(0,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(0,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 2:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(1,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(1,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(1,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(1,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 3:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(2,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(2,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(2,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(2,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 4:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(3,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(3,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(3,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(3,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 5:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(4,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(4,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(4,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(4,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 6:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(5,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(5,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(5,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(5,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 7:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(6,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(6,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(6,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(6,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 8:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(7,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(7,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(7,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(7,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 9:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(8,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(8,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(8,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(8,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 10:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(9,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(9,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(9,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(9,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 11:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(10,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(10,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(10,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(10,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    case 12:
                    {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(11,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(11,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(11,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(11,0,itm_setOsnovaBrezIF);
                    }
                    break;
                    }
                    default:
                        break;
                    }
                } else {
                    continue;
                }
        } else if(tmp == ui->comboBox_podjetja->currentText()) {
            tmp = mList.at(6);
            tmp.remove("Dat_valute: ");
            if(tmp.endsWith(date)) {
                tmp.remove(-6,6);
                tmpList = tmp.split(exp_dot, QString::SkipEmptyParts);
                tmp = tmpList.at(1);
                switch (tmp.toInt()) {
                    case 1: {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(0,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(0,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(0,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(0,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 2:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(1,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(1,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(1,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(1,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 3:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(2,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(2,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(2,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(2,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 4:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(3,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(3,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(3,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(3,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 5:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(4,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(4,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(4,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(4,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 6:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(5,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(5,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(5,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(5,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 7:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(6,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(6,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(6,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(6,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 8:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(7,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(7,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(7,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(7,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 9:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(8,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(8,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(8,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(8,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 10:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(9,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(9,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(9,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(9,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 11:
                    {
                        // OSNOVA BREZ
                        tmp = mList.at(7);
                        tmp.remove("Placilo: ");
                        if(tmp != "") {
                            num = ui->tableWidget_brez->item(10,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            mSumOsnova -= tmp.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(10,0,itm_setOsnovaBrezIF);
                        } else {
                            num = ui->tableWidget_brez->item(10,0)->text();
                            num.remove("€");
                            mSumOsnova = num.toDouble();
                            if(mSumOsnova < 0)
                                mSumOsnova = 0;
                            itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                            if(mSumOsnova > 0)
                                itm_setOsnovaBrezIF->setForeground(QColor("red"));
                            ui->tableWidget_brez->setItem(10,0,itm_setOsnovaBrezIF);
                        }
                        break;
                    }
                    case 12:
                {
                    // OSNOVA BREZ
                    tmp = mList.at(7);
                    tmp.remove("Placilo: ");
                    if(tmp != "") {
                        num = ui->tableWidget_brez->item(11,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        mSumOsnova -= tmp.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(11,0,itm_setOsnovaBrezIF);
                    } else {
                        num = ui->tableWidget_brez->item(11,0)->text();
                        num.remove("€");
                        mSumOsnova = num.toDouble();
                        if(mSumOsnova < 0)
                            mSumOsnova = 0;
                        itm_setOsnovaBrezIF->setText("€" + QString::number(mSumOsnova, 'f', 2));
                        if(mSumOsnova > 0)
                            itm_setOsnovaBrezIF->setForeground(QColor("red"));
                        ui->tableWidget_brez->setItem(11,0,itm_setOsnovaBrezIF);
                    }
                    break;
                }
                    default:
                        break;
                }
            } else {
                continue;
            }
        }
    }
    mFile.close();
    }
}

void StatistikaObveznosti::Plot() {
    ui->plot->clearPlottables();
    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(180, 180, 180));
    gradient.setColorAt(0.38, QColor(200, 200, 200));
    gradient.setColorAt(1, QColor(130, 130, 130));
    ui->plot->setBackground(QBrush(gradient));
    // set all companys name and color from comboBox
    QCPBars *podjetja[m_numInCombo];
    // create empty bar chart objects:
    for(int itr(0); itr < m_numInCombo; itr++)
    {
        podjetja[itr] = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
        podjetja[itr]->setAntialiased(false); // gives more crisp, pixel aligned bar borders
        podjetja[itr]->setStackingGap(1);
        // set names and colors:
        podjetja[itr]->setName(ui->comboBox_podjetja->itemText(itr));
        podjetja[itr]->setPen(QPen(QColor(0,0,0).lighter(130)));
        podjetja[itr]->setBrush(QColor(m_colorArray[qrand()%150], m_colorArray[qrand()%150], m_colorArray[qrand()%150]));
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
    ui->plot->xAxis->setTickLength(0, 5);
    ui->plot->xAxis->setRange(0, 13);
    ui->plot->xAxis->setBasePen(QPen(Qt::black));
    ui->plot->xAxis->setTickPen(QPen(Qt::black));
    ui->plot->xAxis->grid()->setVisible(true);
    ui->plot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->plot->xAxis->setTickLabelColor(Qt::black);
    ui->plot->xAxis->setLabelColor(Qt::black);

    // prepare y axis:
    ui->plot->yAxis->setRange(0, 17500);
    ui->plot->yAxis->setPadding(1); // a bit more space to the left border
    ui->plot->yAxis->setLabel("Promet po mesecih\nOSNOVA €");
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
    if(ui->comboBox_podjetja->itemText(ui->comboBox_podjetja->currentIndex()) == "Vse obveznosti")
    {
        for(int itr(1); itr < m_numInCombo; itr++)
        {
            GetData(allData, itr, "Vse obveznosti");
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
    ui->plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->plot->legend->setBrush(QColor(255, 255, 255, 100));
    ui->plot->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    legendFont.setFamily("MS Sans Serif");
    legendFont.setItalic(true);
    ui->plot->legend->setFont(legendFont);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->plot->replot();
    ui->plot->update();
}

void StatistikaObveznosti::GetData(QVector<double> *allData, int itr, QString name)
{
    QString date = QString::number(ui->dateEdit->date().year());
    QString opomba("");
    QFile mFile(m_fileRacun);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in GetData in statistic.cpp";
        return;
    } else {
        if(name == "Vse obveznosti") {
            QTextStream in(&mFile);
            in.setCodec("UTF-8");
            QString lineText("");
            QString tmp("");
            QStringList listText;
            QStringList tmpList;
            QRegularExpression expDot(". ");
            QRegularExpression exp(" ; ");
            double sumPerMonth(0);
            for(int itr_month(0); itr_month < 12; itr_month++) {
                while(!in.atEnd()) {
                    lineText = in.readLine();
                    listText = lineText.split(exp, QString::SkipEmptyParts);
                    tmp = listText.at(4);
                    tmp.remove("Dobavitelj: ");
                    opomba = listText.at(9);
                    opomba.remove("Opombe: ");
                    if(opomba == "terjatev_je_bila_odstranjena!!!????") {
                        continue;
                    }
                    if(ui->comboBox_podjetja->itemText(itr) == tmp) {
                        tmp = listText.at(6);
                        tmp.remove("Dat_valute: ");
                        if(tmp.endsWith(date)) {
                            tmpList = tmp.split(expDot, QString::SkipEmptyParts);
                            tmp = tmpList.at(1);
                            if(tmp.toInt() == (itr_month+1))
                            {
                                tmp = listText.at(5);
                                tmp.remove("Znesek: ");
                                tmp = QString::number(tmp.toDouble(), 'f', 2);
                                sumPerMonth += tmp.toDouble();
                            }
                        } else {
                            continue;
                        }
                    }
                }
                in.seek(0);
                allData[itr].append(sumPerMonth);
                sumPerMonth = 0;
            }
        } else {
            QTextStream in(&mFile);
            in.setCodec("UTF-8");
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
                    tmp = listText.at(4);
                    tmp.remove("Dobavitelj: ");
                    opomba = listText.at(9);
                    opomba.remove("Opombe: ");
                    if(opomba == "terjatev_je_bila_odstranjena!!!????") {
                        continue;
                    }
                    if(name == tmp)
                    {
                        tmp = listText.at(6);
                        tmp.remove("Dat_valute: ");
                        if(tmp.endsWith(date)) {
                            tmpList = tmp.split(expDot, QString::SkipEmptyParts);
                            tmp = tmpList.at(1);
                            if(tmp.toInt() == (itr_month+1))
                            {
                                tmp = listText.at(5);
                                tmp.remove("Znesek: ");
                                tmp = QString::number(tmp.toDouble(), 'f', 2);
                                sumPerMonth += tmp.toDouble();
                            }
                        } else {
                            continue;
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

void StatistikaObveznosti::on_pushButton_isci_clicked() {
    Plot();
    AddToTableWidget(m_fileRacun);
}

void StatistikaObveznosti::on_tableWidget_vse_itemDoubleClicked(QTableWidgetItem *item) {
    Terjatve terjatve;
    terjatve.setModal(true);
    QDate dateOd;
    dateOd.setDate(ui->dateEdit->date().year(), item->row()+1, 1);
    QDate dateDo;
    dateDo.setDate(ui->dateEdit->date().year(), item->row()+1, dateOd.daysInMonth());
    terjatve.SetStatisticParameterObveznosti(ui->comboBox_podjetja->currentIndex(),  dateOd, dateDo, false);
    terjatve.ReadTerjatve(true);
    this->hide();
    QObject::connect(&terjatve,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        terjatve.exec();
    }
    Plot();
    AddToTableWidget(m_fileRacun);
    this->show();
}

void StatistikaObveznosti::on_tableWidget_brez_itemDoubleClicked(QTableWidgetItem *item) {
    Terjatve terjatve;
    terjatve.setModal(true);
    QDate dateOd;
    dateOd.setDate(ui->dateEdit->date().year(), item->row()+1, 1);
    QDate dateDo;
    dateDo.setDate(ui->dateEdit->date().year(), item->row()+1, dateOd.daysInMonth());
    terjatve.SetStatisticParameterObveznosti(ui->comboBox_podjetja->currentIndex(),  dateOd, dateDo, true);
    terjatve.ReadTerjatve(false);
    this->hide();
    QObject::connect(&terjatve,SIGNAL(close_me()),this,SLOT(CloseChild()));
    m_show_child = true;
    while (m_show_child) {
        terjatve.exec();
    }
    Plot();
    AddToTableWidget(m_fileRacun);
    this->show();
}

void StatistikaObveznosti::on_comboBox_podjetja_currentIndexChanged(int /*index*/) {
    Plot();
    AddToTableWidget(m_fileRacun);
}
