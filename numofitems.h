#ifndef NUMOFITEMS_H
#define NUMOFITEMS_H

#include <QDialog>
#include <QIcon>
#include <QIntValidator>

namespace Ui {
class NumOfItems;
}

class NumOfItems : public QDialog
{
    Q_OBJECT

public:
    explicit NumOfItems(QWidget *parent = nullptr);
    ~NumOfItems();
    QString m_numKosov;
    QString m_popust;
    bool m_kronoterm;

protected:
    void closeEvent(QCloseEvent *);

signals:
  void close_me();

private slots:
    void on_pushButton_vnos_clicked();
    void reject();

    void on_lineEdit_steviloKosov_textChanged(const QString &arg1);

private:
    Ui::NumOfItems *ui;
};

#endif // NUMOFITEMS_H
