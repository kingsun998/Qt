/********************************************************************************
** Form generated from reading UI file 'messagedisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEDISPLAY_H
#define UI_MESSAGEDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessageDisplay
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;

    void setupUi(QWidget *MessageDisplay)
    {
        if (MessageDisplay->objectName().isEmpty())
            MessageDisplay->setObjectName(QString::fromUtf8("MessageDisplay"));
        MessageDisplay->resize(524, 382);
        verticalLayout = new QVBoxLayout(MessageDisplay);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableWidget = new QTableWidget(MessageDisplay);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout->addWidget(tableWidget);


        retranslateUi(MessageDisplay);

        QMetaObject::connectSlotsByName(MessageDisplay);
    } // setupUi

    void retranslateUi(QWidget *MessageDisplay)
    {
        MessageDisplay->setWindowTitle(QApplication::translate("MessageDisplay", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MessageDisplay: public Ui_MessageDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEDISPLAY_H
