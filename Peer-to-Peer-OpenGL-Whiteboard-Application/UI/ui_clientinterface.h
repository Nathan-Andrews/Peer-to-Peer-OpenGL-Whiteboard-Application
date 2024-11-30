/********************************************************************************
** Form generated from reading UI file 'clientinterface.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTINTERFACE_H
#define UI_CLIENTINTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientInterface
{
public:
    QWidget *centralwidget;
    QLabel *lblclient;
    QPushButton *btnjoinsession;
    QLineEdit *txtsessioncode;
    QLineEdit *txtresultcode;
    QComboBox *boxswitch;
    QPushButton *btncreatesession;
    QLabel *lblserver;

    void setupUi(QMainWindow *ClientInterface)
    {
        if (ClientInterface->objectName().isEmpty())
            ClientInterface->setObjectName("ClientInterface");
        ClientInterface->resize(548, 437);
        centralwidget = new QWidget(ClientInterface);
        centralwidget->setObjectName("centralwidget");
        lblclient = new QLabel(centralwidget);
        lblclient->setObjectName("lblclient");
        lblclient->setGeometry(QRect(240, 40, 91, 31));
        lblclient->setStyleSheet(QString::fromUtf8("font: 900 18pt \"Arial Black\";"));
        btnjoinsession = new QPushButton(centralwidget);
        btnjoinsession->setObjectName("btnjoinsession");
        btnjoinsession->setGeometry(QRect(320, 200, 131, 31));
        btnjoinsession->setStyleSheet(QString::fromUtf8("font: 16pt \"Arial\";"));
        txtsessioncode = new QLineEdit(centralwidget);
        txtsessioncode->setObjectName("txtsessioncode");
        txtsessioncode->setGeometry(QRect(120, 130, 311, 31));
        txtsessioncode->setStyleSheet(QString::fromUtf8("font: 14pt \"Arial\";"));
        txtresultcode = new QLineEdit(centralwidget);
        txtresultcode->setObjectName("txtresultcode");
        txtresultcode->setGeometry(QRect(100, 290, 341, 41));
        txtresultcode->setReadOnly(true);
        boxswitch = new QComboBox(centralwidget);
        boxswitch->addItem(QString());
        boxswitch->addItem(QString());
        boxswitch->addItem(QString());
        boxswitch->addItem(QString());
        boxswitch->setObjectName("boxswitch");
        boxswitch->setGeometry(QRect(240, 90, 91, 24));
        boxswitch->setStyleSheet(QString::fromUtf8("font: 14pt \"Arial\";"));
        btncreatesession = new QPushButton(centralwidget);
        btncreatesession->setObjectName("btncreatesession");
        btncreatesession->setGeometry(QRect(120, 200, 161, 31));
        btncreatesession->setStyleSheet(QString::fromUtf8("font: 16pt \"Arial\";"));
        lblserver = new QLabel(centralwidget);
        lblserver->setObjectName("lblserver");
        lblserver->setGeometry(QRect(240, 40, 91, 31));
        lblserver->setStyleSheet(QString::fromUtf8("font: 900 18pt \"Arial Black\";"));
        ClientInterface->setCentralWidget(centralwidget);

        retranslateUi(ClientInterface);

        QMetaObject::connectSlotsByName(ClientInterface);
    } // setupUi

    void retranslateUi(QMainWindow *ClientInterface)
    {
        ClientInterface->setWindowTitle(QCoreApplication::translate("ClientInterface", "ClientInterface", nullptr));
        lblclient->setText(QString());
        btnjoinsession->setText(QCoreApplication::translate("ClientInterface", "Join Server", nullptr));
        boxswitch->setItemText(0, QString());
        boxswitch->setItemText(1, QCoreApplication::translate("ClientInterface", "Server", nullptr));
        boxswitch->setItemText(2, QCoreApplication::translate("ClientInterface", "Client", nullptr));
        boxswitch->setItemText(3, QString());

        btncreatesession->setText(QCoreApplication::translate("ClientInterface", "Create Session", nullptr));
        lblserver->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ClientInterface: public Ui_ClientInterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTINTERFACE_H
