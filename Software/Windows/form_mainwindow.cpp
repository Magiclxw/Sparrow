#include "form_mainwindow.h"
#include "ui_form_mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QTimer>
#include <QString>
#include <QSizePolicy>
#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QMetaEnum>
#include <QKeyEvent>
#include "lib/GlobalMouseKey/globalkeyevent.h"
#include "qcoreevent.h"
#include "form_hidewindow.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include "system/system_init.h"
#include <QFileInfo>
#include <QFileIconProvider>
#include <QLabel>
#include <QDesktopServices>
#include <QProcess>
#include <QTextCodec>
#include <QClipboard>
#include <QWindow>
#include "form_chatsetting.h"


QColor itemValid(0,255,255);       //指纹有效颜色
QColor itemUnValid(255,255,255);   //指纹无效颜色
QColor itemkeyValid(0,255,0);       //指纹+按键有效颜色
QColor itemkeyUnValid(255,255,255);   //指纹+按键无效颜色

QAction *Action_Add;                    //添加指纹
QAction *Action_Refresh;                //刷新列表
QAction *Action_Delete;                 //删除指纹
QAction *Action_unlock;                 //解锁功能
QAction *Action_shortcut;               //快捷键
QAction *Action_enterAccount_Password;  //输入账号
QAction *Action_enterPassword;          //输入密码
QAction *Action_setQuickStart1;                //快捷启动
QAction *Action_setQuickStart2;                //快捷启动
QAction *Action_setQuickStart3;                //快捷启动
QAction *Action_setQuickStart4;                //快捷启动
QAction *Action_setQuickStart5;                //快捷启动
QAction *Action_setQuickStart6;                //快捷启动
QAction *Action_listWidget_Delete_Item;        //删除索引表项

/* RGB三色控制 */
float color_R = 0;
float color_G = 0;
float color_B = 0;
uint8_t dir = 0;    //亮度增减方向
float tmp_R = 0;
float tmp_G = 0;
float tmp_B = 0;
float R_decrease = 0;
float G_decrease = 0;
float B_decrease = 0;
uint8_t interval = 0;   //亮度改变间隔

QString chat_rec_last_msg;  //记录接收到的对话信息

QString table_1_Content = "config/table1Content.json";
QString table_2_Content = "config/table2Content.json";

QTimer *rgb_timer = NULL;   //灯效演示定时器
QTimer *hidewindowHold_timer = NULL;    //侧边栏超时隐藏定时器

Form_HideWindow *hidewindow = NULL;

Form_MainWindow::Form_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Form_MainWindow)
{
    ui->setupUi(this);
    /* 隐藏保存参数按键 */
    ui->pushButton_save_param->hide();
    ui->pushButton_save_param_key->hide();

    this->setWindowTitle("Sparrow");
    this->setWindowIcon(QIcon(":/icon/Icon.png"));
    this->setFixedSize(this->width(),this->height());

    InitSysTray();  //放置托盘

    QFile file(":/qss/lightgray.css"); // 创建QFile对象，指定样式表文件路径
    file.open(QFile::ReadOnly); // 打开文件，只读模式
    QString styleSheet = QLatin1String(file.readAll()); // 读取文件内容到字符串
    setStyleSheet(styleSheet); // 应用样式表

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));   //设置编码格式

    ui->textEdit_chat_input->setPlaceholderText("此处输入对话内容");

    ui->lineEdit_email->setText(AUTHOR_EMAIL);
    ui->lineEdit_group->setText(AUTHOR_GROUP);
    ui->lineEdit_developer_group->setText(AUTHOR_DEVELOPER_GROUP);
    ui->label_software_version->setText(SOFTWARE_VERSION);
    ui->label_software_build_time->setText(__DATE__);

    hidewindow = new Form_HideWindow;
    hidewindow->show();
    hidewindow->File_Update_Hidewindow_Content();   //更新侧边栏内容

    QMenuBar *bar = menuBar();
    this->setMenuBar(bar);
    QMenu *Menu = bar->addMenu("MENU");
    QMenu *ListWidgetMenu = bar->addMenu("MENU");
    bar->setVisible(false);

    Update_QuickStart_Content();   //更新快捷启动列表
    Update_HideWindow_List();      //更新侧边栏启动列表

    rgb_timer = new QTimer; //初始化rgb显示定时器
    hidewindowHold_timer = new QTimer; //初始化侧边栏隐藏定时器

    Action_Add = Menu->addAction("增加");
    Action_Delete = Menu->addAction("删除");
    Action_unlock = Menu->addAction("解锁");
    Action_Refresh = Menu->addAction("刷新");
    Action_shortcut = Menu->addAction("快捷键");
    Action_enterPassword = Menu->addAction("输入密码");
    Action_enterAccount_Password = Menu->addAction("输入账号/密码");
    Action_setQuickStart1 = Menu->addAction("快捷启动流程1");
    Action_setQuickStart2 = Menu->addAction("快捷启动流程2");
    Action_setQuickStart3 = Menu->addAction("快捷启动流程3");
    Action_setQuickStart4 = Menu->addAction("快捷启动流程4");
    Action_setQuickStart5 = Menu->addAction("快捷启动流程5");
    Action_setQuickStart6 = Menu->addAction("快捷启动流程6");
    Action_listWidget_Delete_Item = ListWidgetMenu->addAction("删除");
    /* 注册按键监听事件 */
    GlobalKeyEvent::installKeyEvent();
    connect(GlobalKeyEvent::getInstance(),&GlobalKeyEvent::keyEvent,this,&Form_MainWindow::on_keyEvent);

    File_Update_TableContent(table_1_Content);  //更新索引表1(指纹)
    File_Update_TableContent(table_2_Content);  //更新索引表2(指纹+按键)

    ui->listWidget_table_state->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_table_state_key->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_task_1->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_task_2->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_task_3->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_task_4->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_task_5->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_task_6->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_hidwindow->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setContextMenuPolicy(Qt::NoContextMenu);

}

void Form_MainWindow::Slot_Update_FirmwareMsg(char* date,char* version)
{
    qDebug() << "date" << date;
    qDebug() << "version" << version;
    ui->label_build_date->setText(date);
    ui->label_firmware_version->setText(version);
}

/**
*@brief	更新硬件信息
*@param	-flashId:flash id号
*@param -ch9329_ver:ch9329版本号
*@param -fpm383cSN:fpm383c SN号
*@return NULL
*/
void Form_MainWindow::Slot_Update_HardwareMsg(uint8_t* flashId,char ch9329_ver,char* fpm383cSN)
{
    QString t_fpm383cSN = QString::fromUtf8(fpm383cSN);
    uint16_t t_flashId = flashId[0] | flashId[1]<<8;
    char t_ch9329_ver[2] = {ch9329_ver,0};
    ui->label_flashId->setText(QString::number(t_flashId,16).toUpper());
    ui->label_ch9329_version->setText("V1." + QString::fromUtf8(t_ch9329_ver));
    ui->label_fpm383_sn->setText(t_fpm383cSN);
}


//更新快捷启动列表
void Form_MainWindow::Update_QuickStart_Content()
{

    for (uint8_t i=0; i<6; i++ )
    {
        uint8_t item = 0;
        uint8_t itemNum = 0;
        switch (i) {
        case 0:
        {
            ui->listWidget_task_1->clear();
            break;
        }
        case 1:
        {
            ui->listWidget_task_2->clear();
            break;
        }
        case 2:
        {
            ui->listWidget_task_3->clear();
            break;
        }
        case 3:
        {
            ui->listWidget_task_4->clear();
            break;
        }
        case 4:
        {
            ui->listWidget_task_5->clear();
            break;
        }
        case 5:
        {
            ui->listWidget_task_6->clear();
            break;
        }
        }
        itemNum = File_Fast_Start_Num_Get(i,item);
        while(1)
        {

            if(item == itemNum)
            {
                break;
            }
            QString path = File_FastStart_Read(i,item);
            {
                QListWidgetItem *listwidgetitem = new QListWidgetItem;
                QFileInfo fileInfo(path);
                QFileIconProvider iconProvider;
                QString fileName = fileInfo.fileName();
                QIcon icon = iconProvider.icon(fileInfo);
                QLabel label;
                label.setPixmap(icon.pixmap(50,50));
                listwidgetitem->setIcon(icon);
                listwidgetitem->setText(fileName);
                switch (i)
                {
                case 0:
                {
                    ui->listWidget_task_1->addItem(listwidgetitem);
                    break;
                }
                case 1:
                {
                    ui->listWidget_task_2->addItem(listwidgetitem);
                    break;
                }
                case 2:
                {
                    ui->listWidget_task_3->addItem(listwidgetitem);
                    break;
                }
                case 3:
                {
                    ui->listWidget_task_4->addItem(listwidgetitem);
                    break;
                }
                case 4:
                {
                    ui->listWidget_task_5->addItem(listwidgetitem);
                    break;
                }
                case 5:
                {
                    ui->listWidget_task_6->addItem(listwidgetitem);
                    break;
                }
                }


//                if(checkState == true)
//                {
//                    item->setCheckState(Qt::Checked);
//                }
            }
            item++;
        }
    }
}

void Form_MainWindow::Update_HideWindow_List()
{
    uint8_t item = 0;
    uint8_t itemNum = 0;
    itemNum = File_HideWindow_ItemNum_Get();
    ui->listWidget_hidwindow->clear();
    while(1)
    {

        if(item == itemNum)
        {
            break;
        }
        QString path = File_HideWindow_Item_Read(item);
        {
            QListWidgetItem *listwidgetitem = new QListWidgetItem;
            QFileInfo fileInfo(path);
            QFileIconProvider iconProvider;
            QString fileName = fileInfo.fileName();
            QIcon icon = iconProvider.icon(fileInfo);
            QLabel label;
            label.setPixmap(icon.pixmap(50,50));
            listwidgetitem->setIcon(icon);
            listwidgetitem->setText(fileName);

            ui->listWidget_hidwindow->addItem(listwidgetitem);
        }
        item++;
    }
}

void Form_MainWindow::dropEvent(QDropEvent *event)           // 放下事件
{
    uint8_t page_index = 0; //文件拖入页索引
    const QMimeData *mimeData = event->mimeData();      // 获取MIME数据
    if(mimeData->hasUrls()){                            // 如果数据中包含URL
        QList<QUrl> urlList = mimeData->urls();         // 获取URL列表
        // 将其中第一个URL表示为本地文件路径
        QString fileName = urlList.at(0).toLocalFile();
        qDebug() << "url:" << fileName;
        if(ui->tabWidget->currentWidget() == ui->tab_quickstart)
        {
            uint8_t page = ui->toolBox->currentIndex();
            uint8_t index = 0;
            switch (page)
            {
            case 0:
            {
                index = ui->listWidget_task_1->count();
                break;
            }
            case 1:
            {
                index = ui->listWidget_task_2->count();
                break;
            }
            case 2:
            {
                index = ui->listWidget_task_3->count();
                break;
            }
            case 3:
            {
                index = ui->listWidget_task_4->count();
                break;
            }
            case 4:
            {
                index = ui->listWidget_task_5->count();
                break;
            }
            case 5:
            {
                index = ui->listWidget_task_6->count();
                break;
            }
            default:break;
            }
            qDebug() << "page =" << page;
            File_FastStart_Save(page,index,0,fileName);
            Update_QuickStart_Content();
        }
        if(ui->tabWidget->currentWidget() == ui->tab_hidwindow)
        {
            uint8_t index = ui->listWidget_hidwindow->count();
            File_HideWindow_Content_Save(fileName,index);
            Update_HideWindow_List();
            hidewindow->File_Update_Hidewindow_Content();
        }
    }
    page_index = ui->toolBox->currentIndex();

}

void Form_MainWindow::dragEnterEvent(QDragEnterEvent *event) // 拖动进入事件
{
    if(event->mimeData()->hasUrls())                    // 数据中是否包含URL
        event->acceptProposedAction();                  // 如果是则接收动作
    else event->ignore();                               // 否则忽略该事件
}

/* 监听窗口是否进入最小化状态 */
//void Form_MainWindow::changeEvent(QEvent *event)
//{
//    if(event->type()!=QEvent::WindowStateChange) return;
//    if(this->windowState()==Qt::WindowMinimized)
//    {
//        qDebug() << "min";
//    }
//}

void Form_MainWindow::InitSysTray()
{
    m_sysTrayIcon = new QSystemTrayIcon(this);

    QIcon icon = QIcon(":/icon/Icon.png");

    m_sysTrayIcon->setIcon(icon);

    m_sysTrayIcon->setToolTip("Sparrow");

    connect(m_sysTrayIcon,&QSystemTrayIcon::activated,[=](QSystemTrayIcon::ActivationReason reason){
        if(reason == QSystemTrayIcon::DoubleClick)
        {
            this->show();
        }
    });

    createAction();
    createMenu();

    m_sysTrayIcon->show();
}

void Form_MainWindow::createAction()
{
    m_shoWindow = new QAction("显示主界面",this);
    connect(m_shoWindow,&QAction::triggered,this,&Form_MainWindow::Slot_ShowWindow);
    m_exit = new QAction("退出",this);
    connect(m_exit,&QAction::triggered,this,&Form_MainWindow::Slot_Exit);

}

void Form_MainWindow::createMenu()
{
    m_menu = new QMenu(this);
    //新增菜单项---显示主界面
    m_menu->addAction(m_shoWindow);
    //增加分隔符
    m_menu->addSeparator();
    //新增菜单项---退出程序
    m_menu->addAction(m_exit);
    //把QMenu赋给QSystemTrayIcon对象
    m_sysTrayIcon->setContextMenu(m_menu);
}

void Form_MainWindow::Slot_ShowWindow()
{
    this->show();
}

void Form_MainWindow::Slot_Exit()
{
    qApp->exit();
}

void Form_MainWindow::closeEvent(QCloseEvent *event)
{
    //忽略窗口关闭事件
        QApplication::setQuitOnLastWindowClosed( true );
        this->hide();
        event->ignore();
}

Form_MainWindow::~Form_MainWindow()
{
    delete ui;
}


void Form_MainWindow::on_listWidget_table_state_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);

    // 新建Menu菜单
    QMenu *ptr = new QMenu(this);
    QMenu *functionMenu = new QMenu("功能");
    //ptr->setFixedWidth(100);

    // 添加Actions创建菜单项
    ptr->addAction(Action_Add);
    ptr->addAction(Action_Refresh);
    ptr->addAction(Action_Delete);

    functionMenu->addAction(Action_unlock);
    functionMenu->addAction(Action_shortcut);
    functionMenu->addAction(Action_enterAccount_Password);
    functionMenu->addAction(Action_enterPassword);
    functionMenu->addAction(Action_setQuickStart1);
    functionMenu->addAction(Action_setQuickStart2);
    functionMenu->addAction(Action_setQuickStart3);
    functionMenu->addAction(Action_setQuickStart4);
    functionMenu->addAction(Action_setQuickStart5);
    functionMenu->addAction(Action_setQuickStart6);

    ptr->addMenu(functionMenu);
    // 在鼠标光标位置显示右键快捷菜单
    ptr->exec(QCursor::pos());
    // 手工创建的指针必须手工删除
    delete ptr;
}



void Form_MainWindow::on_listWidget_task_1_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *ptr = new QMenu(this);
    ptr->addAction(Action_listWidget_Delete_Item);
    ptr->exec(QCursor::pos());
    delete ptr;

}


void Form_MainWindow::on_listWidget_task_2_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *ptr = new QMenu(this);
    ptr->addAction(Action_listWidget_Delete_Item);
    ptr->exec(QCursor::pos());
    delete ptr;
}


void Form_MainWindow::on_listWidget_task_3_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *ptr = new QMenu(this);
    ptr->addAction(Action_listWidget_Delete_Item);
    ptr->exec(QCursor::pos());
    delete ptr;
}


void Form_MainWindow::on_listWidget_task_4_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *ptr = new QMenu(this);
    ptr->addAction(Action_listWidget_Delete_Item);
    ptr->exec(QCursor::pos());
    delete ptr;
}


void Form_MainWindow::on_listWidget_task_5_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *ptr = new QMenu(this);
    ptr->addAction(Action_listWidget_Delete_Item);
    ptr->exec(QCursor::pos());
    delete ptr;
}


void Form_MainWindow::on_listWidget_task_6_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *ptr = new QMenu(this);
    ptr->addAction(Action_listWidget_Delete_Item);
    ptr->exec(QCursor::pos());
    delete ptr;
}

void Form_MainWindow::on_listWidget_hidwindow_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *ptr = new QMenu(this);
    ptr->addAction(Action_listWidget_Delete_Item);
    ptr->exec(QCursor::pos());
    delete ptr;
}

void Form_MainWindow::on_listWidget_table_state_key_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);

    // 新建Menu菜单
    QMenu *ptr = new QMenu(this);
    QMenu *functionMenu = new QMenu("功能");
    //ptr->setFixedWidth(100);

    // 添加Actions创建菜单项
    functionMenu->addAction(Action_unlock);
    functionMenu->addAction(Action_shortcut);
    functionMenu->addAction(Action_enterAccount_Password);
    functionMenu->addAction(Action_enterPassword);
    functionMenu->addAction(Action_setQuickStart1);
    functionMenu->addAction(Action_setQuickStart2);
    functionMenu->addAction(Action_setQuickStart3);
    functionMenu->addAction(Action_setQuickStart4);
    functionMenu->addAction(Action_setQuickStart5);
    functionMenu->addAction(Action_setQuickStart6);

    ptr->addMenu(functionMenu);
    // 在鼠标光标位置显示右键快捷菜单
    ptr->exec(QCursor::pos());
    // 手工创建的指针必须手工删除
    delete ptr;
}


void Form_MainWindow::on_pushButton_chat_copy_answer_clicked()
{
    QClipboard *clip = QApplication::clipboard();
    if(chat_rec_last_msg != NULL)
    {
        clip->setText(chat_rec_last_msg);
    }


}


void Form_MainWindow::on_pushButton_RGB_next_page_clicked()
{
    ui->stackedWidget_RGB->setCurrentWidget(ui->page_finger_rgb);
}


void Form_MainWindow::on_pushButton_RGB_last_page_clicked()
{
    ui->stackedWidget_RGB->setCurrentWidget(ui->page_rgb);
}


void Form_MainWindow::on_pushButton_get_firmware_msg_clicked()
{
    emit Signal_GetFirmwareMsg();
}


void Form_MainWindow::on_pushButton_get_hardware_msg_clicked()
{
    emit Signal_GetHardwareMsg();
}


void Form_MainWindow::on_pushButton_update_clicked()
{
    QDesktopServices::openUrl(QUrl(QString(SOFTWARE_UPDATE_LINK)));
}

