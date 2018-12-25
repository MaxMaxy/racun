#ifndef NUMOFITEMSKALK_H
#define NUMOFITEMSKALK_H

#include <QDialog>

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

private slots:
    void on_pushButton_vstavi_clicked();

private:
    Ui::numOfItemsKalk *ui;
    bool m_dolzina;
};

#endif // NUMOFITEMSKALK_H
