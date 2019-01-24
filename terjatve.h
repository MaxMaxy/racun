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
    void ReadTerjatve(bool);
    void ReadObveznosti(bool);
    void AddRootTerjatve(QStringList, bool);
    void AddRootObveznosti(QStringList, bool);
    void Search(QString, QString, bool);
    void SumTerjatve();
    void SetStatisticParameterTerjatve(int, QDate, QDate, bool);
    void SetStatisticParameterObveznosti(int, QDate, QDate, bool);
    void LabelsUpdate();
    void Shrani();

protected:
    void closeEvent(QCloseEvent *);

signals:
  void close_me();

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
    void on_lineEdit_iskalnikTerjatve_textChanged();
    void on_lineEdit_iskalnikObveznosti_textChanged();
    void on_comboBox_stranke_currentIndexChanged(int index);
    void on_comboBox_currentIndexChanged(int index);
    void on_dateEdit_terjatveOd_dateChanged(const QDate &date);
    void on_dateEdit_terjatveDo_dateChanged(const QDate &date);
    void on_dateEdit_obveznostiOd_dateChanged(const QDate &date);
    void on_dateEdit_obveznostiDo_dateChanged(const QDate &date);
    void on_checkBox_neplacaneTerjatve_stateChanged(int arg1);
    void on_checkBox_neplacaneObveznosti_stateChanged(int arg1);
    void on_pushButton_isciTerjatve_clicked();
    void on_pushButton_isciObveznosti_clicked();
    void CloseChild();

private:
    Ui::Terjatve *ui;
    QString m_currentDir;
    QString m_stranke;
    QString m_terjatve;
    QString m_obveznosti;
    QString m_upnikiSeznam;
    QString m_shrani;
    QString m_fileShrani;
    double m_totalTerjatve;
    double m_totalObveznosti;
    double m_totalTerMinObv;
    double m_totalTerjatveStaro;
    double m_totalObveznostiStaro;
    bool m_show_child;
};

#endif // TERJATVE_H
