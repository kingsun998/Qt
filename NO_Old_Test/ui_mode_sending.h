/********************************************************************************
** Form generated from reading UI file 'mode_sending.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODE_SENDING_H
#define UI_MODE_SENDING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mode_sending
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QComboBox *comboBox;
    QLabel *label_2;
    QComboBox *comboBox_2;
    QLabel *label;
    QComboBox *comboBox_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_13;
    QSpinBox *spinBox_3;
    QLabel *label_4;
    QSpinBox *spinBox_2;
    QLabel *label_14;
    QSpinBox *spinBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QLabel *label_6;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget;

    void setupUi(QWidget *mode_sending)
    {
        if (mode_sending->objectName().isEmpty())
            mode_sending->setObjectName(QString::fromUtf8("mode_sending"));
        mode_sending->resize(499, 385);
        verticalLayout = new QVBoxLayout(mode_sending);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(mode_sending);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        comboBox = new QComboBox(groupBox);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout->addWidget(comboBox);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        comboBox_2 = new QComboBox(groupBox);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        horizontalLayout->addWidget(comboBox_2);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        comboBox_3 = new QComboBox(groupBox);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));

        horizontalLayout->addWidget(comboBox_3);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_2->addWidget(label_13);

        spinBox_3 = new QSpinBox(groupBox);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setMinimum(1);

        horizontalLayout_2->addWidget(spinBox_3);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_2->addWidget(label_4);

        spinBox_2 = new QSpinBox(groupBox);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMinimum(8);

        horizontalLayout_2->addWidget(spinBox_2);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_2->addWidget(label_14);

        spinBox = new QSpinBox(groupBox);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);

        horizontalLayout_2->addWidget(spinBox);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_3->addWidget(label_6);

        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_3->addWidget(lineEdit_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_4->addWidget(pushButton);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_4->addWidget(pushButton_2);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(mode_sending);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tableWidget = new QTableWidget(groupBox_2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout_3->addWidget(tableWidget);


        verticalLayout->addWidget(groupBox_2);


        retranslateUi(mode_sending);

        QMetaObject::connectSlotsByName(mode_sending);
    } // setupUi

    void retranslateUi(QWidget *mode_sending)
    {
        mode_sending->setWindowTitle(QApplication::translate("mode_sending", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("mode_sending", "\345\270\247\345\217\221\351\200\201", nullptr));
        label_3->setText(QApplication::translate("mode_sending", "\345\270\247\346\240\274\345\274\217\357\274\232", nullptr));
        comboBox->setItemText(0, QApplication::translate("mode_sending", "\346\240\207\345\207\206\345\270\247", nullptr));
        comboBox->setItemText(1, QApplication::translate("mode_sending", "\346\211\251\345\261\225\345\270\247", nullptr));

        label_2->setText(QApplication::translate("mode_sending", "\345\270\247\347\261\273\345\236\213\357\274\232", nullptr));
        comboBox_2->setItemText(0, QApplication::translate("mode_sending", "\346\225\260\346\215\256\345\270\247", nullptr));
        comboBox_2->setItemText(1, QApplication::translate("mode_sending", "\350\277\234\347\250\213\345\270\247", nullptr));

        label->setText(QApplication::translate("mode_sending", "\351\200\232\351\201\223\357\274\232", nullptr));
        label_13->setText(QApplication::translate("mode_sending", "\346\257\217\346\254\241\351\227\264\351\232\224(ms):", nullptr));
        label_4->setText(QApplication::translate("mode_sending", "\346\225\260\346\215\256\351\225\277\345\272\246\357\274\232 ", nullptr));
        label_14->setText(QApplication::translate("mode_sending", "\346\257\217\346\254\241\345\217\221\351\200\201\345\270\247\346\225\260\357\274\232", nullptr));
        label_5->setText(QApplication::translate("mode_sending", "\346\225\260\346\215\256\357\274\2320x", nullptr));
        label_6->setText(QApplication::translate("mode_sending", "\345\217\221\351\200\201\346\254\241\346\225\260\357\274\232", nullptr));
        pushButton->setText(QApplication::translate("mode_sending", "\345\217\221\351\200\201", nullptr));
        pushButton_2->setText(QApplication::translate("mode_sending", "\345\217\226\346\266\210", nullptr));
        groupBox_2->setTitle(QApplication::translate("mode_sending", "\345\217\221\351\200\201\345\210\227\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mode_sending: public Ui_mode_sending {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODE_SENDING_H
