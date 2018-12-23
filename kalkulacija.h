#ifndef KALKULACIJA_H
#define KALKULACIJA_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QDebug>

namespace Ui {
class Kalkulacija;
}

class Kalkulacija : public QDialog
{
    Q_OBJECT

public:
    explicit Kalkulacija(QWidget *parent = nullptr);
    ~Kalkulacija();
    void Read();
    void AddItemsToCombo();

private:
    Ui::Kalkulacija *ui;
    QString m_currentDir;
    QString m_fileName;
    QString m_cNaziv;
};

#endif // KALKULACIJA_H
