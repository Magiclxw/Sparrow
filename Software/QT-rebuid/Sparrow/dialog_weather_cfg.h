#ifndef DIALOG_WEATHER_CFG_H
#define DIALOG_WEATHER_CFG_H

#include <QDialog>

namespace Ui {
class Dialog_Weather_Cfg;
}

class Dialog_Weather_Cfg : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Weather_Cfg(QWidget *parent = nullptr);
    ~Dialog_Weather_Cfg();

private slots:
    void on_btnSetWeather_clicked();

private:
    Ui::Dialog_Weather_Cfg *ui;
};

#endif // DIALOG_WEATHER_CFG_H
