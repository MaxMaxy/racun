#ifndef novaNalepka_H
#define novaNalepka_H

#include "methods.h"

QT_BEGIN_NAMESPACE
namespace Ui { class novaNalepka; }
QT_END_NAMESPACE

class novaNalepka : public QDialog
{
    Q_OBJECT

// PUBLIC METHODS
public:
    explicit novaNalepka(QWidget *parent = nullptr);
    ~novaNalepka();
    void Reset();
    void keyReleaseEvent(QKeyEvent*);

// SLOTS
private slots:
    void on_pushButton_natisni_clicked();
    void on_textEdit_qrNapis_textChanged();
    void on_lineEdit_napis_textChanged(const QString& arg1);

// PRIVATE VARS
private:
    Ui::novaNalepka *ui;
    Methods* m_methods;
    QPixmap m_map;
    QPixmap m_mapSaved;
    QString m_Qrtext;
    QString m_id;
    QString m_napis;
    QString m_QrKodaAliNapis;
    int m_numOfCopies;
    Methods::NacinTiska m_nacinTiska;
};

#endif // novaNalepka_H
