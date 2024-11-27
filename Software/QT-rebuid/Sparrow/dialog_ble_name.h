#ifndef DIALOG_BLE_NAME_H
#define DIALOG_BLE_NAME_H

#include <QDialog>

namespace Ui {
class Dialog_Ble_Name;
}

class Dialog_Ble_Name : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Ble_Name(QWidget *parent = nullptr);
    ~Dialog_Ble_Name();

private slots:
    void on_btnSaveBleName_clicked();

private:
    Ui::Dialog_Ble_Name *ui;
};

#endif // DIALOG_BLE_NAME_H
