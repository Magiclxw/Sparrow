/********************************************************************************
** Form generated from reading UI file 'dialog_wifi_cfg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_WIFI_CFG_H
#define UI_DIALOG_WIFI_CFG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_Wifi_CFG
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *leSsid;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLineEdit *lePassword;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnWifiSave;
    QPushButton *btnWifiCancel;

    void setupUi(QDialog *Dialog_Wifi_CFG)
    {
        if (Dialog_Wifi_CFG->objectName().isEmpty())
            Dialog_Wifi_CFG->setObjectName(QString::fromUtf8("Dialog_Wifi_CFG"));
        Dialog_Wifi_CFG->resize(320, 240);
        verticalLayout = new QVBoxLayout(Dialog_Wifi_CFG);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(Dialog_Wifi_CFG);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        leSsid = new QLineEdit(widget_2);
        leSsid->setObjectName(QString::fromUtf8("leSsid"));

        horizontalLayout->addWidget(leSsid);


        verticalLayout_2->addWidget(widget_2);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        lePassword = new QLineEdit(widget_3);
        lePassword->setObjectName(QString::fromUtf8("lePassword"));
        lePassword->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(lePassword);


        verticalLayout_2->addWidget(widget_3);

        widget_4 = new QWidget(widget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btnWifiSave = new QPushButton(widget_4);
        btnWifiSave->setObjectName(QString::fromUtf8("btnWifiSave"));

        horizontalLayout_3->addWidget(btnWifiSave);

        btnWifiCancel = new QPushButton(widget_4);
        btnWifiCancel->setObjectName(QString::fromUtf8("btnWifiCancel"));

        horizontalLayout_3->addWidget(btnWifiCancel);


        verticalLayout_2->addWidget(widget_4);


        verticalLayout->addWidget(widget);


        retranslateUi(Dialog_Wifi_CFG);

        QMetaObject::connectSlotsByName(Dialog_Wifi_CFG);
    } // setupUi

    void retranslateUi(QDialog *Dialog_Wifi_CFG)
    {
        Dialog_Wifi_CFG->setWindowTitle(QApplication::translate("Dialog_Wifi_CFG", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog_Wifi_CFG", "wifi\350\256\276\347\275\256", nullptr));
        label_2->setText(QApplication::translate("Dialog_Wifi_CFG", "\345\220\215\347\247\260", nullptr));
        label_3->setText(QApplication::translate("Dialog_Wifi_CFG", "\345\257\206\347\240\201", nullptr));
        btnWifiSave->setText(QApplication::translate("Dialog_Wifi_CFG", "\344\277\235\345\255\230", nullptr));
        btnWifiCancel->setText(QApplication::translate("Dialog_Wifi_CFG", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_Wifi_CFG: public Ui_Dialog_Wifi_CFG {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_WIFI_CFG_H
