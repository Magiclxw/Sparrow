/********************************************************************************
** Form generated from reading UI file 'dialog_mqtt_cfg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_MQTT_CFG_H
#define UI_DIALOG_MQTT_CFG_H

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

class Ui_Dialog_Mqtt_cfg
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *leMqttAddr;
    QPushButton *btnMqttSaveAddr;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLineEdit *leMqttUserName;
    QPushButton *btnMqttSaveUsername;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *leMqttPassword;
    QPushButton *btnMqttSavePassword;

    void setupUi(QDialog *Dialog_Mqtt_cfg)
    {
        if (Dialog_Mqtt_cfg->objectName().isEmpty())
            Dialog_Mqtt_cfg->setObjectName(QString::fromUtf8("Dialog_Mqtt_cfg"));
        Dialog_Mqtt_cfg->resize(320, 240);
        verticalLayout = new QVBoxLayout(Dialog_Mqtt_cfg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(Dialog_Mqtt_cfg);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout = new QHBoxLayout(widget_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(widget_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_2);

        leMqttAddr = new QLineEdit(widget_3);
        leMqttAddr->setObjectName(QString::fromUtf8("leMqttAddr"));
        leMqttAddr->setMaximumSize(QSize(170, 16777215));

        horizontalLayout->addWidget(leMqttAddr);

        btnMqttSaveAddr = new QPushButton(widget_3);
        btnMqttSaveAddr->setObjectName(QString::fromUtf8("btnMqttSaveAddr"));
        btnMqttSaveAddr->setMaximumSize(QSize(35, 16777215));

        horizontalLayout->addWidget(btnMqttSaveAddr);


        verticalLayout_2->addWidget(widget_3);

        widget_4 = new QWidget(widget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        horizontalLayout_2 = new QHBoxLayout(widget_4);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_3);

        leMqttUserName = new QLineEdit(widget_4);
        leMqttUserName->setObjectName(QString::fromUtf8("leMqttUserName"));
        leMqttUserName->setMaximumSize(QSize(170, 16777215));

        horizontalLayout_2->addWidget(leMqttUserName);

        btnMqttSaveUsername = new QPushButton(widget_4);
        btnMqttSaveUsername->setObjectName(QString::fromUtf8("btnMqttSaveUsername"));
        btnMqttSaveUsername->setMaximumSize(QSize(35, 16777215));

        horizontalLayout_2->addWidget(btnMqttSaveUsername);


        verticalLayout_2->addWidget(widget_4);

        widget_5 = new QWidget(widget);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        horizontalLayout_3 = new QHBoxLayout(widget_5);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(widget_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_4);

        leMqttPassword = new QLineEdit(widget_5);
        leMqttPassword->setObjectName(QString::fromUtf8("leMqttPassword"));
        leMqttPassword->setMaximumSize(QSize(170, 16777215));

        horizontalLayout_3->addWidget(leMqttPassword);

        btnMqttSavePassword = new QPushButton(widget_5);
        btnMqttSavePassword->setObjectName(QString::fromUtf8("btnMqttSavePassword"));
        btnMqttSavePassword->setMaximumSize(QSize(35, 16777215));

        horizontalLayout_3->addWidget(btnMqttSavePassword);


        verticalLayout_2->addWidget(widget_5);


        verticalLayout->addWidget(widget);


        retranslateUi(Dialog_Mqtt_cfg);

        QMetaObject::connectSlotsByName(Dialog_Mqtt_cfg);
    } // setupUi

    void retranslateUi(QDialog *Dialog_Mqtt_cfg)
    {
        Dialog_Mqtt_cfg->setWindowTitle(QApplication::translate("Dialog_Mqtt_cfg", "\346\234\215\345\212\241\345\231\250\350\256\276\347\275\256", nullptr));
        label_2->setText(QApplication::translate("Dialog_Mqtt_cfg", "\345\234\260\345\235\200", nullptr));
        btnMqttSaveAddr->setText(QApplication::translate("Dialog_Mqtt_cfg", "\344\277\235\345\255\230", nullptr));
        label_3->setText(QApplication::translate("Dialog_Mqtt_cfg", "\347\224\250\346\210\267\345\220\215", nullptr));
        btnMqttSaveUsername->setText(QApplication::translate("Dialog_Mqtt_cfg", "\344\277\235\345\255\230", nullptr));
        label_4->setText(QApplication::translate("Dialog_Mqtt_cfg", "\345\257\206\347\240\201", nullptr));
        btnMqttSavePassword->setText(QApplication::translate("Dialog_Mqtt_cfg", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_Mqtt_cfg: public Ui_Dialog_Mqtt_cfg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_MQTT_CFG_H
