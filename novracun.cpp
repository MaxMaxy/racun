#include "novracun.h"
#include "ui_novracun.h"

NovRacun::NovRacun(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NovRacun), m_currentDir(QDir::currentPath()), m_fileName(m_currentDir + "/company_file.txt"), m_arhivNovRacun(m_currentDir + "/arhiv_novRacun.txt"), m_arhivStRacuna(m_currentDir + "/arhiv_stRacuna.txt"), m_arhivLogin(m_currentDir + "/arhiv_login.txt"), m_cNaziv(""), m_naslov(""), m_posta(""), m_ddv(""), m_email(""), m_numItems("1"), m_shrani(m_currentDir + "/settings.txt"), m_fileShrani(m_currentDir), m_count(true), m_total(0), m_itemsAdded(0), m_max_produktov(21), m_sprememba(false)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Nov račun");
    this->setWindowFlags(Qt::Window);
    ui->treeWidget_seznam->setColumnCount(3);
    ui->treeWidget_seznam->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget_seznam->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget_seznam->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget_seznam->header()->setStretchLastSection(false);
    ui->treeWidget_dodani->setColumnCount(4);
    ui->treeWidget_dodani->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget_dodani->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget_dodani->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget_dodani->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeWidget_dodani->header()->setStretchLastSection(false);
    ui->treeWidget_seznam->setRootIsDecorated(false);
    ui->treeWidget_dodani->setRootIsDecorated(false);
    ui->lineEdit_popust->setText("0");
    ui->label_skupaj->setText("€0.0");
    ui->label_ddv->setText("€0.0");
    ui->label_osnova->setText("€0.0");
    ui->dateEdit->setMinimumDate(QDate(2016,1,1));
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEdit_sklic->setMaxLength(35);
    ui->lineEdit->setMaxLength(60);
    QRegularExpression regealfabet("^[a-zA-Z0-9,@. -/&#čšžŠČŽ]*$");
    QValidator *validatoralfabet = new QRegularExpressionValidator(regealfabet, this);
    ui->lineEdit_sklic->setValidator(validatoralfabet);
    ui->lineEdit_stRacuna->setValidator(validatoralfabet);
    ui->lineEdit->setValidator(validatoralfabet);
    QIntValidator *intvalidator = new QIntValidator(0, 100, this);
    ui->lineEdit_popust->setValidator(intvalidator);
    QFile mFile(m_arhivStRacuna);
    ui->pushButton_isci->setFocus();
    ui->pushButton_isci->setVisible(false);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() constructor";
        return;
    }
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QString mText("");
    while(!in.atEnd())
    {
        mText = in.readLine();
    }
    mFile.close();
    int stRacuna = mText.toInt() + 1;
    if(stRacuna < 10)
        mText = "00" + QString::number(stRacuna);
    else if(stRacuna < 100)
        mText = "0" + QString::number(stRacuna);
    else
        mText = QString::number(stRacuna);
    ui->dateEdit->setDisplayFormat("yyyy");
    ui->lineEdit_stRacuna->setText(mText + "-" + ui->dateEdit->text());
    ui->dateEdit->setDisplayFormat("d. M. yyyy");
    AddItemsToCombo();
    Read();
    Shrani();
}

NovRacun::~NovRacun()
{
    delete ui;
}

void NovRacun::Arhiv(QString arhiv_file, QString stream)
{
    // odpri file
    QFile mFile(arhiv_file);
    // test ce je file odprt
    if(!mFile.open(QFile::WriteOnly | QFile::Append))
    {
        qDebug() << "Error opening mFile for writing in Arhiv()";
        return;
    }
    // stream za num file
    QTextStream out(&mFile);
    out.setCodec("UTF-8");
    out << stream << "\n";
    mFile.flush();
    mFile.close();
}

struct PostavkeRacuna
{
    QDomElement postavkeRacuna;
    QDomElement postavka;
    QDomElement stevilkaVrstice;
    QDomText stevilkaVrstice_text;
    QDomElement opisiArtiklov;
    QDomElement opisiArtiklov_1;
    QDomElement kodaOpisaArtikla;
    QDomText kodaOpisaArtikla_text;
    QDomElement opisArtikla;
    QDomElement opisArtikla_1;
    QDomText opisArtikla_1_text;
    QDomElement opisArtikla_2;
    QDomElement opisArtikla_2_1;
    QDomText opisArtikla_2_1_text;
    QDomElement opisArtikla_2_2;
    QDomText opisArtikla_2_2_text;
    QDomElement kolicinaArtikla;
    QDomElement vrstaKolicine;
    QDomText vrstaKolicine_text;
    QDomElement kolicina;
    QDomText kolicina_text;
    QDomElement enotaMere;
    QDomText enotaMere_text;
    QDomElement zneskiPostavke;
    QDomElement vrstaZneskaPostavke;
    QDomText vrstaZneskaPostavke_text;
    QDomElement znesekPostavke;
    QDomText znesekPostavke_text;
    QDomElement zneskiPostavke_1;
    QDomElement vrstaZneskaPostavke_1;
    QDomText vrstaZneskaPostavke_1_text;
    QDomElement znesekPostavke_1;
    QDomText znesekPostavke_1_text;
    QDomElement cenaPostavke;
    QDomElement vrstaCene;
    QDomText vrstaCene_text;
    QDomElement cena;
    QDomText cena_text;
    QDomElement davkiPostavke;
    QDomElement davkiNaPostavki;
    QDomElement vrstaDavkaPostavke;
    QDomText vrstaDavkaPostavke_text;
    QDomElement odstotekDavkaPostavke;
    QDomText odstotekDavkaPostavke_text;
    QDomElement zneskiDavkovPostavke;
    QDomElement vrstaZneskaDavkaPostavke;
    QDomText vrstaZneskaDavkaPostavke_text;
    QDomElement znesek;
    QDomText znesek_text;
    QDomElement zneskiDavkovPostavke_1;
    QDomElement vrstaZneskaDavkaPostavke_1;
    QDomText vrstaZneskaDavkaPostavke_1_text;
    QDomElement znesek_1;
    QDomText znesek_1_text;
    QDomElement odstotkiPostavk;
    QDomElement identifikator;
    QDomText identifikator_text;
    QDomElement vrstaOdstotkaPostavke;
    QDomText vrstaOdstotkaPostavke_text;
    QDomElement odstotekPostavke;
    QDomText odstotekPostavke_text;
    QDomElement vrstaZneskaOdstotka;
    QDomText vrstaZneskaOdstotka_text;
    QDomElement znesekOdstotka;
    QDomText znesekOdstotka_text;
};

void NovRacun::Shrani()
{
    QFile mFile(m_shrani);
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QRegularExpression exp(" ;");
    QString mLine("");
    QStringList mList;
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Shrani() in novracun.cpp";
        return;
    }
    else
    {
        mLine = in.readLine();
        mList = mLine.split(exp, QString::SkipEmptyParts);
        if(mList.at(0) == "Shrani v:")
        {
            m_fileShrani = mList.at(1);
            m_fileShrani.remove(0,1);
        }
    }
    mFile.close();
}

void NovRacun::MakeXML()
{
    QDomDocument document;

    // ROOT
    QDomElement root = document.createElement("IzdaniRacunEnostavni");
    document.appendChild(root);
    root.setAttribute("xsi:noNamespaceSchemaLocation", "http://www.gzs.si/e-poslovanje/sheme/eSLOG_1-6_EnostavniRacun.xsd");
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.setAttribute("xmlns:xds", "http://uri.etsi.org/01903/v1.1.1#");
    root.setAttribute("xmlns:ds", "http://www.w3.org/2000/09/xmldsig#");

    // RACUN
    QDomElement racun = document.createElement("Racun");
    racun.setAttribute("Id", "data");
    root.appendChild(racun);

    // GLAVA RACUNA
    QDomElement glavaRacuna = document.createElement("GlavaRacuna");
    racun.appendChild(glavaRacuna);
    QDomElement vrstaRacuna = document.createElement("VrstaRacuna");
    QDomElement stevilkaRacuna = document.createElement("StevilkaRacuna");
    QDomElement funkcijaRacuna = document.createElement("FunkcijaRacuna");
    QDomElement nacinPlacila = document.createElement("NacinPlacila");
    QDomElement kodaNamena = document.createElement("KodaNamena");
    QDomText vrstaRacunaText = document.createTextNode("380");
    vrstaRacuna.appendChild(vrstaRacunaText);
    QDomText stevilkaRacunaText = document.createTextNode(ui->lineEdit_stRacuna->text());
    stevilkaRacuna.appendChild(stevilkaRacunaText);
    QDomText funkcijaRacunaText = document.createTextNode("9");
    funkcijaRacuna.appendChild(funkcijaRacunaText);
    QDomText nacinPlacilaText = document.createTextNode("0");
    nacinPlacila.appendChild(nacinPlacilaText);
    QDomText kodaNamenaText = document.createTextNode("GDDS");
    kodaNamena.appendChild(kodaNamenaText);
    glavaRacuna.appendChild(vrstaRacuna);
    glavaRacuna.appendChild(stevilkaRacuna);
    glavaRacuna.appendChild(funkcijaRacuna);
    glavaRacuna.appendChild(nacinPlacila);
    glavaRacuna.appendChild(kodaNamena);

    // DATUM RACUNA 1
    QDomElement datumiRacuna_1 = document.createElement("DatumiRacuna");
    racun.appendChild(datumiRacuna_1);
    QDomElement vrstaDatuma_1 = document.createElement("VrstaDatuma");
    datumiRacuna_1.appendChild(vrstaDatuma_1);
    QDomText vrstaDatuma_1_text = document.createTextNode("137");
    vrstaDatuma_1.appendChild(vrstaDatuma_1_text);
    QDomElement datumRacuna_1_1 = document.createElement("DatumRacuna");
    datumiRacuna_1.appendChild(datumRacuna_1_1);
    ui->dateEdit->setDisplayFormat(QString("yyyy-MM-dd"));
    QDomText datumRacuna_1_1_text = document.createTextNode(ui->dateEdit->text() + "T00:00:00.0Z");
    datumRacuna_1_1.appendChild(datumRacuna_1_1_text);

    // DATUM RACUNA 2
    QDomElement datumiRacuna_2 = document.createElement("DatumiRacuna");
    racun.appendChild(datumiRacuna_2);
    QDomElement vrstaDatuma_2 = document.createElement("VrstaDatuma");
    datumiRacuna_2.appendChild(vrstaDatuma_2);
    QDomText vrstaDatuma_2_text = document.createTextNode("35");
    vrstaDatuma_2.appendChild(vrstaDatuma_2_text);
    QDomElement datumRacuna_2_1 = document.createElement("DatumRacuna");
    datumiRacuna_2.appendChild(datumRacuna_2_1);
    QDomText datumRacuna_2_1_text = document.createTextNode(ui->dateEdit->text() + "T00:00:00.0Z");
    datumRacuna_2_1.appendChild(datumRacuna_2_1_text);
    ui->dateEdit->setDisplayFormat(QString("d. M. yyyy"));

    // LOKACIJA
    QDomElement lokacije = document.createElement("Lokacije");
    racun.appendChild(lokacije);
    QDomElement vrstaLokacije = document.createElement("VrstaLokacije");
    lokacije.appendChild(vrstaLokacije);
    QDomText vrstaLokacijeText = document.createTextNode("91");
    vrstaLokacije.appendChild(vrstaLokacijeText);
    QDomElement nazivLokacije = document.createElement("NazivLokacije");
    lokacije.appendChild(nazivLokacije);
    QDomText nazivLokacijeText = document.createTextNode("Polzela");
    nazivLokacije.appendChild(nazivLokacijeText);

    // POLJUBNO BESEDILO 1
    QDomElement poljubnoBesedilo_1 = document.createElement("PoljubnoBesedilo");
    racun.appendChild(poljubnoBesedilo_1);
    QDomElement vrstaBesedila_1 = document.createElement("VrstaBesedila");
    poljubnoBesedilo_1.appendChild(vrstaBesedila_1);
    QDomText vrstaBesedilaText_1 = document.createTextNode("AAI");
    vrstaBesedila_1.appendChild(vrstaBesedilaText_1);
    QDomElement besedilo_1 = document.createElement("Besedilo");
    poljubnoBesedilo_1.appendChild(besedilo_1);
    QDomElement besediloTekst_1_1 = document.createElement("Tekst1");
    besedilo_1.appendChild(besediloTekst_1_1);
    QDomText text_za_tekst_1_1 = document.createTextNode("NASLOV_RACUNA");
    besediloTekst_1_1.appendChild(text_za_tekst_1_1);
    QDomElement besediloTekst_1_2 = document.createElement("Tekst2");
    besedilo_1.appendChild(besediloTekst_1_2);
    QDomText text_za_tekst_1_2 = document.createTextNode("Racun:" + ui->lineEdit_stRacuna->text());
    besediloTekst_1_2.appendChild(text_za_tekst_1_2);

    // POLJUBNO BESEDILO 2
    QDomElement poljubnoBesedilo_2 = document.createElement("PoljubnoBesedilo");
    racun.appendChild(poljubnoBesedilo_2);
    QDomElement vrstaBesedila_2 = document.createElement("VrstaBesedila");
    poljubnoBesedilo_2.appendChild(vrstaBesedila_2);
    QDomText vrstaBesedilaText_2 = document.createTextNode("AAI");
    vrstaBesedila_2.appendChild(vrstaBesedilaText_2);
    QDomElement besedilo_2 = document.createElement("Besedilo");
    poljubnoBesedilo_2.appendChild(besedilo_2);
    QDomElement besediloTekst_2_1 = document.createElement("Tekst1");
    besedilo_2.appendChild(besediloTekst_2_1);
    QDomText text_za_tekst_2_1 = document.createTextNode("DODATNI_TEKST");
    besediloTekst_2_1.appendChild(text_za_tekst_2_1);

    // POLJUBNO BESEDILO 3
    QDomElement poljubnoBesedilo_3 = document.createElement("PoljubnoBesedilo");
    racun.appendChild(poljubnoBesedilo_3);
    QDomElement vrstaBesedila_3 = document.createElement("VrstaBesedila");
    poljubnoBesedilo_3.appendChild(vrstaBesedila_3);
    QDomText vrstaBesedilaText_3 = document.createTextNode("ZZZ");
    vrstaBesedila_3.appendChild(vrstaBesedilaText_3);
    QDomElement besedilo_3 = document.createElement("Besedilo");
    poljubnoBesedilo_3.appendChild(besedilo_3);
    QDomElement besediloTekst_3_1 = document.createElement("Tekst1");
    besedilo_3.appendChild(besediloTekst_3_1);
    QDomText text_za_tekst_3_1 = document.createTextNode("DODATNI_OPIS_LISTINE");
    besediloTekst_3_1.appendChild(text_za_tekst_3_1);
    QDomElement besediloTekst_3_2 = document.createElement("Tekst2");
    besedilo_3.appendChild(besediloTekst_3_2);
    QDomText text_za_tekst_3_2 = document.createTextNode("Zaracunavamo vam");
    besediloTekst_3_2.appendChild(text_za_tekst_3_2);

    // POLJUBNO BESEDILO 4
    QDomElement poljubnoBesedilo_4 = document.createElement("PoljubnoBesedilo");
    racun.appendChild(poljubnoBesedilo_4);
    QDomElement vrstaBesedila_4 = document.createElement("VrstaBesedila");
    poljubnoBesedilo_4.appendChild(vrstaBesedila_4);
    QDomText vrstaBesedilaText_4 = document.createTextNode("ZZZ");
    vrstaBesedila_4.appendChild(vrstaBesedilaText_4);
    QDomElement besedilo_4 = document.createElement("Besedilo");
    poljubnoBesedilo_4.appendChild(besedilo_4);
    QDomElement besediloTekst_4_1 = document.createElement("Tekst1");
    besedilo_4.appendChild(besediloTekst_4_1);
    QDomText text_za_tekst_4_1 = document.createTextNode("PRODAJALEC");
    besediloTekst_4_1.appendChild(text_za_tekst_4_1);
    QDomElement besediloTekst_4_2 = document.createElement("Tekst2");
    besedilo_4.appendChild(besediloTekst_4_2);
    QDomText text_za_tekst_4_2 = document.createTextNode("ELRA SETI D.O.O.");
    besediloTekst_4_2.appendChild(text_za_tekst_4_2);

    // POLJUBNO BESEDILO 5
    QDomElement poljubnoBesedilo_5 = document.createElement("PoljubnoBesedilo");
    racun.appendChild(poljubnoBesedilo_5);
    QDomElement vrstaBesedila_5 = document.createElement("VrstaBesedila");
    poljubnoBesedilo_5.appendChild(vrstaBesedila_5);
    QDomText vrstaBesedilaText_5 = document.createTextNode("TXD");
    vrstaBesedila_5.appendChild(vrstaBesedilaText_5);
    QDomElement besedilo_5 = document.createElement("Besedilo");
    poljubnoBesedilo_5.appendChild(besedilo_5);
    QDomElement besediloTekst_5_1 = document.createElement("Tekst1");
    besedilo_5.appendChild(besediloTekst_5_1);
    QDomText text_za_tekst_5_1 = document.createTextNode("DAVCNI_TEKST");
    besediloTekst_5_1.appendChild(text_za_tekst_5_1);

    // PODATKI PODJETJA 1
    QDomElement podatkiPodjetja_1 = document.createElement("PodatkiPodjetja");
    racun.appendChild(podatkiPodjetja_1);
    QDomElement nazivNaslovPodjetja_1 = document.createElement("NazivNaslovPodjetja");
    podatkiPodjetja_1.appendChild(nazivNaslovPodjetja_1);
    QDomElement vrstaPartnerja_1 = document.createElement("VrstaPartnerja");
    nazivNaslovPodjetja_1.appendChild(vrstaPartnerja_1);
    QDomText vrstaPartnerjaText_1 = document.createTextNode("II");
    vrstaPartnerja_1.appendChild(vrstaPartnerjaText_1);
    QDomElement nazivPartnerja_1 = document.createElement("NazivPartnerja");
    nazivNaslovPodjetja_1.appendChild(nazivPartnerja_1);
    QDomElement nazivPartnerja_1_1 = document.createElement("NazivPartnerja1");
    nazivPartnerja_1.appendChild(nazivPartnerja_1_1);
    QDomText nazivPartnerja_1_1_text = document.createTextNode("ELRA SETI D.O.O.");
    nazivPartnerja_1_1.appendChild(nazivPartnerja_1_1_text);
    QDomElement ulica_1 = document.createElement("Ulica");
    nazivNaslovPodjetja_1.appendChild(ulica_1);
    QDomElement ulica_1_1 = document.createElement("Ulica1");
    ulica_1.appendChild(ulica_1_1);
    QDomText ulica_1_1_text = document.createTextNode("Andraz nad Polzelo 74 A");
    ulica_1_1.appendChild(ulica_1_1_text);
    QDomElement kraj_1 = document.createElement("Kraj");
    nazivNaslovPodjetja_1.appendChild(kraj_1);
    QDomText kraj_1_text = document.createTextNode("Polzela");
    kraj_1.appendChild(kraj_1_text);
    QDomElement nazivDrzave_1 = document.createElement("NazivDrzave");
    nazivNaslovPodjetja_1.appendChild(nazivDrzave_1);
    QDomText nazivDrzave_1_text = document.createTextNode("Slovenija");
    nazivDrzave_1.appendChild(nazivDrzave_1_text);
    QDomElement postnaStevilka_1 = document.createElement("PostnaStevilka");
    nazivNaslovPodjetja_1.appendChild(postnaStevilka_1);
    QDomText postnaStevilka_1_text = document.createTextNode("3313");
    postnaStevilka_1.appendChild(postnaStevilka_1_text);
    QDomElement kodaDrzave_1 = document.createElement("KodaDrzave");
    nazivNaslovPodjetja_1.appendChild(kodaDrzave_1);
    QDomText kodaDrzave_1_text = document.createTextNode("SI");
    kodaDrzave_1.appendChild(kodaDrzave_1_text);
    QDomElement financniPodatkiPodjetja_1 = document.createElement("FinancniPodatkiPodjetja");
    podatkiPodjetja_1.appendChild(financniPodatkiPodjetja_1);
    QDomElement bancniRacun_1 = document.createElement("BancniRacun");
    financniPodatkiPodjetja_1.appendChild(bancniRacun_1);
    QDomElement stevilkaBancnegaRacuna_1 = document.createElement("StevilkaBancnegaRacuna");
    bancniRacun_1.appendChild(stevilkaBancnegaRacuna_1);
    QDomText stevilkaBancnegaRacuna_1_text = document.createTextNode("SI56-3000-0001-1326-144");
    stevilkaBancnegaRacuna_1.appendChild(stevilkaBancnegaRacuna_1_text);
    QDomElement nazivBanke_1 = document.createElement("NazivBanke1");
    bancniRacun_1.appendChild(nazivBanke_1);
    QDomText nazivBanke_1_text = document.createTextNode("SBERBANK");
    nazivBanke_1.appendChild(nazivBanke_1_text);
    QDomElement bic_1 = document.createElement("BIC");
    bancniRacun_1.appendChild(bic_1);
    QDomText bic_1_text = document.createTextNode("SABRSI2XXXX");
    bic_1.appendChild(bic_1_text);
    QDomElement referencniPodatkiPodjetja_1 = document.createElement("ReferencniPodatkiPodjetja");
    podatkiPodjetja_1.appendChild(referencniPodatkiPodjetja_1);
    QDomElement vrstaPodatkaPodjetja_1 = document.createElement("VrstaPodatkaPodjetja");
    referencniPodatkiPodjetja_1.appendChild(vrstaPodatkaPodjetja_1);
    QDomText vrstaPodatkaPodjetja_1_text = document.createTextNode("VA");
    vrstaPodatkaPodjetja_1.appendChild(vrstaPodatkaPodjetja_1_text);
    QDomElement podatekPodjetja_1 = document.createElement("PodatekPodjetja");
    referencniPodatkiPodjetja_1.appendChild(podatekPodjetja_1);
    QDomText podatekPodjetja_1_text = document.createTextNode("SI83452010");
    podatekPodjetja_1.appendChild(podatekPodjetja_1_text);
    QDomElement kontaktiPodjetja_1 = document.createElement("KontaktiPodjetja");
    podatkiPodjetja_1.appendChild(kontaktiPodjetja_1);
    QDomElement komunikacije_1 = document.createElement("Komunikacije");
    kontaktiPodjetja_1.appendChild(komunikacije_1);
    QDomElement stevilkaKomunikacije_1 = document.createElement("StevilkaKomunikacije");
    komunikacije_1.appendChild(stevilkaKomunikacije_1);
    QDomText stevilkaKomunikacije_1_text = document.createTextNode("+386 (0)3 897 44 46");
    stevilkaKomunikacije_1.appendChild(stevilkaKomunikacije_1_text);
    QDomElement vrstaKomunikacije_1 = document.createElement("VrstaKomunikacije");
    komunikacije_1.appendChild(vrstaKomunikacije_1);
    QDomText vrstaKomunikacije_1_text = document.createTextNode("TE");
    vrstaKomunikacije_1.appendChild(vrstaKomunikacije_1_text);
    QDomElement komunikacije_1_1 = document.createElement("Komunikacije");
    kontaktiPodjetja_1.appendChild(komunikacije_1_1);
    QDomElement stevilkaKomunikacije_1_1 = document.createElement("StevilkaKomunikacije");
    komunikacije_1_1.appendChild(stevilkaKomunikacije_1_1);
    QDomText stevilkaKomunikacije_1_1_text = document.createTextNode("+386 (0)3 897 44 47");
    stevilkaKomunikacije_1_1.appendChild(stevilkaKomunikacije_1_1_text);
    QDomElement vrstaKomunikacije_1_1 = document.createElement("VrstaKomunikacije");
    komunikacije_1_1.appendChild(vrstaKomunikacije_1_1);
    QDomText vrstaKomunikacije_1_1_text = document.createTextNode("FX");
    vrstaKomunikacije_1_1.appendChild(vrstaKomunikacije_1_1_text);
    QDomElement komunikacije_1_2 = document.createElement("Komunikacije");
    kontaktiPodjetja_1.appendChild(komunikacije_1_2);
    QDomElement stevilkaKomunikacije_1_2 = document.createElement("StevilkaKomunikacije");
    komunikacije_1_2.appendChild(stevilkaKomunikacije_1_2);
    QDomText stevilkaKomunikacije_1_2_text = document.createTextNode("info@elraseti.si");
    stevilkaKomunikacije_1_2.appendChild(stevilkaKomunikacije_1_2_text);
    QDomElement vrstaKomunikacije_1_2 = document.createElement("VrstaKomunikacije");
    komunikacije_1_2.appendChild(vrstaKomunikacije_1_2);
    QDomText vrstaKomunikacije_1_2_text = document.createTextNode("EM");
    vrstaKomunikacije_1_2.appendChild(vrstaKomunikacije_1_2_text);

    // PODATKI PODJETJA 2
    QDomElement podatkiPodjetja_2 = document.createElement("PodatkiPodjetja");
    racun.appendChild(podatkiPodjetja_2);
    QDomElement nazivNaslovPodjetja_2 = document.createElement("NazivNaslovPodjetja");
    podatkiPodjetja_2.appendChild(nazivNaslovPodjetja_2);
    QDomElement vrstaPartnerja_2 = document.createElement("VrstaPartnerja");
    nazivNaslovPodjetja_2.appendChild(vrstaPartnerja_2);
    QDomText vrstaPartnerjaText_2 = document.createTextNode("IV");
    vrstaPartnerja_2.appendChild(vrstaPartnerjaText_2);
    QDomElement nazivPartnerja_2 = document.createElement("NazivPartnerja");
    nazivNaslovPodjetja_2.appendChild(nazivPartnerja_2);
    QDomElement nazivPartnerja_2_1 = document.createElement("NazivPartnerja1");
    nazivPartnerja_2.appendChild(nazivPartnerja_2_1);
    QDomText nazivPartnerja_2_1_text = document.createTextNode("Kronoterm d.o.o.");  // ------> line edit od stranke
    nazivPartnerja_2_1.appendChild(nazivPartnerja_2_1_text);
    QDomElement ulica_2 = document.createElement("Ulica");
    nazivNaslovPodjetja_2.appendChild(ulica_2);
    QDomElement ulica_2_1 = document.createElement("Ulica1");
    ulica_2.appendChild(ulica_2_1);
    QDomText ulica_2_1_text = document.createTextNode("Orla vas 27 A"); // -------------> naslov od stranke iz fila
    ulica_2_1.appendChild(ulica_2_1_text);
    QDomElement kraj_2 = document.createElement("Kraj");
    nazivNaslovPodjetja_2.appendChild(kraj_2);
    QDomText kraj_2_text = document.createTextNode("Braslovce");
    kraj_2.appendChild(kraj_2_text);
    QDomElement nazivDrzave_2 = document.createElement("NazivDrzave");
    nazivNaslovPodjetja_2.appendChild(nazivDrzave_2);
    QDomText nazivDrzave_2_text = document.createTextNode("Slovenija");
    nazivDrzave_2.appendChild(nazivDrzave_2_text);
    QDomElement postnaStevilka_2 = document.createElement("PostnaStevilka");
    nazivNaslovPodjetja_2.appendChild(postnaStevilka_2);
    QDomText postnaStevilka_2_text = document.createTextNode("3314");
    postnaStevilka_2.appendChild(postnaStevilka_2_text);
    QDomElement kodaDrzave_2 = document.createElement("KodaDrzave");
    nazivNaslovPodjetja_2.appendChild(kodaDrzave_2);
    QDomText kodaDrzave_2_text = document.createTextNode("SI");
    kodaDrzave_2.appendChild(kodaDrzave_2_text);
    QDomElement financniPodatkiPodjetja_2 = document.createElement("FinancniPodatkiPodjetja");
    podatkiPodjetja_2.appendChild(financniPodatkiPodjetja_2);
    QDomElement bancniRacun_2 = document.createElement("BancniRacun");
    financniPodatkiPodjetja_2.appendChild(bancniRacun_2);
    QDomElement stevilkaBancnegaRacuna_2 = document.createElement("StevilkaBancnegaRacuna");
    bancniRacun_2.appendChild(stevilkaBancnegaRacuna_2);
    QDomText stevilkaBancnegaRacuna_2_text = document.createTextNode("SI56-0510-0800-0006-394");
    stevilkaBancnegaRacuna_2.appendChild(stevilkaBancnegaRacuna_2_text);
    QDomElement bic_2 = document.createElement("BIC");
    bancniRacun_2.appendChild(bic_2);
    QDomText bic_2_text = document.createTextNode("ABANSI2XXXX");
    bic_2.appendChild(bic_2_text);
    QDomElement referencniPodatkiPodjetja_2 = document.createElement("ReferencniPodatkiPodjetja");
    podatkiPodjetja_2.appendChild(referencniPodatkiPodjetja_2);
    QDomElement vrstaPodatkaPodjetja_2 = document.createElement("VrstaPodatkaPodjetja");
    referencniPodatkiPodjetja_2.appendChild(vrstaPodatkaPodjetja_2);
    QDomText vrstaPodatkaPodjetja_2_text = document.createTextNode("VA");
    vrstaPodatkaPodjetja_2.appendChild(vrstaPodatkaPodjetja_2_text);
    QDomElement podatekPodjetja_2 = document.createElement("PodatekPodjetja");
    referencniPodatkiPodjetja_2.appendChild(podatekPodjetja_2);
    QDomText podatekPodjetja_2_text = document.createTextNode("SI64719839");
    podatekPodjetja_2.appendChild(podatekPodjetja_2_text);
    QDomElement kontaktiPodjetja_2 = document.createElement("KontaktiPodjetja");
    podatkiPodjetja_2.appendChild(kontaktiPodjetja_2);
    QDomElement komunikacije_2 = document.createElement("Komunikacije");
    kontaktiPodjetja_2.appendChild(komunikacije_2);
    QDomElement stevilkaKomunikacije_2 = document.createElement("StevilkaKomunikacije");
    komunikacije_2.appendChild(stevilkaKomunikacije_2);
    QDomText stevilkaKomunikacije_2_text = document.createTextNode("");
    stevilkaKomunikacije_2.appendChild(stevilkaKomunikacije_2_text);
    QDomElement vrstaKomunikacije_2 = document.createElement("VrstaKomunikacije");
    komunikacije_2.appendChild(vrstaKomunikacije_2);
    QDomText vrstaKomunikacije_2_text = document.createTextNode("TE");
    vrstaKomunikacije_2.appendChild(vrstaKomunikacije_2_text);
    QDomElement komunikacije_2_1 = document.createElement("Komunikacije");
    kontaktiPodjetja_2.appendChild(komunikacije_2_1);
    QDomElement stevilkaKomunikacije_2_1 = document.createElement("StevilkaKomunikacije");
    komunikacije_2_1.appendChild(stevilkaKomunikacije_2_1);
    QDomText stevilkaKomunikacije_2_1_text = document.createTextNode("");
    stevilkaKomunikacije_2_1.appendChild(stevilkaKomunikacije_2_1_text);
    QDomElement vrstaKomunikacije_2_1 = document.createElement("VrstaKomunikacije");
    komunikacije_2_1.appendChild(vrstaKomunikacije_2_1);
    QDomText vrstaKomunikacije_2_1_text = document.createTextNode("FX");
    vrstaKomunikacije_2_1.appendChild(vrstaKomunikacije_2_1_text);
    QDomElement komunikacije_2_2 = document.createElement("Komunikacije");
    kontaktiPodjetja_2.appendChild(komunikacije_2_2);
    QDomElement stevilkaKomunikacije_2_2 = document.createElement("StevilkaKomunikacije");
    komunikacije_2_2.appendChild(stevilkaKomunikacije_2_2);
    QDomText stevilkaKomunikacije_2_2_text = document.createTextNode("");
    stevilkaKomunikacije_2_2.appendChild(stevilkaKomunikacije_2_2_text);
    QDomElement vrstaKomunikacije_2_2 = document.createElement("VrstaKomunikacije");
    komunikacije_2_2.appendChild(vrstaKomunikacije_2_2);
    QDomText vrstaKomunikacije_2_2_text = document.createTextNode("EM");
    vrstaKomunikacije_2_2.appendChild(vrstaKomunikacije_2_2_text);

    // PODATKI PODJETJA 3
    QDomElement podatkiPodjetja_3 = document.createElement("PodatkiPodjetja");
    racun.appendChild(podatkiPodjetja_3);
    QDomElement nazivNaslovPodjetja_3 = document.createElement("NazivNaslovPodjetja");
    podatkiPodjetja_3.appendChild(nazivNaslovPodjetja_3);
    QDomElement vrstaPartnerja_3 = document.createElement("VrstaPartnerja");
    nazivNaslovPodjetja_3.appendChild(vrstaPartnerja_3);
    QDomText vrstaPartnerjaText_3 = document.createTextNode("BY");
    vrstaPartnerja_3.appendChild(vrstaPartnerjaText_3);
    QDomElement nazivPartnerja_3 = document.createElement("NazivPartnerja");
    nazivNaslovPodjetja_3.appendChild(nazivPartnerja_3);
    QDomElement nazivPartnerja_3_1 = document.createElement("NazivPartnerja1");
    nazivPartnerja_3.appendChild(nazivPartnerja_3_1);
    QDomText nazivPartnerja_3_1_text = document.createTextNode("Kronoterm d.o.o.");  // ------> line edit od stranke
    nazivPartnerja_3_1.appendChild(nazivPartnerja_3_1_text);
    QDomElement ulica_3 = document.createElement("Ulica");
    nazivNaslovPodjetja_3.appendChild(ulica_3);
    QDomElement ulica_3_1 = document.createElement("Ulica1");
    ulica_3.appendChild(ulica_3_1);
    QDomText ulica_3_1_text = document.createTextNode("Orla vas 27 A"); // -------------> naslov od stranke iz fila
    ulica_3_1.appendChild(ulica_3_1_text);
    QDomElement kraj_3 = document.createElement("Kraj");
    nazivNaslovPodjetja_3.appendChild(kraj_3);
    QDomText kraj_3_text = document.createTextNode("Braslovce");
    kraj_3.appendChild(kraj_3_text);
    QDomElement nazivDrzave_3 = document.createElement("NazivDrzave");
    nazivNaslovPodjetja_3.appendChild(nazivDrzave_3);
    QDomText nazivDrzave_3_text = document.createTextNode("Slovenija");
    nazivDrzave_3.appendChild(nazivDrzave_3_text);
    QDomElement postnaStevilka_3 = document.createElement("PostnaStevilka");
    nazivNaslovPodjetja_3.appendChild(postnaStevilka_3);
    QDomText postnaStevilka_3_text = document.createTextNode("3314");
    postnaStevilka_3.appendChild(postnaStevilka_3_text);
    QDomElement kodaDrzave_3 = document.createElement("KodaDrzave");
    nazivNaslovPodjetja_3.appendChild(kodaDrzave_3);
    QDomText kodaDrzave_3_text = document.createTextNode("SI");
    kodaDrzave_3.appendChild(kodaDrzave_3_text);
    QDomElement financniPodatkiPodjetja_3 = document.createElement("FinancniPodatkiPodjetja");
    podatkiPodjetja_3.appendChild(financniPodatkiPodjetja_3);
    QDomElement bancniRacun_3 = document.createElement("BancniRacun");
    financniPodatkiPodjetja_3.appendChild(bancniRacun_3);
    QDomElement stevilkaBancnegaRacuna_3 = document.createElement("StevilkaBancnegaRacuna");
    bancniRacun_3.appendChild(stevilkaBancnegaRacuna_3);
    QDomText stevilkaBancnegaRacuna_3_text = document.createTextNode("SI56-0510-0800-0006-394");
    stevilkaBancnegaRacuna_3.appendChild(stevilkaBancnegaRacuna_3_text);
    QDomElement bic_3 = document.createElement("BIC");
    bancniRacun_3.appendChild(bic_3);
    QDomText bic_3_text = document.createTextNode("ABANSI2XXXX");
    bic_3.appendChild(bic_3_text);
    QDomElement referencniPodatkiPodjetja_3 = document.createElement("ReferencniPodatkiPodjetja");
    podatkiPodjetja_3.appendChild(referencniPodatkiPodjetja_3);
    QDomElement vrstaPodatkaPodjetja_3 = document.createElement("VrstaPodatkaPodjetja");
    referencniPodatkiPodjetja_3.appendChild(vrstaPodatkaPodjetja_3);
    QDomText vrstaPodatkaPodjetja_3_text = document.createTextNode("VA");
    vrstaPodatkaPodjetja_3.appendChild(vrstaPodatkaPodjetja_3_text);
    QDomElement podatekPodjetja_3 = document.createElement("PodatekPodjetja");
    referencniPodatkiPodjetja_3.appendChild(podatekPodjetja_3);
    QDomText podatekPodjetja_3_text = document.createTextNode("SI64719839");
    podatekPodjetja_3.appendChild(podatekPodjetja_3_text);
    QDomElement kontaktiPodjetja_3 = document.createElement("KontaktiPodjetja");
    podatkiPodjetja_3.appendChild(kontaktiPodjetja_3);
    QDomElement komunikacije_3 = document.createElement("Komunikacije");
    kontaktiPodjetja_3.appendChild(komunikacije_3);
    QDomElement stevilkaKomunikacije_3 = document.createElement("StevilkaKomunikacije");
    komunikacije_3.appendChild(stevilkaKomunikacije_3);
    QDomText stevilkaKomunikacije_3_text = document.createTextNode("");
    stevilkaKomunikacije_3.appendChild(stevilkaKomunikacije_3_text);
    QDomElement vrstaKomunikacije_3 = document.createElement("VrstaKomunikacije");
    komunikacije_3.appendChild(vrstaKomunikacije_3);
    QDomText vrstaKomunikacije_3_text = document.createTextNode("TE");
    vrstaKomunikacije_3.appendChild(vrstaKomunikacije_3_text);
    QDomElement komunikacije_3_1 = document.createElement("Komunikacije");
    kontaktiPodjetja_3.appendChild(komunikacije_3_1);
    QDomElement stevilkaKomunikacije_3_1 = document.createElement("StevilkaKomunikacije");
    komunikacije_3_1.appendChild(stevilkaKomunikacije_3_1);
    QDomText stevilkaKomunikacije_3_1_text = document.createTextNode("");
    stevilkaKomunikacije_3_1.appendChild(stevilkaKomunikacije_3_1_text);
    QDomElement vrstaKomunikacije_3_1 = document.createElement("VrstaKomunikacije");
    komunikacije_3_1.appendChild(vrstaKomunikacije_3_1);
    QDomText vrstaKomunikacije_3_1_text = document.createTextNode("FX");
    vrstaKomunikacije_3_1.appendChild(vrstaKomunikacije_3_1_text);
    QDomElement komunikacije_3_2 = document.createElement("Komunikacije");
    kontaktiPodjetja_3.appendChild(komunikacije_3_2);
    QDomElement stevilkaKomunikacije_3_2 = document.createElement("StevilkaKomunikacije");
    komunikacije_3_2.appendChild(stevilkaKomunikacije_3_2);
    QDomText stevilkaKomunikacije_3_2_text = document.createTextNode("");
    stevilkaKomunikacije_3_2.appendChild(stevilkaKomunikacije_3_2_text);
    QDomElement vrstaKomunikacije_3_2 = document.createElement("VrstaKomunikacije");
    komunikacije_3_2.appendChild(vrstaKomunikacije_3_2);
    QDomText vrstaKomunikacije_3_2_text = document.createTextNode("EM");
    vrstaKomunikacije_3_2.appendChild(vrstaKomunikacije_3_2_text);

    // VALUTA
    QDomElement valuta = document.createElement("Valuta");
    racun.appendChild(valuta);
    QDomElement vrstaValuteRacuna = document.createElement("VrstaValuteRacuna");
    valuta.appendChild(vrstaValuteRacuna);
    QDomText vrstaValuteRacuna_text = document.createTextNode("2");
    vrstaValuteRacuna.appendChild(vrstaValuteRacuna_text);
    QDomElement kodaValute = document.createElement("KodaValute");
    valuta.appendChild(kodaValute);
    QDomText kodaValute_text = document.createTextNode("EUR");
    kodaValute.appendChild(kodaValute_text);

    // PLACILNI POGOJI
    QDomElement placilniPogoji = document.createElement("PlacilniPogoji");
    racun.appendChild(placilniPogoji);
    QDomElement podatkiOrokih = document.createElement("PodatkiORokih");
    placilniPogoji.appendChild(podatkiOrokih);
    QDomElement vrstaPogoja = document.createElement("VrstaPogoja");
    podatkiOrokih.appendChild(vrstaPogoja);
    QDomText vrstaPogoja_text = document.createTextNode("3");
    vrstaPogoja.appendChild(vrstaPogoja_text);
    QDomElement placilniRoki = document.createElement("PlacilniRoki");
    placilniPogoji.appendChild(placilniRoki);
    QDomElement vrstaDatumaPlacilnegaRoka = document.createElement("VrstaDatumaPlacilnegaRoka");
    placilniRoki.appendChild(vrstaDatumaPlacilnegaRoka);
    QDomText vrstaDatumaPlacilnegaRoka_text = document.createTextNode("13");
    vrstaDatumaPlacilnegaRoka.appendChild(vrstaDatumaPlacilnegaRoka_text);
    QDomElement datum = document.createElement("Datum");
    placilniRoki.appendChild(datum);
    QDate inputDate = ui->dateEdit->date();
    QDomText datum_text = document.createTextNode(inputDate.addDays(45).toString("yyyy-MM-dd") + "T00:00:00.0Z"); // --------> datum + 45 dni
    ui->dateEdit->setDisplayFormat("d. M. yyyy");
    datum.appendChild(datum_text);

    double popust(0);
    double znesekPostavke(0);
    double skupnaCena(0);
    double popust_total(0);
    // POSTAVKE RACUNA
    QVector<PostavkeRacuna> postavkeVec;
    for(int i(0); i < m_itemsAdded; i++)
    {
        PostavkeRacuna postavkaStruct;
        znesekPostavke = ui->treeWidget_dodani->topLevelItem(i)->text(2).remove("€").toDouble() * ui->treeWidget_dodani->topLevelItem(i)->text(3).toDouble();
        popust = znesekPostavke * (ui->lineEdit_popust->text().toDouble() / 100.0);
        popust_total += popust;
        skupnaCena = (znesekPostavke - popust) * 1.22;
        postavkaStruct.postavkeRacuna = document.createElement("PostavkeRacuna");
        postavkaStruct.postavka = document.createElement("Postavka");
        postavkaStruct.stevilkaVrstice = document.createElement("StevilkaVrstice");
        postavkaStruct.stevilkaVrstice_text = document.createTextNode(QString::number(i+1));
        postavkaStruct.opisiArtiklov = document.createElement("OpisiArtiklov");
        postavkaStruct.opisiArtiklov_1 = document.createElement("OpisiArtiklov");
        postavkaStruct.opisArtikla = document.createElement("OpisArtikla");
        postavkaStruct.opisArtikla_1 = document.createElement("OpisArtikla1");
        postavkaStruct.opisArtikla_1_text = document.createTextNode(ui->treeWidget_dodani->topLevelItem(i)->text(1));
        postavkaStruct.kodaOpisaArtikla = document.createElement("KodaOpisaArtikla");
        postavkaStruct.kodaOpisaArtikla_text = document.createTextNode("F");
        postavkaStruct.opisArtikla_2 = document.createElement("OpisArtikla");
        postavkaStruct.opisArtikla_2_1 = document.createElement("OpisArtikla1");
        postavkaStruct.opisArtikla_2_1_text = document.createTextNode("OPIS_ENOTE_MERE");
        postavkaStruct.opisArtikla_2_2 = document.createElement("OpisArtikla2");
        postavkaStruct.opisArtikla_2_2_text = document.createTextNode("PCE");
        postavkaStruct.kolicinaArtikla = document.createElement("KolicinaArtikla");
        postavkaStruct.vrstaKolicine = document.createElement("VrstaKolicine");
        postavkaStruct.vrstaKolicine_text = document.createTextNode("47");
        postavkaStruct.kolicina = document.createElement("Kolicina");
        postavkaStruct.kolicina_text = document.createTextNode(ui->treeWidget_dodani->topLevelItem(i)->text(3));
        postavkaStruct.enotaMere = document.createElement("EnotaMere");
        postavkaStruct.enotaMere_text = document.createTextNode("PCE");
        postavkaStruct.zneskiPostavke = document.createElement("ZneskiPostavke");
        postavkaStruct.vrstaZneskaPostavke = document.createElement("VrstaZneskaPostavke");
        postavkaStruct.vrstaZneskaPostavke_text = document.createTextNode("203");
        postavkaStruct.znesekPostavke = document.createElement("ZnesekPostavke");
        postavkaStruct.znesekPostavke_text = document.createTextNode(QString::number(znesekPostavke, 'f', 2));
        postavkaStruct.zneskiPostavke_1 = document.createElement("ZneskiPostavke");
        postavkaStruct.vrstaZneskaPostavke_1 = document.createElement("VrstaZneskaPostavke");
        postavkaStruct.vrstaZneskaPostavke_1_text = document.createTextNode("38");
        postavkaStruct.znesekPostavke_1 = document.createElement("ZnesekPostavke");
        postavkaStruct.znesekPostavke_1_text = document.createTextNode(QString::number(skupnaCena, 'f', 2));
        postavkaStruct.cenaPostavke = document.createElement("CenaPostavke");
        postavkaStruct.vrstaCene = document.createElement("VrstaCene");
        postavkaStruct.vrstaCene_text = document.createTextNode("AAA");
        postavkaStruct.cena = document.createElement("Cena");
        postavkaStruct.cena_text = document.createTextNode(ui->treeWidget_dodani->topLevelItem(i)->text(2).remove("€"));
        postavkaStruct.davkiPostavke = document.createElement("DavkiPostavke");
        postavkaStruct.davkiNaPostavki = document.createElement("DavkiNaPostavki");
        postavkaStruct.vrstaDavkaPostavke = document.createElement("VrstaDavkaPostavke");
        postavkaStruct.vrstaDavkaPostavke_text = document.createTextNode("VAT");
        postavkaStruct.odstotekDavkaPostavke = document.createElement("OdstotekDavkaPostavke");
        postavkaStruct.odstotekDavkaPostavke_text = document.createTextNode("22");
        postavkaStruct.zneskiDavkovPostavke = document.createElement("ZneskiDavkovPostavke");
        postavkaStruct.vrstaZneskaDavkaPostavke = document.createElement("VrstaZneskaDavkaPostavke");
        postavkaStruct.vrstaZneskaDavkaPostavke_text = document.createTextNode("125");
        postavkaStruct.znesek = document.createElement("Znesek");
        postavkaStruct.znesek_text = document.createTextNode(QString::number(znesekPostavke - popust, 'f', 2));
        postavkaStruct.zneskiDavkovPostavke_1 = document.createElement("ZneskiDavkovPostavke");
        postavkaStruct.vrstaZneskaDavkaPostavke_1 = document.createElement("VrstaZneskaDavkaPostavke");
        postavkaStruct.vrstaZneskaDavkaPostavke_1_text = document.createTextNode("124");
        postavkaStruct.znesek_1 = document.createElement("Znesek");
        postavkaStruct.znesek_1_text = document.createTextNode(QString::number((znesekPostavke - popust)*0.22, 'f', 2));
        postavkaStruct.odstotkiPostavk = document.createElement("OdstotkiPostavk");
        postavkaStruct.identifikator = document.createElement("Identifikator");
        postavkaStruct.identifikator_text = document.createTextNode("A");
        postavkaStruct.vrstaOdstotkaPostavke = document.createElement("VrstaOdstotkaPostavke");
        postavkaStruct.vrstaOdstotkaPostavke_text = document.createTextNode("1");
        postavkaStruct.odstotekPostavke = document.createElement("OdstotekPostavke");
        postavkaStruct.odstotekPostavke_text = document.createTextNode(ui->lineEdit_popust->text());
        postavkaStruct.vrstaZneskaOdstotka = document.createElement("VrstaZneskaOdstotka");
        postavkaStruct.vrstaZneskaOdstotka_text = document.createTextNode("204");
        postavkaStruct.znesekOdstotka = document.createElement("ZnesekOdstotka");
        postavkaStruct.znesekOdstotka_text = document.createTextNode(QString::number(popust, 'f', 2));

        postavkeVec.append(postavkaStruct);
        racun.appendChild(postavkeVec.at(i).postavkeRacuna);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).postavka);
        postavkaStruct.postavka.appendChild(postavkeVec.at(i).stevilkaVrstice);
        postavkaStruct.stevilkaVrstice.appendChild(postavkeVec.at(i).stevilkaVrstice_text);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).opisiArtiklov);
        postavkaStruct.opisiArtiklov.appendChild(postavkeVec.at(i).kodaOpisaArtikla);
        postavkaStruct.kodaOpisaArtikla.appendChild(postavkeVec.at(i).kodaOpisaArtikla_text);
        postavkaStruct.opisiArtiklov.appendChild(postavkeVec.at(i).opisArtikla);
        postavkaStruct.opisArtikla.appendChild(postavkeVec.at(i).opisArtikla_1);
        postavkaStruct.opisArtikla_1.appendChild(postavkeVec.at(i).opisArtikla_1_text);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).opisiArtiklov_1);
        postavkaStruct.opisiArtiklov_1.appendChild(postavkeVec.at(i).opisArtikla_2);
        postavkaStruct.opisArtikla_2.appendChild(postavkeVec.at(i).opisArtikla_2_1);
        postavkaStruct.opisArtikla_2_1.appendChild(postavkeVec.at(i).opisArtikla_2_1_text);
        postavkaStruct.opisArtikla_2.appendChild(postavkeVec.at(i).opisArtikla_2_2);
        postavkaStruct.opisArtikla_2_2.appendChild(postavkeVec.at(i).opisArtikla_2_2_text);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).kolicinaArtikla);
        postavkaStruct.kolicinaArtikla.appendChild(postavkeVec.at(i).vrstaKolicine);
        postavkaStruct.vrstaKolicine.appendChild(postavkeVec.at(i).vrstaKolicine_text);
        postavkaStruct.kolicinaArtikla.appendChild(postavkeVec.at(i).kolicina);
        postavkaStruct.kolicina.appendChild(postavkeVec.at(i).kolicina_text);
        postavkaStruct.kolicinaArtikla.appendChild(postavkeVec.at(i).enotaMere);
        postavkaStruct.enotaMere.appendChild(postavkeVec.at(i).enotaMere_text);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).zneskiPostavke);
        postavkaStruct.zneskiPostavke.appendChild(postavkeVec.at(i).vrstaZneskaPostavke);
        postavkaStruct.vrstaZneskaPostavke.appendChild(postavkeVec.at(i).vrstaZneskaPostavke_text);
        postavkaStruct.zneskiPostavke.appendChild(postavkeVec.at(i).znesekPostavke);
        postavkaStruct.znesekPostavke.appendChild(postavkeVec.at(i).znesekPostavke_text);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).zneskiPostavke_1);
        postavkaStruct.zneskiPostavke_1.appendChild(postavkeVec.at(i).vrstaZneskaPostavke_1);
        postavkaStruct.vrstaZneskaPostavke_1.appendChild(postavkeVec.at(i).vrstaZneskaPostavke_1_text);
        postavkaStruct.zneskiPostavke_1.appendChild(postavkeVec.at(i).znesekPostavke_1);
        postavkaStruct.znesekPostavke_1.appendChild(postavkeVec.at(i).znesekPostavke_1_text);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).cenaPostavke);
        postavkaStruct.cenaPostavke.appendChild(postavkeVec.at(i).vrstaCene);
        postavkaStruct.vrstaCene.appendChild(postavkeVec.at(i).vrstaCene_text);
        postavkaStruct.cenaPostavke.appendChild(postavkeVec.at(i).cena);
        postavkaStruct.cena.appendChild(postavkeVec.at(i).cena_text);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).davkiPostavke);
        postavkaStruct.davkiPostavke.appendChild(postavkeVec.at(i).davkiNaPostavki);
        postavkaStruct.davkiNaPostavki.appendChild(postavkeVec.at(i).vrstaDavkaPostavke);
        postavkaStruct.vrstaDavkaPostavke.appendChild(postavkeVec.at(i).vrstaDavkaPostavke_text);
        postavkaStruct.davkiNaPostavki.appendChild(postavkeVec.at(i).odstotekDavkaPostavke);
        postavkaStruct.odstotekDavkaPostavke.appendChild(postavkeVec.at(i).odstotekDavkaPostavke_text);
        postavkaStruct.davkiPostavke.appendChild(postavkeVec.at(i).zneskiDavkovPostavke);
        postavkaStruct.zneskiDavkovPostavke.appendChild(postavkeVec.at(i).vrstaZneskaDavkaPostavke);
        postavkaStruct.vrstaZneskaDavkaPostavke.appendChild(postavkeVec.at(i).vrstaZneskaDavkaPostavke_text);
        postavkaStruct.zneskiDavkovPostavke.appendChild(postavkeVec.at(i).znesek);
        postavkaStruct.znesek.appendChild(postavkeVec.at(i).znesek_text);
        postavkaStruct.davkiPostavke.appendChild(postavkeVec.at(i).zneskiDavkovPostavke_1);
        postavkaStruct.zneskiDavkovPostavke_1.appendChild(postavkeVec.at(i).vrstaZneskaDavkaPostavke_1);
        postavkaStruct.vrstaZneskaDavkaPostavke_1.appendChild(postavkeVec.at(i).vrstaZneskaDavkaPostavke_1_text);
        postavkaStruct.zneskiDavkovPostavke_1.appendChild(postavkeVec.at(i).znesek_1);
        postavkaStruct.znesek_1.appendChild(postavkeVec.at(i).znesek_1_text);
        postavkaStruct.postavkeRacuna.appendChild(postavkeVec.at(i).odstotkiPostavk);
        postavkaStruct.odstotkiPostavk.appendChild(postavkeVec.at(i).identifikator);
        postavkaStruct.identifikator.appendChild(postavkeVec.at(i).identifikator_text);
        postavkaStruct.odstotkiPostavk.appendChild(postavkeVec.at(i).vrstaOdstotkaPostavke);
        postavkaStruct.vrstaOdstotkaPostavke.appendChild(postavkeVec.at(i).vrstaOdstotkaPostavke_text);
        postavkaStruct.odstotkiPostavk.appendChild(postavkeVec.at(i).odstotekPostavke);
        postavkaStruct.odstotekPostavke.appendChild(postavkeVec.at(i).odstotekPostavke_text);
        postavkaStruct.odstotkiPostavk.appendChild(postavkeVec.at(i).vrstaZneskaOdstotka);
        postavkaStruct.vrstaZneskaOdstotka.appendChild(postavkeVec.at(i).vrstaZneskaOdstotka_text);
        postavkaStruct.odstotkiPostavk.appendChild(postavkeVec.at(i).znesekOdstotka);
        postavkaStruct.znesekOdstotka.appendChild(postavkeVec.at(i).znesekOdstotka_text);
    }

    // POVZETEK DAVKOV 1
    QDomElement povzetekDavkov = document.createElement("PovzetekDavkovRacuna");
    racun.appendChild(povzetekDavkov);
    QDomElement davkiRacuna = document.createElement("DavkiRacuna");
    povzetekDavkov.appendChild(davkiRacuna);
    QDomElement vrstaDavka = document.createElement("VrstaDavka");
    QDomText vrstaDavka_text = document.createTextNode("VAT");
    vrstaDavka.appendChild(vrstaDavka_text);
    davkiRacuna.appendChild(vrstaDavka);
    QDomElement odstotekDavka = document.createElement("OdstotekDavka");
    QDomText odstotekDavka_text = document.createTextNode("22");
    odstotekDavka.appendChild(odstotekDavka_text);
    davkiRacuna.appendChild(odstotekDavka);
    QDomElement zneskiDavkov = document.createElement("ZneskiDavkov");
    povzetekDavkov.appendChild(zneskiDavkov);
    QDomElement vrstaZneskaDavka = document.createElement("VrstaZneskaDavka");
    QDomText vrstaZneskaDavka_text = document.createTextNode("125");
    vrstaZneskaDavka.appendChild(vrstaZneskaDavka_text);
    zneskiDavkov.appendChild(vrstaZneskaDavka);
    QDomElement znesekDavka = document.createElement("ZnesekDavka");
    QDomText znesekDavka_text = document.createTextNode(ui->label_popust->text().remove("€"));
    znesekDavka.appendChild(znesekDavka_text);
    zneskiDavkov.appendChild(znesekDavka);
    QDomElement zneskiDavkov_1 = document.createElement("ZneskiDavkov");
    povzetekDavkov.appendChild(zneskiDavkov_1);
    QDomElement vrstaZneskaDavka_1 = document.createElement("VrstaZneskaDavka");
    QDomText vrstaZneskaDavka_1_text = document.createTextNode("124");
    vrstaZneskaDavka_1.appendChild(vrstaZneskaDavka_1_text);
    zneskiDavkov_1.appendChild(vrstaZneskaDavka_1);
    QDomElement znesekDavka_1 = document.createElement("ZnesekDavka");
    QDomText znesekDavka_1_text = document.createTextNode(ui->label_ddv->text().remove("€"));
    znesekDavka_1.appendChild(znesekDavka_1_text);
    zneskiDavkov_1.appendChild(znesekDavka_1);

    // POVZETEK DAVKOV 2
    QDomElement povzetekDavkov_2 = document.createElement("PovzetekDavkovRacuna");
    racun.appendChild(povzetekDavkov_2);
    QDomElement davkiRacuna_2 = document.createElement("DavkiRacuna");
    povzetekDavkov_2.appendChild(davkiRacuna_2);
    QDomElement vrstaDavka_2 = document.createElement("VrstaDavka");
    QDomText vrstaDavka_2_text = document.createTextNode("VAT");
    vrstaDavka_2.appendChild(vrstaDavka_2_text);
    davkiRacuna_2.appendChild(vrstaDavka_2);
    QDomElement odstotekDavka_2 = document.createElement("OdstotekDavka");
    QDomText odstotekDavka_2_text = document.createTextNode("9.5");
    odstotekDavka_2.appendChild(odstotekDavka_2_text);
    davkiRacuna_2.appendChild(odstotekDavka_2);
    QDomElement zneskiDavkov_2 = document.createElement("ZneskiDavkov");
    povzetekDavkov_2.appendChild(zneskiDavkov_2);
    QDomElement vrstaZneskaDavka_2 = document.createElement("VrstaZneskaDavka");
    QDomText vrstaZneskaDavka_2_text = document.createTextNode("125");
    vrstaZneskaDavka_2.appendChild(vrstaZneskaDavka_2_text);
    zneskiDavkov_2.appendChild(vrstaZneskaDavka_2);
    QDomElement znesekDavka_2 = document.createElement("ZnesekDavka");
    QDomText znesekDavka_2_text = document.createTextNode("0.00");
    znesekDavka_2.appendChild(znesekDavka_2_text);
    zneskiDavkov_2.appendChild(znesekDavka_2);
    QDomElement zneskiDavkov_2_1 = document.createElement("ZneskiDavkov");
    povzetekDavkov_2.appendChild(zneskiDavkov_2_1);
    QDomElement vrstaZneskaDavka_2_1 = document.createElement("VrstaZneskaDavka");
    QDomText vrstaZneskaDavka_2_1_text = document.createTextNode("124");
    vrstaZneskaDavka_2_1.appendChild(vrstaZneskaDavka_2_1_text);
    zneskiDavkov_2_1.appendChild(vrstaZneskaDavka_2_1);
    QDomElement znesekDavka_2_1 = document.createElement("ZnesekDavka");
    QDomText znesekDavka_2_1_text = document.createTextNode("0.00");
    znesekDavka_2_1.appendChild(znesekDavka_2_1_text);
    zneskiDavkov_2_1.appendChild(znesekDavka_2_1);

    // POVZETEK DAVKOV 3
    QDomElement povzetekDavkov_3 = document.createElement("PovzetekDavkovRacuna");
    racun.appendChild(povzetekDavkov_3);
    QDomElement davkiRacuna_3 = document.createElement("DavkiRacuna");
    povzetekDavkov_3.appendChild(davkiRacuna_3);
    QDomElement vrstaDavka_3 = document.createElement("VrstaDavka");
    QDomText vrstaDavka_3_text = document.createTextNode("VAT");
    vrstaDavka_3.appendChild(vrstaDavka_3_text);
    davkiRacuna_3.appendChild(vrstaDavka_3);
    QDomElement odstotekDavka_3 = document.createElement("OdstotekDavka");
    QDomText odstotekDavka_3_text = document.createTextNode("0");
    odstotekDavka_3.appendChild(odstotekDavka_3_text);
    davkiRacuna_3.appendChild(odstotekDavka_3);
    QDomElement zneskiDavkov_3 = document.createElement("ZneskiDavkov");
    povzetekDavkov_3.appendChild(zneskiDavkov_3);
    QDomElement vrstaZneskaDavka_3 = document.createElement("VrstaZneskaDavka");
    QDomText vrstaZneskaDavka_3_text = document.createTextNode("125");
    vrstaZneskaDavka_3.appendChild(vrstaZneskaDavka_3_text);
    zneskiDavkov_3.appendChild(vrstaZneskaDavka_3);
    QDomElement znesekDavka_3 = document.createElement("ZnesekDavka");
    QDomText znesekDavka_3_text = document.createTextNode("0.00");
    znesekDavka_3.appendChild(znesekDavka_3_text);
    zneskiDavkov_3.appendChild(znesekDavka_3);
    QDomElement zneskiDavkov_3_1 = document.createElement("ZneskiDavkov");
    povzetekDavkov_3.appendChild(zneskiDavkov_3_1);
    QDomElement vrstaZneskaDavka_3_1 = document.createElement("VrstaZneskaDavka");
    QDomText vrstaZneskaDavka_3_1_text = document.createTextNode("124");
    vrstaZneskaDavka_3_1.appendChild(vrstaZneskaDavka_3_1_text);
    zneskiDavkov_3_1.appendChild(vrstaZneskaDavka_3_1);
    QDomElement znesekDavka_3_1 = document.createElement("ZnesekDavka");
    QDomText znesekDavka_3_1_text = document.createTextNode("0.00");
    znesekDavka_3_1.appendChild(znesekDavka_3_1_text);
    zneskiDavkov_3_1.appendChild(znesekDavka_3_1);

    // POVZETEK ZNESKOV RACUNA 1
    QDomElement povzetekZneskov_1 = document.createElement("PovzetekZneskovRacuna");
    racun.appendChild(povzetekZneskov_1);
    QDomElement zneskiRacuna_1 = document.createElement("ZneskiRacuna");
    povzetekZneskov_1.appendChild(zneskiRacuna_1);
    QDomElement vrstaZneska_1 = document.createElement("VrstaZneska");
    QDomText vrstaZneska_1_text = document.createTextNode("79");
    vrstaZneska_1.appendChild(vrstaZneska_1_text);
    zneskiRacuna_1.appendChild(vrstaZneska_1);
    QDomElement znesekRacuna_1 = document.createElement("ZnesekRacuna");
    QDomText znesekRacuna_1_text = document.createTextNode(QString::number(m_total, 'f', 2));
    znesekRacuna_1.appendChild(znesekRacuna_1_text);
    zneskiRacuna_1.appendChild(znesekRacuna_1);
    QDomElement sklicZaPlacilo_1 = document.createElement("SklicZaPlacilo");
    povzetekZneskov_1.appendChild(sklicZaPlacilo_1);
    QDomElement sklicPlacila_1 = document.createElement("SklicPlacila");
    QDomText sklicPlacila_1_text = document.createTextNode("PQ");
    sklicPlacila_1.appendChild(sklicPlacila_1_text);
    sklicZaPlacilo_1.appendChild(sklicPlacila_1);
    QDomElement stevilkaSklica_1 = document.createElement("StevilkaSklica");
    QDomText stevilkaSklica_1_text = document.createTextNode("SI00 " + ui->lineEdit_stRacuna->text());
    stevilkaSklica_1.appendChild(stevilkaSklica_1_text);
    sklicZaPlacilo_1.appendChild(stevilkaSklica_1);

    // POVZETEK ZNESKOV RACUNA 2
    QDomElement povzetekZneskov_2 = document.createElement("PovzetekZneskovRacuna");
    racun.appendChild(povzetekZneskov_2);
    QDomElement zneskiRacuna_2 = document.createElement("ZneskiRacuna");
    povzetekZneskov_2.appendChild(zneskiRacuna_2);
    QDomElement vrstaZneska_2 = document.createElement("VrstaZneska");
    QDomText vrstaZneska_2_text = document.createTextNode("53");
    vrstaZneska_2.appendChild(vrstaZneska_2_text);
    zneskiRacuna_2.appendChild(vrstaZneska_2);
    QDomElement znesekRacuna_2 = document.createElement("ZnesekRacuna");
    QDomText znesekRacuna_2_text = document.createTextNode(QString::number(popust_total, 'f', 2));
    znesekRacuna_2.appendChild(znesekRacuna_2_text);
    zneskiRacuna_2.appendChild(znesekRacuna_2);
    QDomElement sklicZaPlacilo_2 = document.createElement("SklicZaPlacilo");
    povzetekZneskov_2.appendChild(sklicZaPlacilo_2);
    QDomElement sklicPlacila_2 = document.createElement("SklicPlacila");
    QDomText sklicPlacila_2_text = document.createTextNode("PQ");
    sklicPlacila_2.appendChild(sklicPlacila_2_text);
    sklicZaPlacilo_2.appendChild(sklicPlacila_2);
    QDomElement stevilkaSklica_2 = document.createElement("StevilkaSklica");
    QDomText stevilkaSklica_2_text = document.createTextNode("SI00 " + ui->lineEdit_stRacuna->text());
    stevilkaSklica_2.appendChild(stevilkaSklica_2_text);
    sklicZaPlacilo_2.appendChild(stevilkaSklica_2);

    // POVZETEK ZNESKOV RACUNA 3
    QDomElement povzetekZneskov_3 = document.createElement("PovzetekZneskovRacuna");
    racun.appendChild(povzetekZneskov_3);
    QDomElement zneskiRacuna_3 = document.createElement("ZneskiRacuna");
    povzetekZneskov_3.appendChild(zneskiRacuna_3);
    QDomElement vrstaZneska_3 = document.createElement("VrstaZneska");
    QDomText vrstaZneska_3_text = document.createTextNode("125");
    vrstaZneska_3.appendChild(vrstaZneska_3_text);
    zneskiRacuna_3.appendChild(vrstaZneska_3);
    QDomElement znesekRacuna_3 = document.createElement("ZnesekRacuna");
    QDomText znesekRacuna_3_text = document.createTextNode(QString::number(m_total - popust_total, 'f', 2));
    znesekRacuna_3.appendChild(znesekRacuna_3_text);
    zneskiRacuna_3.appendChild(znesekRacuna_3);
    QDomElement sklicZaPlacilo_3 = document.createElement("SklicZaPlacilo");
    povzetekZneskov_3.appendChild(sklicZaPlacilo_3);
    QDomElement sklicPlacila_3 = document.createElement("SklicPlacila");
    QDomText sklicPlacila_3_text = document.createTextNode("PQ");
    sklicPlacila_3.appendChild(sklicPlacila_3_text);
    sklicZaPlacilo_3.appendChild(sklicPlacila_3);
    QDomElement stevilkaSklica_3 = document.createElement("StevilkaSklica");
    QDomText stevilkaSklica_3_text = document.createTextNode("SI00 " + ui->lineEdit_stRacuna->text());
    stevilkaSklica_3.appendChild(stevilkaSklica_3_text);
    sklicZaPlacilo_3.appendChild(stevilkaSklica_3);

    // POVZETEK ZNESKOV RACUNA 4
    QDomElement povzetekZneskov_4 = document.createElement("PovzetekZneskovRacuna");
    racun.appendChild(povzetekZneskov_4);
    QDomElement zneskiRacuna_4 = document.createElement("ZneskiRacuna");
    povzetekZneskov_4.appendChild(zneskiRacuna_4);
    QDomElement vrstaZneska_4 = document.createElement("VrstaZneska");
    QDomText vrstaZneska_4_text = document.createTextNode("176");
    vrstaZneska_4.appendChild(vrstaZneska_4_text);
    zneskiRacuna_4.appendChild(vrstaZneska_4);
    QDomElement znesekRacuna_4 = document.createElement("ZnesekRacuna");
    QDomText znesekRacuna_4_text = document.createTextNode(QString::number((m_total - popust_total) * 0.22, 'f', 2));
    znesekRacuna_4.appendChild(znesekRacuna_4_text);
    zneskiRacuna_4.appendChild(znesekRacuna_4);
    QDomElement sklicZaPlacilo_4 = document.createElement("SklicZaPlacilo");
    povzetekZneskov_4.appendChild(sklicZaPlacilo_4);
    QDomElement sklicPlacila_4 = document.createElement("SklicPlacila");
    QDomText sklicPlacila_4_text = document.createTextNode("PQ");
    sklicPlacila_4.appendChild(sklicPlacila_4_text);
    sklicZaPlacilo_4.appendChild(sklicPlacila_4);
    QDomElement stevilkaSklica_4 = document.createElement("StevilkaSklica");
    QDomText stevilkaSklica_4_text = document.createTextNode("SI00 " + ui->lineEdit_stRacuna->text());
    stevilkaSklica_4.appendChild(stevilkaSklica_4_text);
    sklicZaPlacilo_4.appendChild(stevilkaSklica_4);

    // POVZETEK ZNESKOV RACUNA 5
    QDomElement povzetekZneskov_5 = document.createElement("PovzetekZneskovRacuna");
    racun.appendChild(povzetekZneskov_5);
    QDomElement zneskiRacuna_5 = document.createElement("ZneskiRacuna");
    povzetekZneskov_5.appendChild(zneskiRacuna_5);
    QDomElement vrstaZneska_5 = document.createElement("VrstaZneska");
    QDomText vrstaZneska_5_text = document.createTextNode("86");
    vrstaZneska_5.appendChild(vrstaZneska_5_text);
    zneskiRacuna_5.appendChild(vrstaZneska_5);
    QDomElement znesekRacuna_5 = document.createElement("ZnesekRacuna");
    QDomText znesekRacuna_5_text = document.createTextNode(ui->label_skupaj->text().remove("€"));
    znesekRacuna_5.appendChild(znesekRacuna_5_text);
    zneskiRacuna_5.appendChild(znesekRacuna_5);
    QDomElement sklicZaPlacilo_5 = document.createElement("SklicZaPlacilo");
    povzetekZneskov_5.appendChild(sklicZaPlacilo_5);
    QDomElement sklicPlacila_5 = document.createElement("SklicPlacila");
    QDomText sklicPlacila_5_text = document.createTextNode("PQ");
    sklicPlacila_5.appendChild(sklicPlacila_5_text);
    sklicZaPlacilo_5.appendChild(sklicPlacila_5);
    QDomElement stevilkaSklica_5 = document.createElement("StevilkaSklica");
    QDomText stevilkaSklica_5_text = document.createTextNode("SI00 " + ui->lineEdit_stRacuna->text());
    stevilkaSklica_5.appendChild(stevilkaSklica_5_text);
    sklicZaPlacilo_5.appendChild(stevilkaSklica_5);

    // POVZETEK ZNESKOV RACUNA 6
    QDomElement povzetekZneskov_6 = document.createElement("PovzetekZneskovRacuna");
    racun.appendChild(povzetekZneskov_6);
    QDomElement zneskiRacuna_6 = document.createElement("ZneskiRacuna");
    povzetekZneskov_6.appendChild(zneskiRacuna_6);
    QDomElement vrstaZneska_6 = document.createElement("VrstaZneska");
    QDomText vrstaZneska_6_text = document.createTextNode("9");
    vrstaZneska_6.appendChild(vrstaZneska_6_text);
    zneskiRacuna_6.appendChild(vrstaZneska_6);
    QDomElement znesekRacuna_6 = document.createElement("ZnesekRacuna");
    QDomText znesekRacuna_6_text = document.createTextNode(ui->label_skupaj->text().remove("€"));
    znesekRacuna_6.appendChild(znesekRacuna_6_text);
    zneskiRacuna_6.appendChild(znesekRacuna_6);
    QDomElement sklicZaPlacilo_6 = document.createElement("SklicZaPlacilo");
    povzetekZneskov_6.appendChild(sklicZaPlacilo_6);
    QDomElement sklicPlacila_6 = document.createElement("SklicPlacila");
    QDomText sklicPlacila_6_text = document.createTextNode("PQ");
    sklicPlacila_6.appendChild(sklicPlacila_6_text);
    sklicZaPlacilo_6.appendChild(sklicPlacila_6);
    QDomElement stevilkaSklica_6 = document.createElement("StevilkaSklica");
    QDomText stevilkaSklica_6_text = document.createTextNode("SI00 " + ui->lineEdit_stRacuna->text());
    stevilkaSklica_6.appendChild(stevilkaSklica_6_text);
    sklicZaPlacilo_6.appendChild(stevilkaSklica_6);

    QString output = m_fileShrani + "/RACUN_IN_DOBAVNICA_" + ui->comboBox_narocnik->currentText() + "_" + ui->lineEdit_stRacuna->text() + ".xml";
    QFile file(output);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       qDebug() << "Error";
       return;
    }
    else
    {
        QTextStream out(&file);
        const int Indent = 4;
        QDomNode xmlNode = document.createProcessingInstruction("xml",
                                     "version=\"1.0\" encoding=\"utf-8\"");
        QDomNode xmlStyle = document.createProcessingInstruction("xml-stylesheet", "type=\"text/xsl\" href=\"http://www.bizbox.eu/XSL/2015/06/eRacun.xslt\"");
        document.insertBefore(xmlNode, document.firstChild());
        document.insertAfter(xmlStyle, document.firstChild());
        document.save(out, Indent);
        file.close();
    }
}

void NovRacun::PopraviRacun(QString stranka, QString stevilka_racuna, QString vezni_dokument, QString datum, QString opomba, QString produkti, QString osnova, QString ddv, QString skupaj, QString popust)
{
    qDebug() << skupaj << ddv;
    ui->comboBox_narocnik->setCurrentText(stranka);
    ui->lineEdit_stRacuna->setText(stevilka_racuna);
    ui->lineEdit_sklic->setText(vezni_dokument);
    ui->dateEdit->setDate(QDate::fromString(datum));
    if(opomba == "Rac_napisal")
        opomba = "";
    ui->lineEdit->setText(opomba);
    popust.remove("(end)");
    ui->lineEdit_popust->setText(popust);
    QStringList tmp_list;
    QString tmp_string;
    QString tmp_string1;
    QString tmp_string2;
    QString tmp_string3;
    QVector<QTreeWidgetItem*> itms;
    double m_osnova(0);
    if(produkti == " ") {
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_dodani);
        itm->setText(0, "");
        itm->setText(1, "");
        itm->setText(2, "");
        itm->setText(3, "");
        itms.append(itm);
        ui->treeWidget_dodani->addTopLevelItem(itms.at(0));
        m_itemsAdded = 1;
        delete ui->treeWidget_dodani->takeTopLevelItem(ui->treeWidget_dodani->indexOfTopLevelItem(itm));
    } else {
        QStringList produkti_list = produkti.split(" | ", QString::SkipEmptyParts);
        for(int i(0); i < produkti_list.length(); i++)
        {
            QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_dodani);
            tmp_string = produkti_list.at(i);
            tmp_list = tmp_string.split(" - ", QString::SkipEmptyParts);
            tmp_string = tmp_list.at(0);
            tmp_string.remove("(");
            tmp_string.remove(")");
            tmp_string1 = tmp_list.at(1);
            tmp_string2 = tmp_list.at(2);
            tmp_string2.remove("(");
            tmp_string2.remove(")");
            tmp_string3 = tmp_list.at(3);
            m_osnova += (tmp_string2.toDouble() * tmp_string3.toInt());
            tmp_string2.prepend("€");
            itm->setText(0, tmp_string);
            itm->setText(1, tmp_string1);
            itm->setText(2, tmp_string2);
            itm->setText(3, tmp_string3);
            itms.append(itm);
            ui->treeWidget_dodani->addTopLevelItem(itms.at(i));
            tmp_list.clear();
            m_itemsAdded++;
        }
    }
    ui->label_osnova->setText("€" + QString::number(m_osnova, 'f', 2));
    m_total = osnova.toDouble();
    double m_pop = ui->lineEdit_popust->text().toDouble();
    double m_popust(0);
    double m_skupaj(0);
    double m_ddv;
    m_popust = m_total - (m_total * (m_pop / 100));
    if(m_pop == 0.0)
        ui->label_popust->setText("€" + QString::number(m_total, 'f', 2));
    else
        ui->label_popust->setText("€" + QString::number(m_popust, 'f', 2));

    m_ddv = m_popust * 0.22;
    if(m_ddv < 0)
        ui->label_ddv->setText("€0.0");
    else
        ui->label_ddv->setText("€" + QString::number(m_ddv, 'f', 2));

    m_skupaj = m_popust + m_ddv;
    if(m_skupaj < 0)
        ui->label_skupaj->setText("€0.0");
    else
        ui->label_skupaj->setText("€" + QString::number(m_skupaj, 'f', 2));
    m_sprememba = true;
}

// Preberi produkt iz file-a
void NovRacun::Read()
{
    // zbrise vse iz treeWidgeta
    ui->treeWidget_seznam->clear();
    int izbPod = ui->comboBox_narocnik->currentIndex() + 1;
    QString podjetje = QString::number(izbPod) + ".txt";

    QFile fileName(podjetje);
    // test ce je odprt za branje
    if(!fileName.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() READ";
        return;
    }
    // stream za branje fila
    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    // preberi vsako vrstico ce je  izpusti drugace dodaj vrstico v listWidget
    QString mText("");
    // locilo med podatki podjetja v filu ime_podjetja;naslov;ddv;email itd itd
    QRegExp rx("[;]");
    // naredimo listo da posamezne podatke locimo in shranimo kot posamezno kategorijo
    QStringList list;

    // preberemo celoten dokument in shranimo posamezne podatke v list
    while(!in.atEnd())
    {
        // prebere podjetje
        mText = in.readLine();
        // v listo shranimo posamezne podatke podjetja iz fila
        list = mText.split(rx, QString::SkipEmptyParts);
        if(mText == "")
        {
            continue;
        }
        else if(list.at(0) == "Dodano: ")
        {
            continue;
        }
        else
        {
            // v treeWidget vnesemo vsa podjetja (posamezni podatki iz liste so list.at(?))
            AddRoot(list.at(0), list.at(1), list.at(2));
        }
    }
    // zapremo file
    fileName.close();
}

// vstavi podjetja v combo box
void NovRacun::AddItemsToCombo()
{
    // zbrise vse iz combo boxa
    ui->comboBox_narocnik->clear();
    // naredi QFile var za fileName
    QFile mFile(m_fileName);
    // test ce je odprt za branje
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() addItemsToCombo";
        return;
    }
    // stream za branje fila
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    // preberi vsako vrstico ce je  izpusti drugace dodaj vrstico v combo box
    QString mText("");
    // locilo med podatki podjetja v filu ime_podjetja;naslov;ddv;email itd itd
    QRegExp rx("[;]");
    // naredimo listo da posamezne podatke locimo in shranimo kot posamezno kategorijo
    QStringList list;
    bool first(true);
    // preberemo celoten dokument in shranimo posamezne podatke v list
    while(!in.atEnd())
    {
        // prebere podjetje
        mText = in.readLine();
        // v listo shranimo posamezne podatke podjetja iz fila
        list = mText.split(rx, QString::SkipEmptyParts);
        if(first)
        {
            m_cNaziv = list.at(2);
            m_naslov = list.at(3);
            m_posta = list.at(4);
            m_ddv = list.at(5);
            m_email = list.at(6);
            first = false;
        }
        if(mText == "")
        {
            continue;
        }
        else
        {
            // doda podjetje v combo box
            ui->comboBox_narocnik->addItem(list.at(1));
        }
    }
    // zapremo file
    mFile.close();
    Read();
}

// dodat root v treeWidget
void NovRacun::AddRoot(QString id, QString naziv, QString cena)
{
    // pointer na treeWidget
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_seznam);
    // nastavimo stevilko podjetja in ime podjetja v colom 0 in 1
    itm->setText(0, id);
    itm->setTextAlignment(0, Qt::AlignLeading);
    itm->setText(1, naziv);
    itm->setTextAlignment(1, Qt::AlignLeading);
    itm->setText(2, "€" + cena);
    itm->setTextAlignment(2, Qt::AlignLeading);
    // dodamo podjetje v treeWidget
    ui->treeWidget_seznam->addTopLevelItem(itm);
    // nastavimo barvo za vsako drugo podjetje
    QColor color(210,210,210);
    QColor wcolor(250,250,250);
    // m_count je member bool ce je tru je barva siva drugace bela
    if(m_count)
    {
        itm->setBackgroundColor(0,color);
        itm->setBackgroundColor(1,color);
        itm->setBackgroundColor(2,color);
        m_count = false;
    } else {
        itm->setBackgroundColor(0,wcolor);
        itm->setBackgroundColor(1,wcolor);
        itm->setBackgroundColor(2,wcolor);
        m_count = true;
    }
}

void NovRacun::on_comboBox_narocnik_currentIndexChanged()
{
    m_itemsAdded = 0;
    m_cNaziv = "";
    m_naslov = "";
    m_posta = "";
    m_ddv = "";
    m_email = "";
    QFile mFile(m_fileName);
    // test ce je odprt za branje
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read() addItemsToCombo";
        return;
    }
    // stream za branje fila
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    // preberi vsako vrstico ce je  izpusti drugace dodaj vrstico v combo box
    QString mText("");
    // locilo med podatki podjetja v filu ime_podjetja;naslov;ddv;email itd itd
    QRegExp rx("[;]");
    // naredimo listo da posamezne podatke locimo in shranimo kot posamezno kategorijo
    QStringList list;
    // preberemo celoten dokument in shranimo posamezne podatke v list
    while(!in.atEnd())
    {
        // prebere podjetje
        mText = in.readLine();
        // v listo shranimo posamezne podatke podjetja iz fila
        list = mText.split(rx, QString::SkipEmptyParts);
        if(mText == "")
        {
            continue;
        }
        else if(list.at(1) == ui->comboBox_narocnik->currentText())
        {
            m_cNaziv = list.at(2);
            m_naslov = list.at(3);
            m_posta = list.at(4);
            m_ddv = list.at(5);
            m_email = list.at(6);
            break;
        }
    }
    // zapremo file
    mFile.close();
    Read();
    ui->treeWidget_dodani->clear();
    ui->label_skupaj->setText("€0.0");
    ui->label_ddv->setText("€0.0");
    ui->label_osnova->setText("€0.0");
    ui->lineEdit_popust->setText("0");
    m_total = 0;
    m_itemsAdded = 0;
}

QString NovRacun::CenaDDV(QString price, QString items)
{
    QRegExp eur("[€]");
    QRegExp dot("[,]");
    QStringList price_list;
    price_list = price.split(eur, QString::SkipEmptyParts);
    price = price_list.at(0);
    price_list = price.split(dot, QString::SkipEmptyParts);
    if(price_list.size() == 2)
    {
        QRegularExpression place_dot(price);
        QString rep_produkt(price_list.at(0) + "." + price_list.at(1));
        price.replace(place_dot, rep_produkt);
    }
    double dPrice = price.toDouble();
    int intItems = items.toInt();
    double sumPrice = dPrice * intItems;
    return QString::number(sumPrice, 'f', 2);
}

void NovRacun::on_treeWidget_seznam_doubleClicked()
{
    if(m_itemsAdded > m_max_produktov) {
        QMessageBox error_stringToLong;
        error_stringToLong.critical(this, "Napaka", "Racun vsebuje prevec produktov ali pa je ime produkta predolgo!");
        error_stringToLong.show();
        return;
    }
    NumOfItems numItems;
    numItems.setModal(true);
    numItems.exec();
    m_numItems = numItems.m_numKosov;
    bool insertCorrect(true);
    if(m_numItems == "0")
        insertCorrect = false;
    while(insertCorrect) {
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_dodani);
        QString cenaDDV = CenaDDV(ui->treeWidget_seznam->currentItem()->text(2), m_numItems);
        itm->setText(0, ui->treeWidget_seznam->currentItem()->text(0));
        itm->setTextAlignment(0, Qt::AlignLeading);
        itm->setText(1, ui->treeWidget_seznam->currentItem()->text(1));
        itm->setTextAlignment(1, Qt::AlignLeading);
        itm->setText(2, ui->treeWidget_seznam->currentItem()->text(2));
        itm->setTextAlignment(2, Qt::AlignLeading);
        itm->setText(3, m_numItems);
        itm->setTextAlignment(3, Qt::AlignHCenter);
        itm->setText(4, cenaDDV);
        ui->treeWidget_dodani->addTopLevelItem(itm);
        QColor color(210,210,210);
        QColor wcolor(250,250,250);
        // m_count je member bool ce je true je barva siva drugace bela
        if(m_count)
        {
            itm->setBackgroundColor(0,color);
            itm->setBackgroundColor(1,color);
            itm->setBackgroundColor(2,color);
            itm->setBackgroundColor(3,color);
            m_count = false;
        } else {
            itm->setBackgroundColor(0,wcolor);
            itm->setBackgroundColor(1,wcolor);
            itm->setBackgroundColor(2,wcolor);
            itm->setBackgroundColor(3,wcolor);
            m_count = true;
        }
        QString price = ui->treeWidget_seznam->currentItem()->text(2);
        CalcSkupaj(price, m_numItems, true);
        ui->treeWidget_dodani->setUniformRowHeights(true);
        m_itemsAdded++;
        insertCorrect = false;
    }
}

// sesteva v label skupaj
void NovRacun::CalcSkupaj(QString &price, QString &numOfItems, bool plus)
{
    double popust(0.0);
    double skupaj(0.0);
    double ddv(0.0);
    QRegExp eur("[€]");
    QRegExp dot("[,]");
    QStringList price_list;
    price_list = price.split(eur, QString::SkipEmptyParts);
    price = price_list.at(0);
    price_list = price.split(dot, QString::SkipEmptyParts);
    if(price_list.size() == 2)
    {
        QRegularExpression place_dot(price);
        QString rep_produkt(price_list.at(0) + "." + price_list.at(1));
        price.replace(place_dot, rep_produkt);
    }

    if(plus) {
        m_total = m_total + (numOfItems.toInt() * price.toDouble());
    }
    else {
        m_total = m_total - (numOfItems.toInt() * price.toDouble());
    }

    if(m_total < 0)
        ui->label_osnova->setText("€0");
    else
        ui->label_osnova->setText("€" + QString::number(m_total, 'f', 2));

    double pop = ui->lineEdit_popust->text().toDouble();
    popust = m_total - (m_total * (pop / 100));
    if(pop == 0.0)
        ui->label_popust->setText("€" + QString::number(m_total, 'f', 2));
    else
        ui->label_popust->setText("€" + QString::number(popust, 'f', 2));

    ddv = popust * 0.22;
    if(ddv < 0)
        ui->label_ddv->setText("€0");
    else
        ui->label_ddv->setText("€" + QString::number(ddv, 'f', 2));

    skupaj = popust + ddv;
    if(skupaj < 0)
        ui->label_skupaj->setText("€0");
    else
        ui->label_skupaj->setText("€" + QString::number(skupaj, 'f', 2));
}

void NovRacun::on_treeWidget_dodani_doubleClicked()
{
    if(m_itemsAdded < 0)
    {
        QMessageBox error_stringToLong;
        error_stringToLong.critical(this, "Napaka", "V seznamu ni produktov!");
        error_stringToLong.show();
        return;
    }

    QString price = ui->treeWidget_dodani->currentItem()->text(2);
    QString numItems = ui->treeWidget_dodani->currentItem()->text(3);
    CalcSkupaj(price, numItems, false);
    delete ui->treeWidget_dodani->takeTopLevelItem(ui->treeWidget_dodani->currentIndex().row());
    m_itemsAdded--;
}

void NovRacun::Search(QString searchName)
{
    int izbPod = ui->comboBox_narocnik->currentIndex() + 1;
    QString podjetje = QString::number(izbPod) + ".txt";
    // odpre file za branje
    QFile mFile(podjetje);
    // test ce je file odprt
    if(!mFile.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug() << "Error opening mFile for reading in popravi button";
        return;
    }
    // stream za shranit text fila
    QTextStream out(&mFile);
    out.setCodec("UTF-8");
    // prebere celoten dokument in shrani v var
    QString line;
    while(!out.atEnd()){
        line = out.readLine();
        if(line.contains(searchName, Qt::CaseInsensitive)) {
            // locilo med podatki podjetja v filu ime_podjetja;naslov;ddv;email itd itd
            QRegExp rx("[;]");
            // naredimo listo da posamezne podatke locimo in shranimo kot posamezno kategorijo
            QStringList list;
            list = line.split(rx, QString::SkipEmptyParts);
                if(list.at(0) == "Dodano: ")
                    break;
                else
                {
                    // v treeWidget vnesemo vsa podjetja (posamezni podatki iz liste so list.at(?))
                    AddRoot(list.at(0), list.at(1), list.at(2));
                }
        }
    }
    // zapre file
    mFile.close();
}

int NovRacun::creatPDF()
{
    // Arhivski del
    QDate inputDate = ui->dateEdit->date();
    QDateTime date = QDateTime::currentDateTime();
    QString items(" ");
    QString product;
    for(int i(0); i < m_itemsAdded; i++)
    {
        items.append("(");
        product = ui->treeWidget_dodani->topLevelItem(i)->text(0);
        items.append(product);
        items.append(") - ");
        product = ui->treeWidget_dodani->topLevelItem(i)->text(1);
        items.append(product);
        items.append(" - (");
        product = ui->treeWidget_dodani->topLevelItem(i)->text(2);
        items.append(product);
        items.append(") - ");
        product = ui->treeWidget_dodani->topLevelItem(i)->text(3);
        items.append(product);
        items.append(" | ");
    }
    items.remove("€");
    QFile mFile(m_arhivLogin);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in creatPDF login";
        return 1;
    }
    QTextStream in(&mFile);
    in.setCodec("UTF-8");
    QString mText("");
    while(!in.atEnd())
    {
        mText = in.readLine();
    }
    QStringList mTextList = mText.split(" ; ");
    mText = mTextList.at(1);
    mFile.close();
    QString sum_osnova = ui->label_osnova->text();
    QString sum_ddv = ui->label_ddv->text();
    QString sum_skupaj = ui->label_skupaj->text();
    sum_osnova.remove("€");
    sum_ddv.remove("€");
    sum_skupaj.remove("€");
    QString st_racuna(" ");
    QString st_sklic(" ");
    QString text_opomba(" ");
    QString nov_sprememba(" ");
    if(ui->lineEdit_stRacuna->text() != "")
        st_racuna = ui->lineEdit_stRacuna->text();
    if(ui->lineEdit_sklic->text() != "")
        st_sklic = ui->lineEdit_sklic->text();
    if(ui->lineEdit->text() != "")
        text_opomba = ui->lineEdit->text();
    if(m_sprememba)
        nov_sprememba = "Sprememba racuna ; ";
    else
        nov_sprememba = "Nov racun ; ";
    QString arhiv = nov_sprememba + date.toString("dd.MM.yyyy ; hh:mm:ss.zzz") + " ; "
            + "St_rac: " + st_racuna + " ; "
            + "Vezni_dok: " + st_sklic + " ; "
            + "Dat_izdaje: " + ui->dateEdit->text() + " ; "
            + "Dat_valute: " + inputDate.addDays(45).toString("d. M. yyyy") + " ; "
            + "Narocnik: " + ui->comboBox_narocnik->currentText() + " ; "
            + "Produkti: " + items + " ; "
            + "Sum_osnova: " + sum_osnova + " ; "
            + "Sum_ddv: " + sum_ddv + " ; "
            + "Sum_skupaj: " + sum_skupaj + " ; "
            + "Opomba: " + text_opomba + " ; "
            + "Rac_napisal: " + mText + " ; "
            + "Placilo: " + " " + " ; " + "Dat_placila: " + " " + " ; " + "Opomba: " + " " + " ; Popust: " + ui->lineEdit_popust->text() + " (end)" ;
    Arhiv(m_arhivNovRacun, arhiv);
    arhiv = ui->lineEdit_stRacuna->text();
    QStringList arhivLine = arhiv.split("-", QString::SkipEmptyParts);
    arhiv = arhivLine.at(0);
    Arhiv(m_arhivStRacuna, arhiv);

    // PDF del
    QString footer = "<table align=center width=100% height=100%>"
                     "<div align=center>"
                        "<hr width=100% size=5>""<font color=#0000aa size=5>""|""</font>""<font color=#aa0000 size=5>"" ELRA SETI d.o.o., proizvodnja setov iz kablov in vodnikov ""</font>""<font color=#0000aa size=5>""|""</font>""<font color=#aa0000 size=5>"" Andraž nad Polzelo 74a ""</font>""<font color=#0000aa size=5>""|""</font>""<font color=#aa0000 size=5>"" 3313 Polzela ""</font>""<font color=#0000aa size=5>""|""<br>"
                        "<font color=#aa0000 size=5>""|""</font>""<font color=#0000aa size=5>"" T+386(0)3 897 44 46 ""</font>""<font color=#aa0000 size=5>""|""</font>""<font color=#0000aa size=5>"" GSM +386 (0)41 326 103 ""</font>""<font color=#aa0000 size=5>""|""</font>""<font color=#0000aa size=5>"" F+386(0)3 897 44 47 ""</font>""<font color=#aa0000 size=5>""|""</font>""<font color=#0000aa size=5>"" info@elraseti.si ""</font>""<font color=#aa0000 size=5>""|""</font>""<br>"
                        "<font color=#000000 size=5>"
                        "| ID DDV (VAT nr.): SI83452010 | Matična št.: 6514812000 | Reg. št.: Srg 2013/53219 |<br>"
                        "| TRR (BANK): SBERBANK: SI56 3000 0001 1326 144 | osnovni kapital €7.500,00 |"
                        "</font>"
                     "</div>"
                     "</table>";

    QString header = "<font color=#ffffff size=0 align=center>"
                        "prazno""<br>"
                     "<font color=#000000>"
                     "<div align=center>"
                        "<font color='#aa0000'>"
                            "<h1 width=100%>"
                                "ELRA SETI D.O.O.<hr width=100%>"
                            "</h1>"
                        "</font>"
                     "</div>";

    QString tabela_za_dodane_produkte = "<table align=left width=100% height=100%>"
                                        "<tr>"
                                            "<th>"
                                                "<div align=center>""<font size=4>"
                                                    "<hr width=100% size=1>Št. "
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>""<font size=4>"
                                                    "<hr width=100% size=1> Id "
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>""<font size=4>"
                                                    "<hr width=100% size=1> Opis "
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>""<font size=4>"
                                                    "<hr width=100% size=1> Kol. "
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>""<font size=4>"
                                                    "<hr width=100% size=1> Cena "
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>""<font size=4>"
                                                    "<hr width=100% size=1> DDV "
                                                "</div>"
                                            "</th>"
                                            "<th>"
                                                "<div align=center>""<font size=4>"
                                                    "<hr width=100% size=1> Vrednost "
                                                "</div>"
                                            "</th>"
                                        "</tr>";

    QString opombe_izdelal = "<table align=left width=100% height=100%>"
                             "<div align=left>"
                                 "<tr>"
                                     "<th>"
                                         "<div align=left>"
                                             "Opombe: " + ui->lineEdit->text() + "<br>"
                                         "</div>"
                                     "</th>"
                                     "<th rowspan=5>"
                                         "<div align=right>"
                                            "<img src=\"qrc:/icons/zig.png\"/>"
                                            "<img src=\"qrc:/icons/podpis.png\"/ height=16 width=16>"
                                         "</div>"
                                     "</th>"
                                     "<th>"
                                     "</th>"
                                 "</tr>"
                                 "<tr>"
                                     "<th>"
                                         "<div align=left>"
                                            "Dokument sestavil:"
                                         "</div>"
                                     "</th>"
                                     "<th>"
                                         "<div align=center>"
                                             ""
                                         "</div>"
                                     "</th>"
                                     "<th>"
                                         "<div align=right>"
                                             "Srečko Rajh"
                                         "</div>"
                                     "</th>"

                                 "</tr>"
                                 "<tr>"
                                     "<th>"
                                         "<div align=left>"
                                             "Nejc Sedovnik"
                                         "</div>"
                                     "</th>"
                                     "<th>"
                                         "<div align=center>"
                                             ""
                                         "</div>"
                                     "</th>"
                                     "<th>"
                                         "<div align=right>"
                                             "prokurist"
                                         "</div>"
                                     "</th>"
                                 "</tr>"
                             "</div>"
                         "</table>";

    QString produkti("");

    for(int i(0); i < m_itemsAdded; i++)
    {
        if(i == 0)
            produkti.append("<tr>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""</tr>");

        produkti.append("<tr>");
        produkti.append("<th>");
        produkti.append("<div align=center>""<font size=4>");
        produkti.append(QString::number(i+1));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>""<font size=4>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(0));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>""<font size=4>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(1));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>""<font size=4>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(3));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>""<font size=4>");
        produkti.append(ui->treeWidget_dodani->topLevelItem(i)->text(2));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>""<font size=4>");
        produkti.append("22%");
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("<th>");
        produkti.append("<div align=center>""<font size=4>");
        produkti.append("€" + ui->treeWidget_dodani->topLevelItem(i)->text(4));
        produkti.append("</div>");
        produkti.append("</th>");
        produkti.append("</tr>");
    }

    QString produkti_brezCen("");
    for(int i(0); i < m_itemsAdded; i++)
    {
        if(i == 0)
            produkti_brezCen.append("<tr>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""<th>""<hr width=100% size=2>""""</th>""</tr>");

        produkti_brezCen.append("<tr>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>""<font size=4>");
        produkti_brezCen.append(QString::number(i+1));
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>""<font size=4>");
        produkti_brezCen.append(ui->treeWidget_dodani->topLevelItem(i)->text(0));
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>""<font size=4>");
        produkti_brezCen.append(ui->treeWidget_dodani->topLevelItem(i)->text(1));
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>""<font size=4>");
        produkti_brezCen.append(ui->treeWidget_dodani->topLevelItem(i)->text(3));
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>""<font size=4>");
        produkti_brezCen.append("");
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>""<font size=4>");
        produkti_brezCen.append("");
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("<th>");
        produkti_brezCen.append("<div align=center>""<font size=4>");
        produkti_brezCen.append("");
        produkti_brezCen.append("</div>");
        produkti_brezCen.append("</th>");
        produkti_brezCen.append("</tr>");
    }

    if(m_itemsAdded < m_max_produktov)
    {
        for(int i(0); i <= (m_max_produktov - m_itemsAdded); i++)
        {
            produkti.append("<tr>");
            produkti.append("<th>");
            produkti.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti.append("_");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti.append("_");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti.append("_");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti.append("_");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti.append("_");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti.append("_");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("<th>");
            produkti.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti.append("_");
            produkti.append("</div>");
            produkti.append("</th>");
            produkti.append("</tr>");
            produkti_brezCen.append("<tr>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti_brezCen.append("_");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti_brezCen.append("_");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti_brezCen.append("_");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti_brezCen.append("_");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti_brezCen.append("_");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti_brezCen.append("_");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("<th>");
            produkti_brezCen.append("<div align=center>""<font size=4 color=#ffffff>");
            produkti_brezCen.append("_");
            produkti_brezCen.append("</div>");
            produkti_brezCen.append("</th>");
            produkti_brezCen.append("</tr>");
        }
    }

    QString sklic = ui->lineEdit_stRacuna->text();
    QStringList sklicList = sklic.split("-", QString::SkipEmptyParts);
    QString text_racun =
            /**************************** RACUN *********************************/
            "<font color='#00000000' size=4>""_" +
            header +
            "<table align=right width=100% height=100%>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "<font color='#ffffff'>"
                                "Izdelal: Nejc Sedovnik"
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<font size=6>"
                            "<div align=left>"
                                "<b>""Račun št.:""</b>"
                            "</div>"
                        "</font>"
                    "</th>"
                    "<th>"
                        "<font size=6>"
                            "<div align=left>"
                                "<b>" + ui->lineEdit_stRacuna->text() + "</b>"
                            "</div>"
                        "</font>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Kraj izdaje:"
                        "</div>"
                    "</th>"
                  "<th>"
                      "<div align=left>""<font size=4>"
                          "Andraž nad Polzelo"
                      "</div>"
                  "</th>"
                "</tr>"
                "<tr>"
                    "<th rowspan=2>"
                        "<div align=left>"
                            "<font size=6>"
                                "<b>""<u>""Naročnik:""</u>""</b>"
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Vezni dokument:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + ui->lineEdit_sklic->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Številka dobavnice:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + ui->lineEdit_stRacuna->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th rowspan=2>"
                        "<div align=left>""<font size=5>"
                            + ui->comboBox_narocnik->currentText() +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Datum opr. posla:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + ui->dateEdit->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Datum izd. računa:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + ui->dateEdit->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + m_naslov +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Datum valute:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + inputDate.addDays(45).toString("d. M. yyyy") +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + m_posta +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "TRR:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "SI56 3000 0001 1326 144"
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            "<font size=1>"
                                + m_email +
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Naša ID št. za DDV:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "SI83452010"
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Telefon:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "041 326 103"
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "ID za DDV: " + m_ddv +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Sklic:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "00 " + sklicList.at(0) + " " + sklicList.at(1) +
                        "</div>"
                    "</th>"
                "</tr>"
            "</table>"
            + tabela_za_dodane_produkte
            + produkti
            +
            "</table>"

            "<table align=left width=100% height=100%>"
                "<div align=left>"
                    "<tr>"
                        "<th>"
                            "<div align=left>""<font size=4>"
                            "<hr width=100% size=2>OSNOVA €"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<hr width=100% size=2>"
                        "</th>"
                        "<th>""<font size=4>"
                            "<hr width=100% size=2>" + ui->label_popust->text() +
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "Popust %"
                            "</div>"
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            + ui->lineEdit_popust->text() + "%"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "DDV €"
                            "</div>"
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            ""
                        "</th>"
                        "<th>"
                            + ui->label_ddv->text() +
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "<font size=6>"
                                "<hr width=100% size=2>ZA PLAČILO €<hr width=100% size=2>"
                            "</font>"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<font size=6>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=6>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=6>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=6>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=6>"
                            "<hr width=100% size=2>""<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=6>"
                                "<hr width=100% size=2>" + ui->label_skupaj->text() + "<hr width=100% size=2>"
                            "</font>"
                        "</th>"
                    "</tr>"
            "</table>"
            + opombe_izdelal
            + footer;

    QString text_dobavnica =
            /**************************** DOBAVNICA *********************************/
            "<font color='#00000000' size=0>""_" +
            header +
            "<table align=left width=100% height=100%>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            "<font color='#ffffff'>"
                                "Izdelal: Nejc Sedovnik"
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<font size=6>"
                            "<div align=left>"
                                "<b>""Dobavnica št.:""</b>"
                            "</div>"
                        "</font>"
                    "</th>"
                    "<th>"
                        "<font size=6>"
                            "<div align=left>"
                                "<b>" + ui->lineEdit_stRacuna->text() + "</b>"
                            "</div>"
                        "</font>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Kraj izdaje:"
                        "</div>"
                    "</th>"
                  "<th>"
                      "<div align=left>""<font size=4>"
                          "Andraž nad Polzelo"
                      "</div>"
                  "</th>"
                "</tr>"
                "<tr>"
                    "<th rowspan=2>"
                        "<div align=left>"
                            "<font size=6>"
                                "<b>""<u>""Naročnik:""</u>""</b>"
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Vezni dokument:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + ui->lineEdit_sklic->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Datum opr. posla:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + ui->dateEdit->text() +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th rowspan=2>"
                        "<div align=left>""<font size=5>"
                            + ui->comboBox_narocnik->currentText() +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Sklic:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "00 " + sklicList.at(0) + " " + sklicList.at(1) +
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "Telefon:"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            "041 326 103"
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + m_naslov +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>""<font size=4>"
                            + m_posta +
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            "<font size=1>"
                                + m_email +
                            "</font>"
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                "</tr>"
                "<tr>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                    "<th>"
                        "<div align=left>"
                            ""
                        "</div>"
                    "</th>"
                "</tr>"
            "</table>"

            + tabela_za_dodane_produkte
            + produkti_brezCen
            +
            "</table>"
            "<table align=left width=100% height=100%>"
                "<div align=left>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<div align=left>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                            "</div>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>"
                        "</th>"
                    "</tr>"
                    "<tr>"
                    "</tr>"
                    "<tr>"
                    "</tr>"
                    "<tr>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                        "<th>"
                            "<font size=5>"
                            "<font color=#ffffff>""prazno""</font>""<hr width=100% size=2>"
                        "</th>"
                    "</tr>"
            "</table>"
            + opombe_izdelal
            + footer;

    QTextDocument document_racun;
    QTextCursor cursor(&document_racun);
    cursor.insertHtml(text_racun);
    QTextBlockFormat blockFormat;
    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
    cursor.insertBlock(blockFormat);
    cursor.insertHtml(text_dobavnica);

    QPrinter* printer = new QPrinter(QPrinter::ScreenResolution);

    printer->setPaperSize(QPrinter::A4);
    printer->setOutputFormat(QPrinter::PdfFormat);

    QString stevilka_racuna = ui->lineEdit_stRacuna->text();
    QString stranka = ui->comboBox_narocnik->currentText();
    QRegExp rx("[ ]");
    QStringList list;
    list = stranka.split(rx, QString::SkipEmptyParts);

    if(list.at(1) == "d.o.o." || list.at(1) == "doo" || list.at(1) == "D.O.O."  || list.at(1) == "DOO" || list.at(1) == "s.p." || list.at(1) == "sp" || list.at(1) == "s.p" || list.at(1) == "S.P." || list.at(1) == "S.P" || list.at(1) == "SP")
        stranka = list.at(0);
    else
        stranka = list.at(0) + "_" + list.at(1);

    QString output = m_fileShrani + "/RACUN_IN_DOBAVNICA_" + stranka + "_" + stevilka_racuna + ".pdf";
    printer->setOutputFileName(output);
    QSizeF paperSize;
    paperSize.setWidth(printer->width());
    paperSize.setHeight(printer->height());
    document_racun.setPageSize(paperSize);
    qreal left = 10, right = 10, top = 0, bottom = 5;
    printer->setPageMargins(left, top, right, bottom, QPrinter::Millimeter);
    document_racun.print(printer);
    QDesktopServices::openUrl(QUrl::fromLocalFile(output));  

    /* Star način, ker mi print dialog v QT ne dela (ne začne printat, verjetno driverji), direktno odprem pdf file in sprintam od tam
    QPrintDialog printDlg(printer);
    QList<QWidget*> childWidgets = printDlg.findChildren<QWidget*>(QLatin1String("printers"), Qt::FindChildrenRecursively);

    if (childWidgets.count() == 1)
    {
       QComboBox* comboBox(qobject_cast<QComboBox*>(childWidgets.at(0)));

       if (comboBox && comboBox->findText(printer->printerName()))
          comboBox->setCurrentText(printer->printerName());

    }

    if (printDlg.exec() == QDialog::Accepted)
    {
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setPaperSize(QPrinter::A4);
        QString stevilka_racuna = ui->lineEdit_stRacuna->text();
        QString stranka = ui->comboBox_narocnik->currentText();
        QRegExp rx("[ ]");
        QStringList list;
        list = stranka.split(rx, QString::SkipEmptyParts);

        if(list.at(1) == "d.o.o." || list.at(1) == "doo" || list.at(1) == "D.O.O."  || list.at(1) == "DOO" || list.at(1) == "s.p." || list.at(1) == "sp" || list.at(1) == "s.p" || list.at(1) == "S.P." || list.at(1) == "S.P" || list.at(1) == "SP")
            stranka = list.at(0);
        else
            stranka = list.at(0) + "_" + list.at(1);

        QString output = m_fileShrani + "/RACUN_IN_DOBAVNICA_" + stranka + "_" + stevilka_racuna + ".pdf";
        printer->setOutputFileName(output);
        qreal left = 0, right = 0, top = 0, bottom = 0;
        printer->setPageMargins(left, top, right, bottom, QPrinter::Millimeter);
        document_racun.print(printer);
        // Test odpiranja pdf datoteke
        QDesktopServices::openUrl(QUrl::fromLocalFile(output));
    }
    */
    if(ui->comboBox_narocnik->currentText() == "KRONOTERM d.o.o.")
        MakeXML();
    NovRacun::close();
    return 0;
}

void NovRacun::on_pushButton_dodajNovProdukt_clicked()
{
    DodajProdukt produkt;
    produkt.setModal(true);
    produkt.SetIndex(ui->comboBox_narocnik->currentIndex());
    produkt.exec();
    Read();
}

void NovRacun::on_lineEdit_popust_textChanged()
{
    double pop = ui->lineEdit_popust->text().toDouble();
    double popust(0);
    double skupaj(0);
    double ddv = ui->label_ddv->text().toDouble();
    popust = m_total - (m_total * (pop / 100));
    if(pop == 0.0)
        ui->label_popust->setText("€" + QString::number(m_total, 'f', 2));
    else
        ui->label_popust->setText("€" + QString::number(popust, 'f', 2));

    ddv = popust * 0.22;
    if(ddv < 0)
        ui->label_ddv->setText("€0");
    else
        ui->label_ddv->setText("€" + QString::number(ddv, 'f', 2));

    skupaj = popust + ddv;
    if(skupaj < 0)
        ui->label_skupaj->setText("€0");
    else
        ui->label_skupaj->setText("€" + QString::number(skupaj, 'f', 2));
}

void NovRacun::on_lineEdit_popust_editingFinished()
{
    if(ui->lineEdit_popust->text() == "")
        ui->lineEdit_popust->setText("0");
}

void NovRacun::on_lineEdit_isci_textChanged()
{
    QString search = ui->lineEdit_isci->text();
    if(search == "")
        Read();
    else
    {
        ui->treeWidget_seznam->clear();
        Search(search);
    }
}

void NovRacun::on_lineEdit_stRacuna_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_stRacuna->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit_stRacuna->backspace();
        ui->lineEdit_stRacuna->backspace();
        ui->lineEdit_stRacuna->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit_stRacuna->backspace();
        ui->lineEdit_stRacuna->backspace();
        ui->lineEdit_stRacuna->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_stRacuna->backspace();
    }
}

void NovRacun::on_lineEdit_sklic_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_sklic->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit_sklic->backspace();
        ui->lineEdit_sklic->backspace();
        ui->lineEdit_sklic->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit_sklic->backspace();
        ui->lineEdit_sklic->backspace();
        ui->lineEdit_sklic->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit_sklic->backspace();
    }
}

void NovRacun::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit->backspace();
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ".") {
        ui->lineEdit->backspace();
        ui->lineEdit->backspace();
        ui->lineEdit->insert(".");
    }
    if(arg1.at(arg1.length()-2) == " " && arg1.at(arg1.length()-1) == ",") {
        ui->lineEdit->backspace();
        ui->lineEdit->backspace();
        ui->lineEdit->insert(",");
    }
    if(arg1.length() == 1 && arg1.at(arg1.length()-1) == " ") {
        ui->lineEdit->backspace();
    }
}
