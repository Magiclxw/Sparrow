#ifndef DIALOG_MQTT_CFG_H
#define DIALOG_MQTT_CFG_H

#include <QDialog>

namespace Ui {
class Dialog_Mqtt_cfg;
}

class Dialog_Mqtt_cfg : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Mqtt_cfg(QWidget *parent = nullptr);
    ~Dialog_Mqtt_cfg();

private slots:
    void on_btnMqttSaveAddr_clicked();

    void on_btnMqttSaveUsername_clicked();

    void on_btnMqttSavePassword_clicked();

private:
    Ui::Dialog_Mqtt_cfg *ui;
};

#endif // DIALOG_MQTT_CFG_H
