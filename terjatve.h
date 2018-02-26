#ifndef TERJATVE_H
#define TERJATVE_H

#include <QDialog>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QTextStream>
#include "vnosobveznosti.h"
#include "novracun.h"
#include "placiloracuna.h"

namespace Ui {
class Terjatve;
}

class Terjatve : public QDialog
{
    Q_OBJECT

public:
    explicit Terjatve(QWidget *parent = 0);
    ~Terjatve();

    void AddItemsToComboBox();
    void ReadTerjatve();
    void ReadObveznosti();
    void AddRootTerjatve(QStringList);
    void AddRootObveznosti(QStringList);
    void Search(QString, QString, bool);
    void SumTerjatve();

private slots:
    void on_comboBox_stranke_currentIndexChanged();
    void on_dateEdit_terjatveOd_editingFinished();
    void on_dateEdit_terjatveDo_editingFinished();
    void on_lineEdit_iskalnikTerjatve_editingFinished();
    void on_pushButton_clicked();
    void on_comboBox_upniki_currentIndexChanged();
    void on_dateEdit_obveznostiOd_editingFinished();
    void on_dateEdit_obveznostiDo_editingFinished();
    void on_lineEdit_iskalnikObveznosti_editingFinished();
    void on_pushButton_2_clicked();
    void on_treeWidget_terjatve_itemDoubleClicked(QTreeWidgetItem *);
    void on_treeWidget_obveznosti_itemDoubleClicked(QTreeWidgetItem *);

private:
    Ui::Terjatve *ui;
    QString m_stranke = "/usr/home/cloudjunkie/test.txt";
    QString m_terjatve = "/usr/home/cloudjunkie/arhiv_novRacun.txt";
    QString m_obveznosti = "/usr/home/cloudjunkie/arhiv_upniki.txt";
    QString m_upnikiSeznam = "/usr/home/cloudjunkie/arhiv_upnikiSeznam.txt";
    float m_totalTerjatve;
    float m_totalObveznosti;
    float m_totalTerMinObv;
};

#endif // TERJATVE_H
