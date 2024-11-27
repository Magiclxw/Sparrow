/********************************************************************************
** Form generated from reading UI file 'dialog_ble_name.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_BLE_NAME_H
#define UI_DIALOG_BLE_NAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_Ble_Name
{
public:
    QHBoxLayout *horizontalLayout;
    QLineEdit *leBleName;
    QPushButton *btnSaveBleName;

    void setupUi(QDialog *Dialog_Ble_Name)
    {
        if (Dialog_Ble_Name->objectName().isEmpty())
            Dialog_Ble_Name->setObjectName(QString::fromUtf8("Dialog_Ble_Name"));
        Dialog_Ble_Name->resize(320, 100);
        horizontalLayout = new QHBoxLayout(Dialog_Ble_Name);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        leBleName = new QLineEdit(Dialog_Ble_Name);
        leBleName->setObjectName(QString::fromUtf8("leBleName"));

        horizontalLayout->addWidget(leBleName);

        btnSaveBleName = new QPushButton(Dialog_Ble_Name);
        btnSaveBleName->setObjectName(QString::fromUtf8("btnSaveBleName"));

        horizontalLayout->addWidget(btnSaveBleName);


        retranslateUi(Dialog_Ble_Name);

        QMetaObject::connectSlotsByName(Dialog_Ble_Name);
    } // setupUi

    void retranslateUi(QDialog *Dialog_Ble_Name)
    {
        Dialog_Ble_Name->setWindowTitle(QApplication::translate("Dialog_Ble_Name", "Dialog", nullptr));
        btnSaveBleName->setText(QApplication::translate("Dialog_Ble_Name", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_Ble_Name: public Ui_Dialog_Ble_Name {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_BLE_NAME_H
