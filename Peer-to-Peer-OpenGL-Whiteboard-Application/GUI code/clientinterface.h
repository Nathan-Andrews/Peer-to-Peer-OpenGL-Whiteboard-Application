#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QMainWindow>

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
    void on_btnjoin_clicked();

    void on_modeChanged(int index);

    void on_btncreatesession_clicked();

private:
    Ui::ClientInterface *ui;
};
#endif // CLIENTINTERFACE_H
