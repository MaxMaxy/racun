#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QIcon>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    void Arhiv(QString, QString);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_prijava_clicked();

private:
    Ui::Login *ui;
    QString m_currentDir;
    QString m_fileName;
    QString m_arhivLogin;
};

#endif // LOGIN_H
