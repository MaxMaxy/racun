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
    void PopraviObveznost(QString, QString, QString, QString, QString, QString);

signals:
    void close_me();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_pushButton_vnos_clicked();
    void on_lineEdit_listina_textChanged(const QString &arg1);
    void on_lineEdit_dobavitelj_textChanged(const QString &arg1);
    void on_lineEdit_znesek_textChanged(const QString &arg1);
    void on_lineEdit_opombe_textChanged(const QString &arg1);
    void on_pushButton_izhod_clicked();

    void on_pushButton_popraviObv_clicked();

    void on_lineEdit_dobavitelj_editingFinished();

private:
    Ui::VnosObveznosti *ui;
    QString m_currentDir;
    QString m_upnikSeznam;
    QString m_vnosObv;
    QString m_staraObveznost;
};

#endif // VNOSOBVEZNOSTI_H
