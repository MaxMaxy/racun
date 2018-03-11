#include "testingdialog.h"
#include "ui_testingdialog.h"

TestingDialog::TestingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestingDialog)
{
    ui->setupUi(this);
}

TestingDialog::~TestingDialog()
{
    delete ui;
}

void TestingDialog::Test()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
    db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + QString("/usr/home/cloudjunkie/Documents/Test.xlsx"));
    qDebug() << db.isValid();
    if(db.open())
    {
     QSqlQuery query("select * from [" + QString("Sheet1") + "$]"); // Select range, place A1:B5 after $
     while (query.next())
     {
     QString column1= query.value(0).toString();
     qDebug() << column1;
     }
    db.close();
    QSqlDatabase::removeDatabase("xlsx_connection");
    }
    else
        qDebug() << "Not open";
}

void TestingDialog::on_pushButton_2_clicked()
{
    Test();
}
