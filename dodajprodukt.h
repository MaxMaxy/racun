#ifndef DODAJPRODUKT_H
#define DODAJPRODUKT_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QDateTime>
#include <QIcon>

namespace Ui {
class DodajProdukt;
}

class DodajProdukt : public QDialog
{
    Q_OBJECT

public:
    explicit DodajProdukt(QWidget *parent = nullptr);
    ~DodajProdukt();
    void AddItemsToCombo();
    void AddRoot(QString, QString, QString);
    void Read();
    void Search(QString);
    void Arhiv(QString, QString);
    void SetIndex(int);

private slots:
    void on_pushButton_dodaj_clicked();
    void on_comboBox_podjetje_currentIndexChanged();
    void on_treeWidget_doubleClicked(const QModelIndex &index);
    void on_pushButton_popravi_clicked();
    void on_lineEdit_isci_textChanged();
    void on_lineEdit_nazivProdukta_textChanged(const QString &arg1);
    void on_lineEdit_cena_textChanged(const QString &arg1);

private:
    Ui::DodajProdukt *ui;
    QString m_currentDir;
    QString m_fileName;
    QString m_arhivProdukti;
    QString m_id;
    QString m_naziv;
    QString m_cena;
    QString m_produkt;
    bool m_count;
    int m_itr;
};

#endif // DODAJPRODUKT_H
