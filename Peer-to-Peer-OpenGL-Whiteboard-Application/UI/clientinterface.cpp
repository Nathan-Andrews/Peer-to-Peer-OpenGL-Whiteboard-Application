#include "Source.h"

ClientInterface::ClientInterface(std::mutex& mtx, std::condition_variable& cv, FormData* formData ,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientInterface)
    , mtx(mtx)
    , cv(cv)
    , formData(formData)
{
    ui->setupUi(this);

    // Connect the ComboBox's currentIndexChanged signal to the on_modeChanged slot
    connect(ui->boxswitch, SIGNAL(currentIndexChanged(int)), this, SLOT(on_modeChanged(int)));
    connect(ui->btnjoinsession, SIGNAL(clicked()), this, SLOT(on_btnjoin_clicked()));
}

ClientInterface::ClientInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientInterface)
    , mtx(mtx)
    , cv(cv)
    , formData(nullptr)
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
    // sessioncode = ui->txtsessioncode->text().toStdString();

    SessionCode sessionCode(ui->txtsessioncode->text().toStdString());

    std::cout << "decoded: " << sessionCode.host.ip << ":" << sessionCode.host.port << std::endl;

    {
        std::lock_guard<std::mutex> lock(mtx);
        formData->type = JOIN_SERVER;
        formData->host = sessionCode.host;
    }
    cv.notify_all(); // Notify all waiting threads

    // Convert std::string to QString for displaying in QLabel
    // QString sessionCodeQString = QString::fromStdString(sessioncode);

    // Set the text of the QLabel to show the session code
    // ui->txtresultcode->setText("Session Code: " + sessionCodeQString);
}
void ClientInterface::on_btncreatesession_clicked()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        formData->type = HOST_SERVER;
    }
    cv.notify_all(); // Notify all waiting threads

    SessionCode sessionCode;

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return formData->type == SESSION_LINK; });

        sessionCode = SessionCode(formData->host);

        std::cout << "session code: " << sessionCode.generateCode() << std::endl;
    }

    sessioncode = sessionCode.generateCode();


    // Convert std::string to QString for displaying in QLabel
    QString sessionCodeQString = QString::fromStdString(sessioncode);

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


