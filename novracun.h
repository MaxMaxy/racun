#ifndef NOVRACUN_H
#define NOVRACUN_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QString>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QDebug>
#include <QPrinter>
#include <QTextDocument>
#include <QMessageBox>
#include <QPrintDialog>
#include <QTextEdit>
#include <QPainter>
#include <QIcon>
#include "numofitems.h"
#include "dodajprodukt.h"

namespace Ui {
class NovRacun;
}

class NovRacun : public QDialog
{
    Q_OBJECT

public:
    explicit NovRacun(QWidget *parent = 0);
    ~NovRacun();
    void Read();
    void AddItemsToCombo();
    void AddRoot(QString, QString, QString);
    void CalcSkupaj(QString &, QString &, bool);
    void Search(QString);
    void Arhiv(QString, QString);
    void Shrani();
    QString CenaDDV(QString, QString);


private slots:
    void on_comboBox_narocnik_currentIndexChanged();
    void on_treeWidget_seznam_doubleClicked();
    void on_treeWidget_dodani_doubleClicked();
    void on_lineEdit_popust_editingFinished();
    void on_lineEdit_isci_editingFinished();
    void on_pushButton_dodajNovProdukt_clicked();

private:
    Ui::NovRacun *ui;
    QString m_currentDir;
    QString m_fileName;
    QString m_arhivNovRacun;
    QString m_arhivStRacuna;
    QString m_arhivLogin;
    QString m_cNaziv;
    QString m_naslov;
    QString m_posta;
    QString m_ddv;
    QString m_email;
    QString m_numItems;
    QString m_shrani;
    QString m_fileShrani;
    bool m_count;
    float m_total;
    int m_itemsAdded;
    int m_max_produktov;
    int m_vrstic;

public slots:
    int creatPDF();
};

#endif // NOVRACUN_H