#include "Source.h"

ClientInterface::ClientInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientInterface)
{
    ui->setupUi(this);

    // Connect the ComboBox's currentIndexChanged signal to the on_modeChanged slot
    connect(ui->boxswitch, SIGNAL(currentIndexChanged(int)), this, SLOT(on_modeChanged(int)));
    connect(ui->btnjoinsession, SIGNAL(clicked()), this, SLOT(on_btnjoin_clicked()));
}

ClientInterface::~ClientInterface()
{
    delete ui;
}
std :: string sessioncode;

void ClientInterface::on_btnjoin_clicked()
{
    // Get the text from the QLineEdit and convert it to std::string
    sessioncode = ui->txtsessioncode->text().toStdString();

    // Convert std::string to QString for displaying in QLabel
    QString sessionCodeQString = QString::fromStdString(sessioncode);

    // Set the text of the QLabel to show the session code
    ui->txtresultcode->setText("Session Code: " + sessionCodeQString);

}


void ClientInterface::on_modeChanged(int index)
{

     if (index == 1)
    {
        // server side
        // Enable server-related UI components and disable client-related UI components
         ui->lblserver->setText("Server");
        ui->btncreatesession->setEnabled(true);   // Enable server-related actions
        ui->btnjoinsession->setDisabled(true);// Disable client actions
        ui->txtsessioncode->setDisabled(true); // Disable session code input
        ui->lblclient->setText("");          // Clear session code label

    }else if(index == 2)
    {

        ui->lblclient->setText("Client");
        ui->btncreatesession->setDisabled(true);  // Disable server actions
        ui->txtsessioncode->setEnabled(true);     // Enable session code input
        ui->btnjoinsession->setEnabled(true);     // Enable client actions
        ui->txtresultcode->setEnabled(true);
        ui->lblserver->setText("");          // Optionally clear label

    }


}

