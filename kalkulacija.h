#ifndef KALKULACIJA_H
#define KALKULACIJA_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QString>
#include <QDateTime>
#include <QScrollArea>
#include <QGroupBox>
#include "numofitemskalk.h"
#include "numofhours.h"

namespace Ui {
class Kalkulacija;
}

class Kalkulacija : public QDialog
{
    Q_OBJECT

public:
    explicit Kalkulacija(QWidget *parent = nullptr);
    ~Kalkulacija();
    void Read(QString, QTreeWidget *);
    void AddItemsToCombo();
    void AddRoot(QString, QString, QString, QString, QTreeWidget *);
    void Search(QString, QString, QTreeWidget *);
    void Arhiv(QString, QString);

private slots:
    void on_lineEdit_iskalnik_textChanged(const QString &);
    void on_treeWidget_material_itemDoubleClicked();
    void on_treeWidget_materialProdukta_itemDoubleClicked();
    void on_treeWidget_delo_itemDoubleClicked();
    void on_treeWidget_deloProdukta_itemDoubleClicked();
    void on_pushButton_dodajMaterial_clicked();
    void on_pushButton_dodajOperacijo_clicked();
    void on_lineEdit_kalo_textChanged();
    void on_checkBox_popraviOperacijo_stateChanged();
    void on_checkBox_popravekMateriala_stateChanged();
    void on_pushButton_vnosProdukta_clicked();

private:
    Ui::Kalkulacija *ui;
    QString m_currentDir;
    QString m_companyFile;
    QString m_material;
    QString m_delavniProces;
    QString m_arhivProdukti;
    QString m_cNaziv;
    double m_materialCena;
    double m_deloCena;
    bool m_count;
};

#endif // KALKULACIJA_H
