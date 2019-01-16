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
    void AddItemsToCombo();
    void QML();
    void ReadFile();
    void WriteUnicode();
    void WriteASCII();

signals:
    void close_me();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_pushButton_Test_clicked();
    void on_pushButton_Test2_clicked();

private:
    Ui::TestingDialog *ui;
    int m_num_of_products;

    int timeout;
    QString message;
};

#endif // TESTINGDIALOG_H
