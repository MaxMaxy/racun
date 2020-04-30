#include "placiloracuna.h"
#include "ui_placiloracuna.h"

PlaciloRacuna::PlaciloRacuna(QWidget *parent) :
    QDialog(parent), deleteRacun(false), m_quit(false), m_popravljen(false),
    ui(new Ui::PlaciloRacuna), m_stRacuna(""), m_currentDir(QDir::currentPath()), m_arhivRacun(m_currentDir + "/arhiv_novRacun.txt"), m_obveznost(false), m_show_child(false)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    ui->dateEdit->setDisplayFormat("d. M. yyyy");
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEdit->setFocus();
    this->setWindowTitle("Plačilo");
    this->setWindowFlags(Qt::Window);
    QRegularExpression regex("^[.,0123456789]*$");
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit->setValidator(validator);
    ui->lineEdit_TrenutnoPlacilo->setValidator(validator);
    QRegularExpression regealfabet("^[a-zA-Z0-9,@. -/&#čšžŠČŽ()+=]*$");
    QValidator *validatoralfabet = new QRegularExpressionValidator(regealfabet, this);
    ui->lineEdit_opombe->setValidator(validatoralfabet);
    ui->radioButton_placiloSkupaj->setChecked(true);
    ui->lineEdit_TrenutnoPlacilo->setEnabled(false);
}

PlaciloRacuna::~PlaciloRacuna() {
    delete ui;
}

void PlaciloRacuna::closeEvent(QCloseEvent *) {
    emit close_me();
}

void PlaciloRacuna::CloseChild() {
  m_show_child = false;
}

void PlaciloRacuna::setOpombe(QString &st_racuna, QString &opomba, QString &cena, QString &datum, QString &terObv, bool obveznost) {
    ui->label_terObv->setText("€" + terObv);
    ui->lineEdit_opombe->setText(opomba);
    ui->lineEdit->setText(cena.replace(',','.'));
    ui->dateEdit->setDate(QDate::fromString(datum));
    m_stRacuna = st_racuna;
    if(!obveznost)
        m_obveznost = true;
}

void PlaciloRacuna::setVars(QString &dobavitelj, QString &listina, QString &znesek, QString &datum, QString &opomba, QString &staraObveznost) {
    m_dobavitelj = dobavitelj;
    m_listina = listina;
    m_znesek = znesek;
    m_datum = datum;
    m_opomba = opomba;
    m_staraObveznost = staraObveznost;
}

QStringList PlaciloRacuna::on_pushButton_clicked() {
    QString opomba(ui->lineEdit_opombe->text());
    if(opomba != "") {
        opomba.replace(0,1,opomba.at(0).toUpper());
        while(opomba.at(opomba.length() - 1) == ' ') {
            opomba.remove(-1, 1);
        }
    }
    if(ui->radioButton_placiloEnkratno->isChecked()) {
        double cena = ui->lineEdit->text().remove("€").toDouble() + ui->lineEdit_TrenutnoPlacilo->text().replace(",",".").toDouble();
        m_list.append(QString::number(cena, 'f', 2));
    }
    else if(ui->radioButton_placiloSkupaj->isChecked()) {
        m_list.append(QString::number(ui->lineEdit->text().replace(',','.').toDouble(), 'f', 2));
    }
    m_list.append(ui->dateEdit->text());
    m_list.append(opomba);
    close();
    m_quit = false;
    return m_list;
}

void PlaciloRacuna::on_lineEdit_textChanged(const QString &arg1) {
    if(arg1.length() == 1 && (arg1.at(arg1.length()-1) == '.' || arg1.at(arg1.length()-1) == ',')) {
        ui->lineEdit->backspace();
    }
    if(arg1.contains("..") || arg1.contains(",,") || arg1.contains(",.") || arg1.contains(".,")) {
        ui->lineEdit->backspace();
    }
}

void PlaciloRacuna::on_lineEdit_opombe_textChanged(const QString &arg1) {
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_opombe->backspace();
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == '.') {
        ui->lineEdit_opombe->backspace();
        ui->lineEdit_opombe->backspace();
        ui->lineEdit_opombe->insert(".");
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ',') {
        ui->lineEdit_opombe->backspace();
        ui->lineEdit_opombe->backspace();
        ui->lineEdit_opombe->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == ' ') {
        ui->lineEdit_opombe->backspace();
    }
}

void PlaciloRacuna::reject() {
    m_quit = true;
    QDialog::reject();
}

void PlaciloRacuna::on_radioButton_placiloSkupaj_clicked()
{
    ui->lineEdit->setEnabled(true);
    ui->lineEdit_TrenutnoPlacilo->setEnabled(false);
}

void PlaciloRacuna::on_radioButton_placiloEnkratno_clicked()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_TrenutnoPlacilo->setEnabled(true);
}

void PlaciloRacuna::on_pushButton_2_clicked() {
    close();
}

void PlaciloRacuna::on_toolButton_clicked() {
    if(!m_obveznost) {
        VnosObveznosti obveznost;
        obveznost.PopraviObveznost(m_dobavitelj, m_listina, m_znesek, m_datum, m_opomba, m_staraObveznost);
        obveznost.setModal(true);
        this->hide();
        QObject::connect(&obveznost,SIGNAL(close_me()),this,SLOT(CloseChild()));
        m_show_child = true;
        while (m_show_child) {
            obveznost.exec();
        }
        this->show();
        m_quit = true;
    } else {
        QString allText("text");
        QString mText("");
        QString st_racuna("");
        QString opomba("");
        QString produkt_star("");
        QString produkt_nov("");
        QRegularExpression exp(";");
        QRegularExpression exp_second(": ");
        QStringList textList;
        QStringList tmp;
        NovRacun racun;
        racun.setModal(true);
        QFile mFile(m_arhivRacun);
        if(!mFile.open(QFile::Text | QFile::ReadOnly)) {
            qDebug() << "Error opening mFile for reading in popravi button";
            return;
        }
        while(!mFile.atEnd()) {
            mText = mFile.readLine();
            textList = mText.split(exp, QString::SkipEmptyParts);
            st_racuna = textList.at(3).toUtf8();
            st_racuna.remove("St_rac:").remove(' ');
            opomba = textList.at(12);
            if(st_racuna == m_stRacuna && (opomba != " Opomba: racun_je_bil_spremenjen!!!???? ") && (opomba != " Opomba: Racun_je_bil_spremenjen!!!???? ") && (opomba != " Opomba: racun_je_bil_odstranjen!!!???? ") && (opomba != " Opomba: Racun_je_bil_odstranjen!!!???? ")) {
                QTextStream in(&mFile);
                in.setCodec("UTF-8");
                in.seek(0);
                allText = in.readAll();
                mFile.close();
                produkt_star = mText.toUtf8();
                opomba = textList.at(12).toUtf8();
                produkt_nov = mText.replace(opomba, " Opomba: racun_je_bil_spremenjen!!!???? ").toUtf8();
                allText.replace(produkt_star, produkt_nov);

                if(!mFile.open(QFile::WriteOnly | QFile::Truncate)) {
                    qDebug() << "Error opening mFile for truncate in popravi button";
                    return;
                }
                mFile.flush();
                mFile.close();

                if(!mFile.open(QFile::WriteOnly | QFile::Text)) {
                    qDebug() << "Error opening mFile for writing in popravi button";
                    return;
                }
                in << allText.toUtf8();
                mFile.flush();
                mFile.close();

                QStringList tmp_list;
                QString tmp_string("");
                for(int i(0); i < textList.length(); i++) {
                    tmp_string = textList.at(i);
                    tmp_string.remove(-1,1);
                    if(textList.at(i) == "")
                        tmp_string = "/";
                    tmp_list.append(tmp_string.split(": ", QString::SkipEmptyParts));
                }
                racun.PopraviRacun(tmp_list.at(12).toUtf8(), tmp_list.at(4).toUtf8(), tmp_list.at(6).toUtf8(), tmp_list.at(8).toUtf8(), tmp_list.at(22).toUtf8(), tmp_list.at(14).toUtf8(), tmp_list.at(32).toUtf8());
                this->hide();
                QObject::connect(&racun,SIGNAL(close_me()),this,SLOT(CloseChild()));
                m_show_child = true;
                while (m_show_child) {
                    racun.exec();
                }
                m_popravljen = true;
                close_me();
                break;
            }
        }
        mFile.close();
    }
}

void PlaciloRacuna::on_toolButton_2_clicked() {
    if(m_obveznost) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("IZBRIS RAČUNA");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setBaseSize(QSize(250, 100));
        msgBox.setText(tr("Želiš izbrisati račun?"));
        QAbstractButton* pButtonYes = msgBox.addButton(tr("IZBRIS RAČUNA!"), QMessageBox::YesRole);
        msgBox.addButton(tr("Izhod"), QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton()==pButtonYes) {
            deleteRacun = true;
            PlaciloRacuna::close();
            m_quit = false;
        } else {
            PlaciloRacuna::close();
            m_quit = false;
        }
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("IZBRIS TERJATVE");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setBaseSize(QSize(250, 100));
        msgBox.setText(tr("Želiš izbrisati terjatev?"));
        QAbstractButton* pButtonYes = msgBox.addButton(tr("IZBRIS TERJATVE!"), QMessageBox::YesRole);
        msgBox.addButton(tr("Izhod"), QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton()==pButtonYes) {
            deleteRacun = true;
            PlaciloRacuna::close();
            m_quit = false;
        } else {
            PlaciloRacuna::close();
            m_quit = false;
        }
    }
    close();
}
