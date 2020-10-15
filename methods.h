#ifndef METHODS_H
#define METHODS_H

#include <QMainWindow>
#include <QPainter>
#include <QDate>
#include <QPdfWriter>
#include <QFile>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QKeyEvent>

#include "QrCode.hpp"

class Methods : public QMainWindow
{
    friend class novaNalepka;
    Q_OBJECT

// PUBLIC VARS
public:
    enum class NacinTiska {
        Standard = 0,
        Napis
    };
    enum class ErrorType {
        ReadError = 0,
        SearchError,
        ProductCheckError,
        DeleteError_ReadFile,
        DeleteError_DeleteFileText,
        DeleteError_InsertNewText,
        SaveError,
        DefoultError
    };

//PUBLIC METHODS
public:
    // Constructor
    explicit Methods(QWidget *parent = nullptr);
    // Destructor
    ~Methods();
    // Methods
    virtual void Reset();
    virtual void keyReleaseEvent(QKeyEvent*);
    void drawText(QPainter&, QPointF&,
                  Qt::Alignment&, const QString&,
                  QRectF*);
    void drawQr(QPixmap&, const QString&,
                const QString&, const QString&);
    void NalepkaPrint(const QString&, const QString&,
                      const QString&, const QString&,
                      const int&, const NacinTiska&);
    void ErrorCall(QFile&, const ErrorType&);

// PRIVATE VARS
protected:
    QString m_nalepkaText;
    Qt::Alignment m_alignment;
    short m_nalepkaCentimeter;
    short m_qrVelikost;
    short m_sirinaNalepke;
    short m_visinaNalepke;
    short m_ofSetHeader;
    short m_ofSetListIzdelka_FirstLine;
    short m_ofSetId_OneLine;
    short m_ofSetNaziv_SecondLine;
    short m_ofSetKolicina;
    short m_XofSet;
};

#endif // METHODS_H
