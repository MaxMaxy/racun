#ifndef VNOSOBVEZNOSTI_H
#define VNOSOBVEZNOSTI_H

#include <QDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>

namespace Ui {
class VnosObveznosti;
}

class VnosObveznosti : public QDialog
{
    Q_OBJECT

public:
    explicit VnosObveznosti(QWidget *parent = 0);
    ~VnosObveznosti();
    void VnosObv();
    void VnosUpnika();

private slots:
    void on_pushButton_vnos_clicked();

private:
    Ui::VnosObveznosti *ui;
    QString m_upnikSeznam = "/usr/home/cloudjunkie/arhiv_upnikiSeznam.txt";
    QString m_vnosObv = "/usr/home/cloudjunkie/arhiv_upniki.txt";
};

#endif // VNOSOBVEZNOSTI_H
