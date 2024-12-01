#include "interface.h"
#include "ui_interface.h"
#include <QString>

Interface::Interface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Interface)
{
    ui->setupUi(this);
}

Interface::~Interface()
{
    delete ui;
}
std :: string sessioncode;
void Interface::on_btncreatesession_clicked()
{
    sessioncode = "123.123.123";
    // Convert std::string to QString for displaying in QLabel
    QString sessionCodeQString = QString::fromStdString(sessioncode);
    ui->txtresultcode->setText("Session Code: " + sessionCodeQString);
    ui->txtsessioncode->hide();
    ui->brnjoinsession->hide();
    ui->btncreatesession->hide();
}


void Interface::on_brnjoinsession_clicked()
{

    // Get the text from the QLineEdit and convert it to std::string
    sessioncode = ui->txtsessioncode->text().toStdString();

    // Convert std::string to QString for displaying in QLabel
    QString sessionCodeQString = QString::fromStdString(sessioncode);

    // Set the text of the QLabel to show the session code
    ui->txtresultcode->setText("Session Code: " + sessionCodeQString);


}


void Interface::on_btnendsession_clicked()
{
    this->close();
}

