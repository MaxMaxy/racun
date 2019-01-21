#ifndef TESTINGDIALOG_H
#define TESTINGDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileDialog>
#include "smtp.h"

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
    void browse();
    void sendMail();
    void mailSent(QString);

private:
    Ui::TestingDialog *ui;
    int m_num_of_products;
    int timeout;
    QString message;
    QStringList files;
};

#endif // TESTINGDIALOG_H
