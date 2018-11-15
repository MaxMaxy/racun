#include "testingdialog.h"
#include "ui_testingdialog.h"

TestingDialog::TestingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestingDialog), m_num_of_products(5)
{
    ui->setupUi(this);
}

TestingDialog::~TestingDialog()
{
    delete ui;
}

void TestingDialog::Test()
{
    QDomDocument document;

    // ROOT
    QDomElement root = document.createElement("IzdaniRacunEnostavni");
    root.setAttribute("xmlns:ds", "http://www.w3.org/2000/09/xmldsig#");
    root.setAttribute("xmlns:xds", "http://uri.etsi.org/01903/v1.1.1#");
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.setAttribute("xsi:noNamespaceSchemaLocation", "http://www.gzs.si/e-poslovanje/sheme/eSLOG_1-6_EnostavniRacun.xsd");
    document.appendChild(root);

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
    datumRacuna_2_1.appendChild(datumRacuna_1_1_text);

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
    QDomText text_za_tekst_1_2 = document.createTextNode("Ra&#269un:" + ui->lineEdit_stRacuna->text());
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
    QDomText text_za_tekst_3_2 = document.createTextNode("Zara&#269unavamo vam");
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
    QDomText ulica_1_1_text = document.createTextNode("Andra&#380 nad Polzelo 74 A");
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
    QDomText stevilkaBancnegaRacuna_1_text = document.createTextNode("SI56300000011326144");
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
    QDomText kraj_2_text = document.createTextNode("Braslov&#269e");
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
    QDomText kraj_3_text = document.createTextNode("Braslov&#269e");
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
    QDomText datum_text = document.createTextNode(ui->dateEdit->text() + "T00:00:00.0Z"); // --------> datum + 45 dni
    datum.appendChild(datum_text);

    // POSTAVKE RACUNA
    for(int i(0); i < m_num_of_products; i++)
    {
       QDomElement postavkeRacuna = document.createElement("PostavkeRacuna");
       racun.appendChild(postavkeRacuna);
    }
    for(int i(0); i < 3; i++)
    {
       QDomElement povzetekDavkov = document.createElement("PovzetekDavkovRacuna");
       racun.appendChild(povzetekDavkov);
    }
    for(int i(0); i < 6; i++)
    {
       QDomElement povzetekZneskov = document.createElement("PovzetekZneskovRacuna");
       racun.appendChild(povzetekZneskov);
    }


    QFile file("/usr/home/cloudjunkie/test.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       qDebug() << "Error";
       return;
    }
    else
    {
       QTextStream stream(&file);
       stream << document.toString();
       file.flush();
       file.close();
    }
}

void TestingDialog::on_pushButton_2_clicked()
{
    Test();
}
