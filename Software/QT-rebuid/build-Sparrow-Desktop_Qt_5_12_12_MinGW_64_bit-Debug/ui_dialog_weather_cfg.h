/********************************************************************************
** Form generated from reading UI file 'dialog_weather_cfg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_WEATHER_CFG_H
#define UI_DIALOG_WEATHER_CFG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_Weather_Cfg
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *leSetWeather;
    QPushButton *btnSetWeather;

    void setupUi(QDialog *Dialog_Weather_Cfg)
    {
        if (Dialog_Weather_Cfg->objectName().isEmpty())
            Dialog_Weather_Cfg->setObjectName(QString::fromUtf8("Dialog_Weather_Cfg"));
        Dialog_Weather_Cfg->resize(320, 100);
        horizontalLayout = new QHBoxLayout(Dialog_Weather_Cfg);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Dialog_Weather_Cfg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        leSetWeather = new QLineEdit(Dialog_Weather_Cfg);
        leSetWeather->setObjectName(QString::fromUtf8("leSetWeather"));

        horizontalLayout->addWidget(leSetWeather);

        btnSetWeather = new QPushButton(Dialog_Weather_Cfg);
        btnSetWeather->setObjectName(QString::fromUtf8("btnSetWeather"));

        horizontalLayout->addWidget(btnSetWeather);


        retranslateUi(Dialog_Weather_Cfg);

        QMetaObject::connectSlotsByName(Dialog_Weather_Cfg);
    } // setupUi

    void retranslateUi(QDialog *Dialog_Weather_Cfg)
    {
        Dialog_Weather_Cfg->setWindowTitle(QApplication::translate("Dialog_Weather_Cfg", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog_Weather_Cfg", "\345\244\251\346\260\224\346\216\245\345\217\243", nullptr));
        btnSetWeather->setText(QApplication::translate("Dialog_Weather_Cfg", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_Weather_Cfg: public Ui_Dialog_Weather_Cfg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_WEATHER_CFG_H
