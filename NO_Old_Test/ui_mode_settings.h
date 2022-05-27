/********************************************************************************
** Form generated from reading UI file 'mode_settings.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODE_SETTINGS_H
#define UI_MODE_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mode_settings
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;

    void setupUi(QWidget *mode_settings)
    {
        if (mode_settings->objectName().isEmpty())
            mode_settings->setObjectName(QString::fromUtf8("mode_settings"));
        mode_settings->resize(466, 389);
        verticalLayout = new QVBoxLayout(mode_settings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(mode_settings);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));

        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(mode_settings);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));

        verticalLayout->addWidget(groupBox_2);


        retranslateUi(mode_settings);

        QMetaObject::connectSlotsByName(mode_settings);
    } // setupUi

    void retranslateUi(QWidget *mode_settings)
    {
        mode_settings->setWindowTitle(QApplication::translate("mode_settings", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("mode_settings", "CAN\350\256\276\347\275\256", nullptr));
        groupBox_2->setTitle(QApplication::translate("mode_settings", "MQTT\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mode_settings: public Ui_mode_settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODE_SETTINGS_H
