#ifndef TESTINGDIALOG_H
#define TESTINGDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>

namespace Ui {
class TestingDialog;
}

class TestingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestingDialog(QWidget *parent = nullptr);
    ~TestingDialog();
    void Test();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::TestingDialog *ui;
    int m_num_of_products;
};

#endif // TESTINGDIALOG_H
