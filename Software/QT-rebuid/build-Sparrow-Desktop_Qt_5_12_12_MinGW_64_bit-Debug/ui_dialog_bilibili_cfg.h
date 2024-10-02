/********************************************************************************
** Form generated from reading UI file 'dialog_bilibili_cfg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_BILIBILI_CFG_H
#define UI_DIALOG_BILIBILI_CFG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_Bilibili_Cfg
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *leSetBilibili;
    QPushButton *btnSetBilibili;

    void setupUi(QDialog *Dialog_Bilibili_Cfg)
    {
        if (Dialog_Bilibili_Cfg->objectName().isEmpty())
            Dialog_Bilibili_Cfg->setObjectName(QString::fromUtf8("Dialog_Bilibili_Cfg"));
        Dialog_Bilibili_Cfg->resize(320, 100);
        horizontalLayout = new QHBoxLayout(Dialog_Bilibili_Cfg);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Dialog_Bilibili_Cfg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        leSetBilibili = new QLineEdit(Dialog_Bilibili_Cfg);
        leSetBilibili->setObjectName(QString::fromUtf8("leSetBilibili"));

        horizontalLayout->addWidget(leSetBilibili);

        btnSetBilibili = new QPushButton(Dialog_Bilibili_Cfg);
        btnSetBilibili->setObjectName(QString::fromUtf8("btnSetBilibili"));

        horizontalLayout->addWidget(btnSetBilibili);


        retranslateUi(Dialog_Bilibili_Cfg);

        QMetaObject::connectSlotsByName(Dialog_Bilibili_Cfg);
    } // setupUi

    void retranslateUi(QDialog *Dialog_Bilibili_Cfg)
    {
        Dialog_Bilibili_Cfg->setWindowTitle(QApplication::translate("Dialog_Bilibili_Cfg", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog_Bilibili_Cfg", "Bilibili\346\216\245\345\217\243", nullptr));
        btnSetBilibili->setText(QApplication::translate("Dialog_Bilibili_Cfg", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_Bilibili_Cfg: public Ui_Dialog_Bilibili_Cfg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_BILIBILI_CFG_H
