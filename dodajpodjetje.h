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

namespace Ui {
class DodajPodjetje;
}

class DodajPodjetje : public QDialog
{
    Q_OBJECT

public:
    explicit DodajPodjetje(QWidget *parent = 0);
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
    bool m_count;
    int m_totalCompanys;
};

#endif // DODAJPODJETJE_H
