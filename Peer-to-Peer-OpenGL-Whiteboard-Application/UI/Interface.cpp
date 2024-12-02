#include "Source.h"


Interface::Interface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Interface)
{
    ui->setupUi(this);

    // Connect the ComboBox's currentIndexChanged signal to the on_modeChanged slot
    // connect(ui->boxswitch, SIGNAL(currentIndexChanged(int)), this, SLOT(on_modeChanged(int)));
    // connect(ui->btnjoinsession, SIGNAL(clicked()), this, SLOT(on_btnjoin_clicked()));
}

Interface::~Interface()
{
    delete ui;
}
std :: string sessioncode;

void Interface::on_brnjoinsession_clicked()
{
    // Get the text from the QLineEdit and convert it to std::string
    // sessioncode = ui->txtsessioncode->text().toStdString();

    SessionCode sessionCode;
    try {
        sessionCode = SessionCode(ui->txtsessioncode->text().toStdString());
    }
    catch (...) {
        ui->txtresultcode->setText(QString::fromStdString("invalid code"));
        return;
    }

    std::cout << "decoded: " << sessionCode.host.ip << ":" << sessionCode.host.port << std::endl;

    {
        std::lock_guard<std::mutex> lock(form_mtx);
        formData->type = JOIN_SERVER;
        formData->host = sessionCode.host;
    }
    ui->txtresultcode->setText(QString::fromStdString("connecting..."));

    wait_form.notify_all(); // Notify all waiting threads

    {
        std::unique_lock<std::mutex> lock(form_mtx);
        wait_form.wait(lock, [] { return formData->type == JOIN_SUCCESS || formData->type == JOIN_FAIL; });

        if (formData->type == JOIN_SUCCESS) {
            formData->type = NONE;

            ui->txtresultcode->setText(QString::fromStdString("join success"));

            this->close();
        }
        else if (formData->type == JOIN_FAIL) {
            formData->type = NONE;

            ui->txtresultcode->setText(QString::fromStdString("invalid code"));
        }
    }
}
void Interface::on_btncreatesession_clicked()
{
    {
        std::lock_guard<std::mutex> lock(form_mtx);
        formData->type = HOST_SERVER;
    }
    wait_form.notify_all(); // Notify all waiting threads

    SessionCode sessionCode;

    {
        std::unique_lock<std::mutex> lock(form_mtx);
        wait_form.wait(lock, [] { return formData->type == SESSION_LINK; });

        sessionCode = SessionCode(formData->host);

        std::cout << "session code: " << sessionCode.generateCode() << std::endl;
    }

    sessioncode = sessionCode.generateCode();


    // Convert std::string to QString for displaying in QLabel
    QString sessionCodeQString = QString::fromStdString(sessioncode);

    ui->txtresultcode->setText("Session Code: " + sessionCodeQString);

    ui->txtsessioncode->hide();
    ui->brnjoinsession->hide();
    ui->btncreatesession->hide();
}

void Interface::on_btnendsession_clicked() {
    // Close the window
    this->close();
}

// void Interface::on_modeChanged(int index)
// {

//      if (index == 1)
//     {
//         // server side
//         // Enable server-related UI components and disable client-related UI components
//          ui->lblserver->setText("Server");
//         ui->btncreatesession->setEnabled(true);   // Enable server-related actions
//         ui->btnjoinsession->setDisabled(true);// Disable client actions
//         ui->txtsessioncode->setDisabled(true); // Disable session code input
//         ui->lblclient->setText("");          // Clear session code label

//     }else if(index == 2)
//     {

//         ui->lblclient->setText("Client");
//         ui->btncreatesession->setDisabled(true);  // Disable server actions
//         ui->txtsessioncode->setEnabled(true);     // Enable session code input
//         ui->btnjoinsession->setEnabled(true);     // Enable client actions
//         ui->txtresultcode->setEnabled(true);
//         ui->lblserver->setText("");          // Optionally clear label

//     }
// }


