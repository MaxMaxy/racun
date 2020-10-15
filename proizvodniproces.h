#ifndef PROIZVODNIPROCES_H
#define PROIZVODNIPROCES_H

#include <QDialog>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QFile>

#include <QDebug>

namespace Ui {
class ProizvodniProces;
}

class ProizvodniProces : public QDialog
{
    Q_OBJECT

public:
    explicit ProizvodniProces(QWidget *parent = nullptr);
    ~ProizvodniProces();
    void Search(const QString&);
    void AddRootToTreeWidget(const QStringList&, QTreeWidgetItem*);

private slots:
    void on_lineEdit_isciProdukt_textChanged(const QString &arg1);

private:
    Ui::ProizvodniProces *ui;
    QString m_searchLine;
    QStringList m_searchList;
};

#endif // PROIZVODNIPROCES_H
