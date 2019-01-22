#ifndef VNOSOBVEZNOSTI_H
#define VNOSOBVEZNOSTI_H

#include <QDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QIcon>

namespace Ui {
class VnosObveznosti;
}

class VnosObveznosti : public QDialog
{
    Q_OBJECT

public:
    explicit VnosObveznosti(QWidget *parent = nullptr);
    ~VnosObveznosti();
    void VnosObv();
    void VnosUpnika();

protected:
    void closeEvent(QCloseEvent *);

signals:
  void close_me();

private slots:
    void on_pushButton_vnos_clicked();
    void on_lineEdit_listina_textChanged(const QString &arg1);
    void on_lineEdit_dobavitelj_textChanged(const QString &arg1);
    void on_lineEdit_znesek_textChanged(const QString &arg1);
    void on_lineEdit_opombe_textChanged(const QString &arg1);

private:
    Ui::VnosObveznosti *ui;
    QString m_currentDir;
    QString m_upnikSeznam;
    QString m_vnosObv;
};

#endif // VNOSOBVEZNOSTI_H
