#ifndef ERACUN_H
#define ERACUN_H

#include <QDialog>
#include <QIntValidator>

namespace Ui {
class eRacun;
}

class eRacun : public QDialog
{
    Q_OBJECT

signals:
    void close_me();

protected:
    void closeEvent(QCloseEvent *);

public:
    explicit eRacun(QWidget *parent = nullptr);
    ~eRacun();
    void setEditLines(QString, QString, QString, QString, QString, QString, QString, QString);
    QString m_cNaziv;
    QString m_naslov;
    QString m_posta;
    QString m_stevilkaPoste;
    QString m_drzavaStranke;
    QString m_kodaDrzave;
    QString m_bancniRacunStranke;
    QString m_bicStranke;
    QString m_vatStranke;

private slots:
    void on_pushButton_vnos_clicked();

private:
    Ui::eRacun *ui;
};

#endif // ERACUN_H
