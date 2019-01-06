#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QIcon>
#include <QRegularExpression>

namespace Ui {
    class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_pushButton_shraniV_clicked();
    void on_pushButton_nastaviShrani_clicked();

private:
    Ui::Settings *ui;
    QString m_currentDir;
    QString m_fileName;
    QString m_dir;
};

#endif // SETTINGS_H
