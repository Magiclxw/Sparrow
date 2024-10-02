#ifndef DIALOG_BILIBILI_CFG_H
#define DIALOG_BILIBILI_CFG_H

#include <QDialog>

namespace Ui {
class Dialog_Bilibili_Cfg;
}

class Dialog_Bilibili_Cfg : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Bilibili_Cfg(QWidget *parent = nullptr);
    ~Dialog_Bilibili_Cfg();

private slots:
    void on_btnSetBilibili_clicked();

private:
    Ui::Dialog_Bilibili_Cfg *ui;
};

#endif // DIALOG_BILIBILI_CFG_H
