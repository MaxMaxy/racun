#ifndef PLACILORACUNA_H
#define PLACILORACUNA_H

#include <QDialog>
#include <QDate>
#include <QStringList>
#include <QIcon>
#include <QMessageBox>
#include "vnosobveznosti.h"
#include "novracun.h"

namespace Ui {
class PlaciloRacuna;
}

class PlaciloRacuna : public QDialog
{
    Q_OBJECT

public:
    explicit PlaciloRacuna(QWidget *parent = nullptr);
    ~PlaciloRacuna();
    void setOpombe(QString&, QString&, QString&, QString&, QString&, bool);
    void setVars(QString&, QString&, QString&, QString&, QString&, QString&);
    bool deleteRacun;
    bool m_quit;
    bool m_popravljen;

protected:
    void closeEvent(QCloseEvent *);

signals:
  void close_me();

public slots:
    QStringList on_pushButton_clicked();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_opombe_textChanged(const QString &arg1);
    void reject();
    void on_radioButton_placiloSkupaj_clicked();
    void on_radioButton_placiloEnkratno_clicked();
    void on_pushButton_2_clicked();
    void on_toolButton_clicked();
    void CloseChild();
    void on_toolButton_2_clicked();

private:
    Ui::PlaciloRacuna *ui;
    QStringList m_list;
    QString m_stRacuna;
    QString m_dobavitelj;
    QString m_listina;
    QString m_znesek;
    QString m_datum;
    QString m_opomba;
    QString m_staraObveznost;
    QString m_currentDir;
    QString m_arhivRacun;
    bool m_obveznost;
    bool m_show_child;
};

#endif // PLACILORACUNA_H
