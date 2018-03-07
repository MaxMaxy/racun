#ifndef STATISTIC_H
#define STATISTIC_H

#include <QDialog>
#include <QDate>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

namespace Ui {
class Statistic;
}

class Statistic : public QDialog
{
    Q_OBJECT

public:
    explicit Statistic(QWidget *parent = 0);
    ~Statistic();
    void AddToComboBox(QString);
    void AddToTableWidget(QString);

private slots:
    void on_pushButton_izpis_clicked();

private:
    Ui::Statistic *ui;
    QString m_currentDir;
    QString m_fileNameCompanys;
    QString m_fileRacun;
};

#endif // STATISTIC_H
