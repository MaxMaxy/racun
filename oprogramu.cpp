#include "oprogramu.h"
#include "ui_oprogramu.h"

Oprogramu::Oprogramu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Oprogramu)
{
    ui->setupUi(this);
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("O programu");
    this->showMaximized();
    QScrollArea *scrollArea = new QScrollArea(this);
    QGroupBox *groupBox = new QGroupBox(scrollArea);
    groupBox->setLayout(ui->gridLayout);
    scrollArea->setWidget(groupBox);
    scrollArea->setWidgetResizable(true);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(scrollArea);
    setLayout(layout);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

Oprogramu::~Oprogramu()
{
    delete ui;
}
