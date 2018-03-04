#ifndef ARHIV_H
#define ARHIV_H

#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QDir>
#include <QIcon>
#include <QTextStream>

namespace Ui {
class Arhiv;
}

class Arhiv : public QDialog
{
    Q_OBJECT

public:
    explicit Arhiv(QWidget *parent = 0);
    ~Arhiv();
    void Read();
    void AddItemsToCombo();
    void Search(QString);

private slots:
    void on_comboBox_currentIndexChanged(const QString&);
    void on_lineEdit_isci_editingFinished();
    void on_radioButton_uspesnaPrijava_toggled(bool);
    void on_radioButton_vse_toggled(bool);
    void on_radioButton_neuspesnaPrijava_toggled(bool);
    void on_radioButton_dodaniProdukti_toggled(bool);
    void on_radioButton_popravljeniProdukti_toggled(bool);
    void on_radioButton_dodaneStranke_toggled(bool);
    void on_radioButton_popravljeneStranke_toggled(bool);

private:
    Ui::Arhiv *ui;
    QString m_currentDir;
    QString m_fileName;

};

#endif // ARHIV_H
