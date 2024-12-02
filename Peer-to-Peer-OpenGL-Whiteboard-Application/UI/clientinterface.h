#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include "Source.h"
#include "Globals.h"

enum FormDataType {
    HOST_SERVER, // indicates that the user pressed the "create session" button
    JOIN_SERVER, // indicates that the user pressed the "join server" button
    SESSION_LINK, // indicates that the server is sending the ip and port do be displayed in the window as a session ID
    JOIN_SUCCESS, // indicates that the code was valid.  Which means the GUI should close
    JOIN_FAIL, // indicates that the join failed (invalid code).  So the user needs to input a new code
    NONE
};

struct FormData {
    FormDataType type = NONE;
    Host host;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class ClientInterface;
}
QT_END_NAMESPACE

class ClientInterface : public QMainWindow
{
    Q_OBJECT

public:
    ClientInterface(QWidget *parent = nullptr);
    ~ClientInterface();

private slots:
    void on_brnjoinsession_clicked();

    // void on_modeChanged(int index);

    void on_btncreatesession_clicked();

    void on_btnendsession_clicked();

private:
    Ui::Interface *ui;
};
#endif // CLIENTINTERFACE_H
