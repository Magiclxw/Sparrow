/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_4;
    QTextEdit *teRecText;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QListWidget *lwRecFiles;
    QPushButton *btnOpenFloader;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btnSetWifi;
    QPushButton *btnSetWeather;
    QPushButton *btnSetMqtt;
    QPushButton *btnSetBilibili;
    QPushButton *btnSleepTime;
    QPushButton *btnSetBleName;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_5;
    QCheckBox *checkBox;
    QCheckBox *cbPCMonitor;
    QWidget *widget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(218, 385);
        MainWindow->setMaximumSize(QSize(218, 16777215));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMaximumSize(QSize(200, 16777215));
        tabWidget->setTabPosition(QTabWidget::West);
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_4 = new QVBoxLayout(tab_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        teRecText = new QTextEdit(tab_2);
        teRecText->setObjectName(QString::fromUtf8("teRecText"));
        teRecText->setMaximumSize(QSize(200, 16777215));
        teRecText->setReadOnly(false);

        verticalLayout_4->addWidget(teRecText);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lwRecFiles = new QListWidget(tab_3);
        lwRecFiles->setObjectName(QString::fromUtf8("lwRecFiles"));

        verticalLayout_3->addWidget(lwRecFiles);

        btnOpenFloader = new QPushButton(tab_3);
        btnOpenFloader->setObjectName(QString::fromUtf8("btnOpenFloader"));

        verticalLayout_3->addWidget(btnOpenFloader);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_2 = new QVBoxLayout(tab_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        btnSetWifi = new QPushButton(tab_4);
        btnSetWifi->setObjectName(QString::fromUtf8("btnSetWifi"));

        verticalLayout_2->addWidget(btnSetWifi);

        btnSetWeather = new QPushButton(tab_4);
        btnSetWeather->setObjectName(QString::fromUtf8("btnSetWeather"));

        verticalLayout_2->addWidget(btnSetWeather);

        btnSetMqtt = new QPushButton(tab_4);
        btnSetMqtt->setObjectName(QString::fromUtf8("btnSetMqtt"));

        verticalLayout_2->addWidget(btnSetMqtt);

        btnSetBilibili = new QPushButton(tab_4);
        btnSetBilibili->setObjectName(QString::fromUtf8("btnSetBilibili"));

        verticalLayout_2->addWidget(btnSetBilibili);

        btnSleepTime = new QPushButton(tab_4);
        btnSleepTime->setObjectName(QString::fromUtf8("btnSleepTime"));

        verticalLayout_2->addWidget(btnSleepTime);

        btnSetBleName = new QPushButton(tab_4);
        btnSetBleName->setObjectName(QString::fromUtf8("btnSetBleName"));

        verticalLayout_2->addWidget(btnSetBleName);

        widget_2 = new QWidget(tab_4);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMaximumSize(QSize(16777215, 100));
        verticalLayout_5 = new QVBoxLayout(widget_2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        checkBox = new QCheckBox(widget_2);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout_5->addWidget(checkBox);

        cbPCMonitor = new QCheckBox(widget_2);
        cbPCMonitor->setObjectName(QString::fromUtf8("cbPCMonitor"));

        verticalLayout_5->addWidget(cbPCMonitor);


        verticalLayout_2->addWidget(widget_2);

        tabWidget->addTab(tab_4, QString());
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        tabWidget->addTab(widget, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\346\226\207\346\234\254\346\216\245\346\224\266", nullptr));
        btnOpenFloader->setText(QApplication::translate("MainWindow", "  \346\211\223\345\274\200\346\226\207\344\273\266\345\244\271", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\346\226\207\344\273\266\346\216\245\346\224\266", nullptr));
        btnSetWifi->setText(QApplication::translate("MainWindow", "wifi\350\256\276\347\275\256", nullptr));
        btnSetWeather->setText(QApplication::translate("MainWindow", "  \345\244\251\346\260\224\346\234\215\345\212\241\350\256\276\347\275\256", nullptr));
        btnSetMqtt->setText(QApplication::translate("MainWindow", "\344\272\221\346\234\215\345\212\241\350\256\276\347\275\256", nullptr));
        btnSetBilibili->setText(QApplication::translate("MainWindow", "   bilibili\346\216\245\345\217\243\350\256\276\347\275\256", nullptr));
        btnSleepTime->setText(QApplication::translate("MainWindow", " \347\235\241\347\234\240\346\227\266\351\225\277(\347\247\222)", nullptr));
        btnSetBleName->setText(QApplication::translate("MainWindow", " \350\223\235\347\211\231\345\220\215\347\247\260", nullptr));
        checkBox->setText(QApplication::translate("MainWindow", "\347\235\241\347\234\240\345\212\237\350\203\275", nullptr));
        cbPCMonitor->setText(QApplication::translate("MainWindow", "\347\224\265\350\204\221\347\233\221\346\216\247", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(widget), QApplication::translate("MainWindow", "\344\277\241\346\201\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
