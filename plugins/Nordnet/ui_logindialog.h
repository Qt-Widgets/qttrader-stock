/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created: Tue Apr 30 19:03:12 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *username;
    QLineEdit *password;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->setWindowModality(Qt::WindowModal);
        Login->resize(400, 180);
        Login->setModal(true);
        buttonBox = new QDialogButtonBox(Login);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 130, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        username = new QLineEdit(Login);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(10, 30, 361, 27));
        password = new QLineEdit(Login);
        password->setObjectName(QString::fromUtf8("password"));
        password->setGeometry(QRect(10, 80, 361, 27));
        password->setEchoMode(QLineEdit::Password);
        label = new QLabel(Login);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 111, 17));
        label_2 = new QLabel(Login);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 60, 111, 17));

        retranslateUi(Login);
        QObject::connect(buttonBox, SIGNAL(accepted()), Login, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Login, SLOT(reject()));

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "Login", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Login", "Anv\303\244ndarnamn:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Login", "L\303\266senord:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
