/********************************************************************************
** Form generated from reading UI file 'dialog_sleep_time.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_SLEEP_TIME_H
#define UI_DIALOG_SLEEP_TIME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Dialog_Sleep_Time
{
public:
    QHBoxLayout *horizontalLayout;
    QSpinBox *sbSleepHour;
    QLabel *label;
    QSpinBox *sbSleepMinute;
    QLabel *label_2;
    QPushButton *btnSaveSleepTime;

    void setupUi(QDialog *Dialog_Sleep_Time)
    {
        if (Dialog_Sleep_Time->objectName().isEmpty())
            Dialog_Sleep_Time->setObjectName(QString::fromUtf8("Dialog_Sleep_Time"));
        Dialog_Sleep_Time->resize(320, 100);
        horizontalLayout = new QHBoxLayout(Dialog_Sleep_Time);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        sbSleepHour = new QSpinBox(Dialog_Sleep_Time);
        sbSleepHour->setObjectName(QString::fromUtf8("sbSleepHour"));

        horizontalLayout->addWidget(sbSleepHour);

        label = new QLabel(Dialog_Sleep_Time);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        sbSleepMinute = new QSpinBox(Dialog_Sleep_Time);
        sbSleepMinute->setObjectName(QString::fromUtf8("sbSleepMinute"));

        horizontalLayout->addWidget(sbSleepMinute);

        label_2 = new QLabel(Dialog_Sleep_Time);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        btnSaveSleepTime = new QPushButton(Dialog_Sleep_Time);
        btnSaveSleepTime->setObjectName(QString::fromUtf8("btnSaveSleepTime"));

        horizontalLayout->addWidget(btnSaveSleepTime);


        retranslateUi(Dialog_Sleep_Time);

        QMetaObject::connectSlotsByName(Dialog_Sleep_Time);
    } // setupUi

    void retranslateUi(QDialog *Dialog_Sleep_Time)
    {
        Dialog_Sleep_Time->setWindowTitle(QApplication::translate("Dialog_Sleep_Time", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog_Sleep_Time", "\345\260\217\346\227\266", nullptr));
        label_2->setText(QApplication::translate("Dialog_Sleep_Time", "\345\210\206\351\222\237", nullptr));
        btnSaveSleepTime->setText(QApplication::translate("Dialog_Sleep_Time", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_Sleep_Time: public Ui_Dialog_Sleep_Time {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_SLEEP_TIME_H
