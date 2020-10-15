#include "methods.h"
#include <QDebug>
#include <QPrinterInfo>

// KONSTRUKTOR
Methods::Methods(QWidget *parent) :
    QMainWindow(parent),
    m_nalepkaText(""),
    m_alignment(Qt::AlignVCenter | Qt::AlignHCenter),
    m_nalepkaCentimeter(100),
    m_qrVelikost(m_nalepkaCentimeter*4),
    m_sirinaNalepke(900),
    m_visinaNalepke(700),
    m_ofSetHeader(m_nalepkaCentimeter / 3),
    m_ofSetListIzdelka_FirstLine(m_nalepkaCentimeter * 0.8),
    m_ofSetId_OneLine(m_nalepkaCentimeter * 1.4),
    m_ofSetNaziv_SecondLine(m_nalepkaCentimeter * 1.9),
    m_ofSetKolicina(m_nalepkaCentimeter * 2.4),
    m_XofSet((m_sirinaNalepke - m_nalepkaCentimeter) / 2)
{

}

// DESTRUKTOR
Methods::~Methods()
{
}

// VIRTUALNI RESET
void Methods::Reset()
{

}

// VIRTUAL KEYRELESEEVENT
void Methods::keyReleaseEvent(QKeyEvent*)
{

}

// POZICIONIRANJE TEXTA NA SREDINO NALEPKE
void Methods::drawText(QPainter& painter, QPointF& point,
                       Qt::Alignment& flags, const QString& text,
                       QRectF* boundingRect)
{
   const qreal size(32767.0);
   QPointF corner(point.x(), point.y() - size);
   if (flags & Qt::AlignHCenter)
       corner.rx() -= size / 2.0;
   else if (flags & Qt::AlignRight)
       corner.rx() -= size;
   if (flags & Qt::AlignVCenter)
       corner.ry() += size / 2.0;
   else if (flags & Qt::AlignTop)
       corner.ry() += size;
   else flags |= Qt::AlignBottom;
   QRectF rect{corner.x(), corner.y(), size, size};
   painter.drawText(rect, flags, text, boundingRect);
}

// QR GENERATOR
void Methods::drawQr(QPixmap& map, const QString& id,
                     const QString& naziv, const QString& opombe)
{
    // Qr text
    QChar newLine('\u000A');
    QString qrText("www.elraseti.si");
    qrText += newLine;
    qrText += id + " ; " + naziv + " ; " + QDate::currentDate().toString("d.M.yyyy");
    qrText += newLine;
    qrText += "Opombe: ";
    qrText += opombe;
    if(naziv == "")
        qrText = opombe;
    else if(opombe == "")
        qrText = naziv;
    else {
        qrText = "www.elraseti.si";
        qrText += newLine;
        qrText += id + " ; " + naziv + " ; " + QDate::currentDate().toString("d.M.yyyy");
        qrText += newLine;
        qrText += "Opombe: ";
        qrText += opombe;
    }

    // Painter
    QPainter painterQr(&map);
    char *str=qrText.toUtf8().data();
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(str, qrcodegen::QrCode::Ecc::HIGH);
    int sizeOf = qr.getInt();
    const int s=sizeOf>0?sizeOf:1;
    const double w=QSize(m_qrVelikost,m_qrVelikost).width();
    const double h=QSize(m_qrVelikost,m_qrVelikost).height();
    const double aspect=w/h;
    const double size=((aspect>1.0)?h:w);
    const double scale=size/(s+2);

    painterQr.setPen(Qt::NoPen);
    painterQr.setBrush(Qt::white);
    for(int y=0; y<2000; y++)
    {
        for(int x=0; x<2000; x++)
        {
            QRectF r(x, y, 1, 1);
            painterQr.drawRects(&r,1);
        }
    }
    painterQr.setBrush(QColor("black"));
    for(int y=0; y<s; y++)
    {
        for(int x=0; x<s; x++)
        {
            const int color = qr.getModule(x, y);
            if(0x0!=color)
            {
                const double rx1=(x+1)*scale, ry1=(y+1)*scale;
                QRectF r(rx1, ry1, scale, scale);
                painterQr.drawRects(&r,1);
            }
        }
    }

    map.save("qrNalepkePrint.png");

    painterQr.end();
}

// PRINT METODA
void Methods::NalepkaPrint(const QString& id, const QString& naziv,
                           const QString& kolicina, const QString& opombe,
                           const int& numCopies, const NacinTiska& nacin)
{
    // PRINTER SETUP
    QPrinter printer(QPrinter::HighResolution);
    //printer.setPageSize(QPrinter::A7);
    printer.setPrinterName("Rollo Printer");
    //printer.setPrinterName("Microsoft Print to PDF");
    printer.setPageSize(QPrinter::Custom);
    printer.setPaperSize(QSizeF(70,100),QPrinter::Millimeter);
    printer.setOrientation(QPrinter::Landscape);
    printer.setFullPage(false);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setNumCopies(numCopies);

    /*
    QList<QPrinterInfo> printerList=QPrinterInfo::availablePrinters();
            foreach (QPrinterInfo printerInfo, printerList)
                qDebug()<<printerInfo.printerName();
    */
    // QR SLIKA
    QPixmap map(m_qrVelikost,m_qrVelikost);

    drawQr(map, id, naziv, opombe);

    QRectF* boundingRect = {0};

    // SHRANJEVANJE V PDF
    m_nalepkaText = QDate::currentDate().toString("d_M_yyyy") + "-" + id + ".pdf";
    const QString nalepkaFile("./nalepke/" + m_nalepkaText);
    const QPageSize pageSize(QPageSize::A7);
    const QPageLayout pageLayout(pageSize, QPageLayout::Landscape,QMargins(0,0,0,0));

    QPdfWriter nalepkaPdf(nalepkaFile);
    nalepkaPdf.setPageSize(pageSize);
    nalepkaPdf.setPageLayout(pageLayout);

    // PAINTER
    QPainter painterTextNalepkePdf(&nalepkaPdf);
    QPainter painterTextNalepkePrint(&printer);

    // PDF okvir in Qr
    painterTextNalepkePdf.drawRect(m_nalepkaCentimeter,
                                   m_nalepkaCentimeter * 3,
                                   (m_sirinaNalepke * m_nalepkaCentimeter * 5.35) - (m_nalepkaCentimeter),
                                   (m_visinaNalepke * m_nalepkaCentimeter * 4.6) - (m_nalepkaCentimeter));

    painterTextNalepkePdf.drawPixmap(((m_sirinaNalepke * m_nalepkaCentimeter) * 2.2) - m_qrVelikost,
                                     ((m_visinaNalepke * m_nalepkaCentimeter) * 2.7) - m_qrVelikost,
                                       m_nalepkaCentimeter * 20,
                                       m_nalepkaCentimeter * 20, map);

    painterTextNalepkePrint.drawRect(m_nalepkaCentimeter/2,
                                     m_nalepkaCentimeter/2,
                                     m_sirinaNalepke - m_nalepkaCentimeter * 2,
                                     m_visinaNalepke - m_nalepkaCentimeter * 2);

    painterTextNalepkePrint.drawPixmap(((m_sirinaNalepke - m_nalepkaCentimeter * 4) / 2),
                                        m_visinaNalepke - (m_qrVelikost + m_nalepkaCentimeter / 2),
                                        m_nalepkaCentimeter*3,
                                        m_nalepkaCentimeter*3, map);

    QString header("Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");
    QFont bigFontStandard("Tahoma",20);
    QFont mediumFontStandard("Tahoma",11);
    QFont smallFontHeader("Tahoma",9);
    QFont bigFontNapis("Tahoma",50);
    QFont mediumFontNapis("Tahoma",35);
    QPointF textPos;
    textPos.setX(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2));
    // STANDARD NACIN (KO GRE ZA NALEPKO Z ID, NAZIVOM, KOLICINO IN OPOMBO
    if(nacin == NacinTiska::Standard) {
        // PDF id, naziv, kolicina
        painterTextNalepkePdf.setFont(smallFontHeader);
        textPos.setY(qreal(m_nalepkaCentimeter * 1.5));
        drawText(painterTextNalepkePdf, textPos, m_alignment, header, boundingRect);

        painterTextNalepkePdf.setFont(bigFontStandard);
        textPos.setY(qreal(m_nalepkaCentimeter * 5));
        drawText(painterTextNalepkePdf, textPos, m_alignment, "LIST IZDELKA", boundingRect);

        painterTextNalepkePdf.setFont(mediumFontStandard);
        textPos.setY(qreal(m_nalepkaCentimeter * 8));
        drawText(painterTextNalepkePdf, textPos, m_alignment, "ID izdelka: " + id, boundingRect);

        textPos.setY(qreal(m_nalepkaCentimeter * 10));
        drawText(painterTextNalepkePdf, textPos, m_alignment, "Naziv izdelka: " + naziv, boundingRect);

        textPos.setY(qreal(m_nalepkaCentimeter * 12));
        drawText(painterTextNalepkePdf, textPos, m_alignment, "Količina: " + kolicina, boundingRect);

        // PRINT id, naziv, kolicina

        painterTextNalepkePrint.setFont(smallFontHeader);
        textPos.setX(qreal(m_XofSet));
        textPos.setY(qreal(m_ofSetHeader));
        drawText(painterTextNalepkePrint, textPos, m_alignment, header, boundingRect);

        painterTextNalepkePrint.setFont(bigFontStandard);
        textPos.setY(qreal(m_ofSetListIzdelka_FirstLine));
        drawText(painterTextNalepkePrint, textPos, m_alignment, "LIST IZDELKA", boundingRect);

        painterTextNalepkePrint.setFont(mediumFontStandard);
        textPos.setY(qreal(m_ofSetId_OneLine));
        drawText(painterTextNalepkePrint, textPos, m_alignment, "ID izdelka: " + id, boundingRect);

        textPos.setY(qreal(m_ofSetNaziv_SecondLine));
        drawText(painterTextNalepkePrint, textPos, m_alignment, "Naziv izdelka: " + naziv, boundingRect);

        textPos.setY(qreal(m_ofSetKolicina));
        drawText(painterTextNalepkePrint, textPos, m_alignment, "Količina: " + kolicina, boundingRect);
    }
    // NAPIS (KO GRE SAMO ZA NAPIS ALI QR KODO
    else if(nacin == NacinTiska::Napis) {
        textPos.setX(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2));
        // PDF qr napis ali kratek text
        painterTextNalepkePdf.setFont(smallFontHeader);
        textPos.setY(qreal(m_nalepkaCentimeter * 1.5));
        drawText(painterTextNalepkePdf, textPos, m_alignment, header, boundingRect);

        QStringList list = naziv.split(";", QString::SkipEmptyParts);

        switch(list.length())
        {
            case 1: { painterTextNalepkePdf.setFont(bigFontNapis);
                      textPos.setY(qreal(m_nalepkaCentimeter * 7));
                      drawText(painterTextNalepkePdf, textPos, m_alignment, list.at(0), boundingRect);
                      break; }
            case 2: { painterTextNalepkePdf.setFont(mediumFontNapis);
                      textPos.setY(qreal(m_nalepkaCentimeter * 5));
                      drawText(painterTextNalepkePrint, textPos, m_alignment, list.at(0), boundingRect);

                      textPos.setY(qreal(m_nalepkaCentimeter * 10.5));
                      drawText(painterTextNalepkePdf, textPos, m_alignment, list.at(1), boundingRect);
                      break; }
            default: {painterTextNalepkePdf.setFont(bigFontNapis);
                      textPos.setY(qreal(m_nalepkaCentimeter * 7));
                      drawText(painterTextNalepkePdf, textPos, m_alignment, "QR KODA", boundingRect);
                      break; }
         }

        // PRINT qr napis ali kratek text
        painterTextNalepkePrint.setFont(smallFontHeader);
        textPos.setX(m_XofSet);
        textPos.setY(qreal(m_ofSetHeader));
        drawText(painterTextNalepkePrint, textPos, m_alignment, header, boundingRect);

        switch(list.length())
        {
            case 1: { painterTextNalepkePrint.setFont(bigFontNapis);
                      textPos.setY(qreal(m_ofSetId_OneLine));
                      drawText(painterTextNalepkePrint, textPos, m_alignment, list.at(0), boundingRect);
                      break; }
            case 2: { painterTextNalepkePrint.setFont(mediumFontNapis);
                      textPos.setY(qreal(m_ofSetListIzdelka_FirstLine));
                      drawText(painterTextNalepkePrint, textPos, m_alignment, list.at(0), boundingRect);

                      textPos.setY(qreal(m_ofSetNaziv_SecondLine));
                      drawText(painterTextNalepkePrint, textPos, m_alignment, list.at(1), boundingRect);
                      break; }
            default: {painterTextNalepkePrint.setFont(bigFontNapis);
                      textPos.setY(qreal(m_ofSetId_OneLine));
                      drawText(painterTextNalepkePrint, textPos, m_alignment, "QR KODA", boundingRect);
                      break; }
         }
    }

    painterTextNalepkePdf.end();
    painterTextNalepkePrint.end();
}

// ERROR CALL
void Methods::ErrorCall(QFile& fileName, const ErrorType& errorType)
{
    if(!fileName.open(QFile::ReadOnly | QFile::Text)) {
        QDialog *dialog = new QDialog(this);
        QLabel *label = new QLabel(this);
        QHBoxLayout *layout = new QHBoxLayout();
        QIcon icon(":icons/error.ico");
        dialog->setWindowTitle("Napaka");
        dialog->setWindowIcon(icon);
        switch (errorType) {
            case ErrorType::ReadError: label->setText("Error:\nfileName v funkciji Read() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::SearchError: label->setText("Error:\nfileName v funkciji Search() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::ProductCheckError: label->setText("Error:\nfileName v funkciji ProductCheck() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::DeleteError_ReadFile: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::DeleteError_DeleteFileText: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::DeleteError_InsertNewText: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::SaveError: label->setText("Error:\nfileName v funkciji on_actionShrani_nalepko_triggered() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::DefoultError: label->setText("Error:\nNeznana napaka\n Znova zaženi program!"); break;
            default: label->setText("Neznana napaka!\n Znova zaženi program!"); break;
        }
        layout->addWidget(label);
        dialog->setLayout(layout);
        dialog->exec();
    }
}
