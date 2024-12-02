/********************************************************************************
** Form generated from reading UI file 'Interface.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Interface
{
public:
    QWidget *centralwidget;
    QLineEdit *txtsessioncode;
    QPushButton *btncreatesession;
    QPushButton *brnjoinsession;
    QLineEdit *txtresultcode;
    QPushButton *btnendsession;

    void setupUi(QMainWindow *Interface)
    {
        if (Interface->objectName().isEmpty())
            Interface->setObjectName("Interface");
        Interface->resize(527, 406);
        Interface->setStyleSheet(QString::fromUtf8("font: 14pt \"Arial\";"));
        centralwidget = new QWidget(Interface);
        centralwidget->setObjectName("centralwidget");
        txtsessioncode = new QLineEdit(centralwidget);
        txtsessioncode->setObjectName("txtsessioncode");
        txtsessioncode->setGeometry(QRect(100, 100, 341, 31));
        btncreatesession = new QPushButton(centralwidget);
        btncreatesession->setObjectName("btncreatesession");
        btncreatesession->setGeometry(QRect(100, 170, 161, 31));
        btncreatesession->setStyleSheet(QString::fromUtf8("font: 16pt \"Arial\";"));
        brnjoinsession = new QPushButton(centralwidget);
        brnjoinsession->setObjectName("brnjoinsession");
        brnjoinsession->setGeometry(QRect(290, 170, 141, 31));
        brnjoinsession->setStyleSheet(QString::fromUtf8("font: 16pt \"Arial\";"));
        txtresultcode = new QLineEdit(centralwidget);
        txtresultcode->setObjectName("txtresultcode");
        txtresultcode->setGeometry(QRect(100, 280, 341, 31));
        txtresultcode->setReadOnly(true);
        btnendsession = new QPushButton(centralwidget);
        btnendsession->setObjectName("btnendsession");
        btnendsession->setGeometry(QRect(190, 220, 161, 31));
        btnendsession->setStyleSheet(QString::fromUtf8("font: 16pt \"Arial\";"));
        Interface->setCentralWidget(centralwidget);

        retranslateUi(Interface);

        QMetaObject::connectSlotsByName(Interface);
    } // setupUi

    void retranslateUi(QMainWindow *Interface)
    {
        Interface->setWindowTitle(QCoreApplication::translate("Interface", "Interface", nullptr));
        btncreatesession->setText(QCoreApplication::translate("Interface", "Create Session", nullptr));
        brnjoinsession->setText(QCoreApplication::translate("Interface", "Join Server", nullptr));
        btnendsession->setText(QCoreApplication::translate("Interface", "End Session", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Interface: public Ui_Interface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACE_H
