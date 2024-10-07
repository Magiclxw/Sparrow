#ifndef DIALOG_SLEEP_TIME_H
#define DIALOG_SLEEP_TIME_H

#include <QDialog>

namespace Ui {
class Dialog_Sleep_Time;
}

class Dialog_Sleep_Time : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Sleep_Time(QWidget *parent = nullptr);
    ~Dialog_Sleep_Time();

private slots:
    void on_btnSaveSleepTime_clicked();

private:
    Ui::Dialog_Sleep_Time *ui;
};

#endif // DIALOG_SLEEP_TIME_H
