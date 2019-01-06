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

private slots:
    void on_pushButton_vnos_clicked();
    void reject();

private:
    Ui::NumOfItems *ui;
};

#endif // NUMOFITEMS_H
