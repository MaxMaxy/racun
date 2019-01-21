#ifndef PLACILORACUNA_H
#define PLACILORACUNA_H

#include <QDialog>
#include <QDate>
#include <QStringList>
#include <QIcon>
#include <QMessageBox>

namespace Ui {
class PlaciloRacuna;
}

class PlaciloRacuna : public QDialog
{
    Q_OBJECT

public:
    explicit PlaciloRacuna(QWidget *parent = nullptr);
    ~PlaciloRacuna();
    void setOpombe(QString&, QString&, QString&, QString&, bool);
    bool deleteRacun;
    bool m_quit;

protected:
    void closeEvent(QCloseEvent *);

signals:
  void close_me();

public slots:
    QStringList on_pushButton_clicked();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_opombe_textChanged(const QString &arg1);
    void on_pushButton_izbris_clicked();
    void reject();

private:
    Ui::PlaciloRacuna *ui;
    QStringList m_list;
    bool m_obveznost;
};

#endif // PLACILORACUNA_H
