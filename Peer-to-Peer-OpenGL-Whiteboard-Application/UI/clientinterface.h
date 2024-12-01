#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include "Source.h"

enum FormDataType {
    HOST_SERVER,
    JOIN_SERVER,
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
    ClientInterface(std::mutex& mtx, std::condition_variable& cv, FormData* formData, QWidget *parent = nullptr);
    ~ClientInterface();

private slots:
    void on_btnjoin_clicked();

    void on_modeChanged(int index);

    void on_btncreatesession_clicked();

private:
    Ui::ClientInterface *ui;

    std::mutex& mtx;
    std::condition_variable& cv;
    FormData* formData;
};
#endif // CLIENTINTERFACE_H
