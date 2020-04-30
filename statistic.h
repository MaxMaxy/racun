#ifndef STATISTIC_H
#define STATISTIC_H

#include <QDialog>
#include <QDate>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QVector>
#include <QTableWidgetItem>
#include "terjatve.h"

namespace Ui {
    class Statistic;
}

class Statistic : public QDialog {
    Q_OBJECT

public:
    explicit Statistic(QWidget *parent = nullptr);
    ~Statistic();
    void AddToComboBox(QString);
    void AddToTableWidget(QString);
    void Plot();
    void GetData(QVector<double>*, int, QString);

protected:
    void closeEvent(QCloseEvent *);

signals:
  void close_me();

private slots:
    void on_pushButton_isci_clicked();
    void on_tableWidget_vse_itemDoubleClicked(QTableWidgetItem *item);
    void on_tableWidget_brez_itemDoubleClicked(QTableWidgetItem *item);
    void CloseChild();

    void on_comboBox_podjetja_currentIndexChanged(int index);

private:
    Ui::Statistic *ui;
    QString m_currentDir;
    QString m_fileNameCompanys;
    QString m_fileRacun;
    QString m_upnikiSeznam;
    int m_numInCombo;
    int m_colorArray[150];
    bool m_show_child;
};

#endif // STATISTIC_H
