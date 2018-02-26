#ifndef PLACILORACUNA_H
#define PLACILORACUNA_H

#include <QDialog>
#include <QDate>
#include <QStringList>

namespace Ui {
class PlaciloRacuna;
}

class PlaciloRacuna : public QDialog
{
    Q_OBJECT

public:
    explicit PlaciloRacuna(QWidget *parent = 0);
    ~PlaciloRacuna();
    void setOpombe(QString&, QString&, QString&);

public slots:
    QStringList on_pushButton_clicked();

private:
    Ui::PlaciloRacuna *ui;
    QStringList m_list;
};

#endif // PLACILORACUNA_H
