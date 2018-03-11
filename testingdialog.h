#ifndef TESTINGDIALOG_H
#define TESTINGDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class TestingDialog;
}

class TestingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestingDialog(QWidget *parent = 0);
    ~TestingDialog();
    void Test();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::TestingDialog *ui;
};

#endif // TESTINGDIALOG_H
