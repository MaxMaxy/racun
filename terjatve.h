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
#include <QIcon>
#include "vnosobveznosti.h"
#include "novracun.h"
#include "placiloracuna.h"
#include "statistic.h"

namespace Ui {
class Terjatve;
}

class Terjatve : public QDialog
{
    Q_OBJECT

public:
    explicit Terjatve(QWidget *parent = nullptr);
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
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged();
    void on_dateEdit_obveznostiOd_editingFinished();
    void on_dateEdit_obveznostiDo_editingFinished();
    void on_pushButton_2_clicked();
    void on_treeWidget_terjatve_itemDoubleClicked(QTreeWidgetItem *);
    void on_treeWidget_obveznosti_itemDoubleClicked(QTreeWidgetItem *);
    void on_pushButton_statistika_clicked();
    void on_lineEdit_iskalnikTerjatve_textChanged();
    void on_lineEdit_iskalnikObveznosti_textChanged();

private:
    Ui::Terjatve *ui;
    QString m_currentDir;
    QString m_stranke;
    QString m_terjatve;
    QString m_obveznosti;
    QString m_upnikiSeznam;
    double m_totalTerjatve;
    double m_totalObveznosti;
    double m_totalTerMinObv;
};

#endif // TERJATVE_H
