#ifndef NOVRACUN_H
#define NOVRACUN_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QString>
#include <QTextDocument>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QDebug>
#include <QPrinter>
#include <QMessageBox>
#include <QPrintDialog>
#include <QIcon>
#include <QDomDocument>
#include <QDomElement>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QPrintPreviewDialog>
#include <QDesktopServices>
#include <QFileDialog>
#include <QKeyEvent>
#include "smtp.h"
#include "numofitems.h"
#include "dodajprodukt.h"
#include "eracun.h"

namespace Ui {
class NovRacun;
}

class NovRacun : public QDialog
{
    Q_OBJECT

public:
    explicit NovRacun(QWidget *parent = nullptr);
    ~NovRacun();
    void Read();
    void AddItemsToCombo();
    void AddRoot(QString, QString, QString);
    void CalcSkupaj(QString &, bool);
    void Search(QString);
    void Arhiv(QString, QString);
    void Shrani();
    void MakeXML(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void PopraviRacun(QString, QString, QString, QString, QString, QString, QString);
    void sendMail(QString);
    void mailSent(QString);
    void keyReleaseEvent(QKeyEvent*);
    QString CenaDDV(QString, QString);

signals:
    void close_me();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_comboBox_narocnik_currentIndexChanged();
    void on_treeWidget_seznam_doubleClicked();
    void on_pushButton_dodajNovProdukt_clicked();
    void on_lineEdit_isci_textChanged();
    void on_lineEdit_stRacuna_textChanged(const QString &);
    void on_lineEdit_sklic_textChanged(const QString &);
    void on_lineEdit_textChanged(const QString &);
    void on_pushButton_izhod_clicked();
    void CloseChild();
    void on_checkBox_eRacun_stateChanged(int);
    void on_radioButton_racun_clicked();
    void on_radioButton_predRacun_clicked();
    void on_radioButton_dobavnica_clicked();
    void on_radioButton_ponudba_clicked();

    void on_treeWidget_dodani_itemDoubleClicked(QTreeWidgetItem *item, int column);

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
    QString m_postnaStStranke;
    QString m_ddv;
    QString m_email;
    QString m_numItems;
    QString m_popust;
    QString m_shrani;
    QString m_fileShrani;
    QStringList m_files;
    QString m_drzavaStranke;
    QString m_kodaDrzave;
    QString m_bancniRacunStranke;
    QString m_bicStranke;
    QString m_vatStranke;
    bool m_count;
    double m_total;
    int m_itemsAdded;
    int m_max_produktov;
    int m_vrstic;
    bool m_sprememba;
    bool m_show_child;

public slots:
    void creatPDF();
};

#endif // NOVRACUN_H
