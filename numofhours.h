#ifndef NUMOFHOURS_H
#define NUMOFHOURS_H

#include <QDialog>
#include <QRegularExpressionValidator>

namespace Ui {
class NumOfHours;
}

class NumOfHours : public QDialog
{
    Q_OBJECT

public:
    explicit NumOfHours(QWidget *parent = nullptr);
    ~NumOfHours();
    QString m_numOfHours;

signals:
    void close_me();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_pushButton_clicked();
    void reject();

private:
    Ui::NumOfHours *ui;
};

#endif // NUMOFHOURS_H
