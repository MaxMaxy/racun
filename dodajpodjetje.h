#ifndef DODAJPODJETJE_H
#define DODAJPODJETJE_H

#include <QTreeWidgetItem>
#include <QDialog>
#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QIcon>

namespace Ui {
class DodajPodjetje;
}

class DodajPodjetje : public QDialog
{
    Q_OBJECT

public:
    explicit DodajPodjetje(QWidget *parent = nullptr);
    ~DodajPodjetje();
    void TotalCompany();
    void Write(QString, QString);
    void Read();
    void AddRoot(QString, QString, QString, QString, QString, QString, QString);
    void AddChild(QTreeWidgetItem *, QString);
    void Arhiv(QString, QString);

private slots:
    void on_pushButton_dodajPodjetje_clicked();
    void on_pushButton_popraviPodatke_clicked();
    void on_treeWidget_doubleClicked(const QModelIndex &index);
    void on_lineEdit_kNaziv_textChanged(const QString &arg1);
    void on_lineEdit_cNaziv_textChanged(const QString &arg1);
    void on_lineEdit_naslov_textChanged(const QString &arg1);
    void on_lineEdit_posta_textChanged(const QString &arg1);
    void on_lineEdit_ddv_textChanged(const QString &arg1);
    void on_lineEdit_email_textChanged(const QString &arg1);

private:
    Ui::DodajPodjetje *ui;
    QString m_currentDir;
    QString m_fileName;
    QString m_numOfCompany;
    QString m_arhivStranke;
    QString m_zapSt;
    QString m_cNaziv;
    QString m_kNaziv;
    QString m_naslov;
    QString m_posta;
    QString m_ddv;
    QString m_email;
    QString m_stranka;
    int m_itr;
    int m_totalCompanys;
    bool m_count;
};

#endif // DODAJPODJETJE_H
