/********************************************************************************
** Form generated from reading UI file 'mode_upload.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODE_UPLOAD_H
#define UI_MODE_UPLOAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mode_upload
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *mode_upload)
    {
        if (mode_upload->objectName().isEmpty())
            mode_upload->setObjectName(QString::fromUtf8("mode_upload"));
        mode_upload->resize(538, 427);
        pushButton = new QPushButton(mode_upload);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(190, 390, 131, 23));

        retranslateUi(mode_upload);

        QMetaObject::connectSlotsByName(mode_upload);
    } // setupUi

    void retranslateUi(QWidget *mode_upload)
    {
        mode_upload->setWindowTitle(QApplication::translate("mode_upload", "Form", nullptr));
        pushButton->setText(QApplication::translate("mode_upload", "\345\217\221\351\200\201\346\225\260\346\215\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mode_upload: public Ui_mode_upload {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODE_UPLOAD_H
