#include "novanalepka.h"
#include "ui_novanalepka.h"

// KONSTRUKTOR
novaNalepka::novaNalepka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::novaNalepka),
    m_methods(new Methods()),
    m_map(m_methods->m_qrVelikost * 5, m_methods->m_qrVelikost * 5),
    m_mapSaved("qrNalepkePrint.png"),
    m_Qrtext(""),
    m_id(""),
    m_napis(""),
    m_QrKodaAliNapis("save"),
    m_nacinTiska(Methods::NacinTiska::Napis)
{
    ui->setupUi(this);
    this->setWindowTitle("Qr koda");
    ui->pushButton_natisni->setDisabled(true);
    ui->lineEdi_napis_prvaCrta->setMaxLength(9);
    ui->lineEdit_napis_drugaCrta->setMaxLength(9);
    ui->spinBox_kopijePrint->setValue(1); 
    m_numOfCopies = ui->spinBox_kopijePrint->value();
    ui->lineEdi_napis_prvaCrta->setFocus();
}

// DESTRUKTOR
novaNalepka::~novaNalepka()
{
    delete m_methods;
    delete ui;
}

// OVERRIDE RESET
void novaNalepka::Reset()
{
    ui->pushButton_natisni->setDisabled(true);
    ui->spinBox_kopijePrint->setValue(1);
    ui->lineEdi_napis_prvaCrta->clear();
    ui->lineEdit_napis_drugaCrta->clear();
    ui->textEdit_qrNapis->clear();
    ui->lineEdit_shrani->clear();
    ui->textEdit_qrNapis->setFocus();
}

void novaNalepka::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_P && event->modifiers().testFlag(Qt::ControlModifier) && ui->pushButton_natisni->isEnabled()) {
        m_id = ui->lineEdit_shrani->text();
        m_napis  = ui->lineEdi_napis_prvaCrta->text() + ";" + ui->lineEdit_napis_drugaCrta->text();
        m_Qrtext = ui->textEdit_qrNapis->toPlainText();
        m_numOfCopies = ui->spinBox_kopijePrint->value();
        m_methods->NalepkaPrint(m_id,
                                m_napis,
                                m_QrKodaAliNapis,
                                m_Qrtext,
                                m_numOfCopies,
                                m_nacinTiska);
        Reset();
    }
}

// NATISNI NALEPKO
void novaNalepka::on_pushButton_natisni_clicked()
{
    m_id = ui->lineEdit_shrani->text();
    m_napis  = ui->lineEdi_napis_prvaCrta->text() + ";" + ui->lineEdit_napis_drugaCrta->text();
    m_Qrtext = ui->textEdit_qrNapis->toPlainText();
    m_numOfCopies = ui->spinBox_kopijePrint->value();
    m_methods->NalepkaPrint(m_id,
                            m_napis,
                            m_QrKodaAliNapis,
                            m_Qrtext,
                            m_numOfCopies,
                            m_nacinTiska);
    Reset();
}

// TEKST V QR KODI
void novaNalepka::on_textEdit_qrNapis_textChanged()
{
    m_napis = ui->textEdit_qrNapis->toPlainText();
    if(m_napis == "")
        ui->pushButton_natisni->setDisabled(true);
    else if(m_napis.length() > 500) {
        m_napis.chop(1);
        QMessageBox::warning(this, "Qr koda predolga", "Qr koda lahko vsebuje največ 500 črk!");
        ui->textEdit_qrNapis->setPlainText(m_napis);
        QTextCursor tmpCursor = ui->textEdit_qrNapis->textCursor();
        ui->textEdit_qrNapis->setTextCursor(tmpCursor);
    }
    else
        ui->pushButton_natisni->setDisabled(false);
}

// NAPIS V NAPISU
void novaNalepka::on_lineEdit_napis_textChanged(const QString& arg1)
{
    m_napis = arg1;
    m_napis == "" ? ui->pushButton_natisni->setDisabled(true)
                  : ui->pushButton_natisni->setDisabled(false);
}
