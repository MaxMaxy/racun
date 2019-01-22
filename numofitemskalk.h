#ifndef NUMOFITEMSKALK_H
#define NUMOFITEMSKALK_H

#include <QDialog>
#include <QRegularExpressionValidator>

namespace Ui {
class numOfItemsKalk;
}

class numOfItemsKalk : public QDialog
{
    Q_OBJECT

public:
    explicit numOfItemsKalk(QWidget *parent = nullptr);
    void LineEditorState(bool);
    QString m_kolicina;
    ~numOfItemsKalk();

protected:
    void closeEvent(QCloseEvent *);

signals:
  void close_me();

private slots:
    void on_pushButton_vstavi_clicked();
    void reject();

private:
    Ui::numOfItemsKalk *ui;
    bool m_dolzina;
};

#endif // NUMOFITEMSKALK_H
