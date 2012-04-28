/********************************************************************************
** Form generated from reading UI file 'errordetails.ui'
**
** Created: Mon Aug 9 14:25:09 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ERRORDETAILS_H
#define ERRORDETAILS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ErrorDetails
{
public:
    QVBoxLayout *vboxLayout;
    QTextBrowser *testresults;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *ErrorDetails)
    {
        if (ErrorDetails->objectName().isEmpty())
            ErrorDetails->setObjectName(QString::fromUtf8("ErrorDetails"));
        ErrorDetails->setEnabled(true);
        ErrorDetails->resize(477, 458);
        ErrorDetails->setAcceptDrops(false);
        vboxLayout = new QVBoxLayout(ErrorDetails);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        testresults = new QTextBrowser(ErrorDetails);
        testresults->setObjectName(QString::fromUtf8("testresults"));

        vboxLayout->addWidget(testresults);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(ErrorDetails);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout->addWidget(okButton);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem1);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(ErrorDetails);
        QObject::connect(okButton, SIGNAL(clicked()), ErrorDetails, SLOT(close()));

        QMetaObject::connectSlotsByName(ErrorDetails);
    } // setupUi

    void retranslateUi(QDialog *ErrorDetails)
    {
        ErrorDetails->setWindowTitle(QApplication::translate("ErrorDetails", "Verification failed", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("ErrorDetails", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ErrorDetails: public Ui_ErrorDetails {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ERRORDETAILS_H
