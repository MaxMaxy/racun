#ifndef STATISTIC_H
#define STATISTIC_H

#include <QDialog>
#include <QDate>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QVector>

namespace Ui {
class Statistic;
}

class Statistic : public QDialog
{
    Q_OBJECT

public:
    explicit Statistic(QWidget *parent = nullptr);
    ~Statistic();
    void AddToComboBox(QString);
    void AddToTableWidget(QString);
    void Plot();
    void GetData(QVector<double>*, int, QString);

private slots:
    void on_pushButton_isci_clicked();

private:
    Ui::Statistic *ui;
    QString m_currentDir;
    QString m_fileNameCompanys;
    QString m_fileRacun;
    int m_numInCombo;
    int m_colorArray[150];
};

#endif // STATISTIC_H
