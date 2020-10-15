#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "novanalepka.h"
#include "methods.h"
#include "proizvodniproces.h"
#include "dodajprodukt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public Methods
{
    Q_OBJECT

// PUBLIC METHODS
public:
    // Constructor
    MainWindow(QWidget *parent = nullptr);
    // Destructor
    ~MainWindow();
    //Methods
    void Reset() override;
    void AddRootToTreeWidget(const QString&, const QString&,
                             QTreeWidgetItem*);
    void ReadFileAndAddToTreeWidget();
    void Search(const QString&, const QString&);
    void keyReleaseEvent(QKeyEvent*) override;
    void ProduktCheck(QString&, QString&);
    void Error(const ErrorType&);

// SLOTS
private slots:
    void on_pushButton_shraniNalepko_clicked();
    void on_pushButton_natisni_clicked();
    void on_lineEdit_IDprodukta_textChanged(const QString&);
    void on_lineEdit_nazivProdukta_textChanged(const QString&);
    void on_treeWidget_itemClicked(QTreeWidgetItem*);
    void on_treeWidget_customContextMenuRequested(QPoint);
    void on_actionIzhod_triggered();
    void on_actionDelete_triggered();
    void on_actionShrani_nalepko_triggered();
    void on_actionPrint_triggered();
    void on_actionO_programu_triggered();
    void on_actionNov_napis_triggered();
    void on_textEdit_opombe_textChanged();

// PRIVATE VARS
    void on_actionProizvodni_proces_triggered();

    void on_actionDodaj_produkt_triggered();

private:
    Ui::MainWindow *ui;
    bool m_count;
    bool m_TreeWidgetIsClicked;
    bool m_executable;
    QString m_id;
    QString m_naziv;
    QString m_kolicina;
    QString m_opomba;
    QString m_verzija;
    QLabel *m_verzijaLabel;
    QString m_searchLine;
    QStringList m_searchList;
    short m_treeItemCount;
    int m_numOfCopies;
    Methods::NacinTiska m_nacinTiska;
};
#endif // MAINWINDOW_H
