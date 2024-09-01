#ifndef DIALOG_WIFI_CFG_H
#define DIALOG_WIFI_CFG_H

#include <QDialog>

namespace Ui {
class Dialog_Wifi_CFG;
}

class Dialog_Wifi_CFG : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Wifi_CFG(QWidget *parent = nullptr);
    ~Dialog_Wifi_CFG();

private slots:
    void on_btnWifiSave_clicked();

private:
    Ui::Dialog_Wifi_CFG *ui;
};

#endif // DIALOG_WIFI_CFG_H
