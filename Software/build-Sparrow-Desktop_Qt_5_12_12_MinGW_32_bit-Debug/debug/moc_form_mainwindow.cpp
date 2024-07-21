/****************************************************************************
** Meta object code from reading C++ file 'form_mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Windows/form_mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'form_mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Form_MainWindow_t {
    QByteArrayData data[99];
    char stringdata0[2105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Form_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Form_MainWindow_t qt_meta_stringdata_Form_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "Form_MainWindow"
QT_MOC_LITERAL(1, 16, 16), // "Signal_AddFinger"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "uint8_t"
QT_MOC_LITERAL(4, 42, 3), // "pos"
QT_MOC_LITERAL(5, 46, 5), // "times"
QT_MOC_LITERAL(6, 52, 6), // "param1"
QT_MOC_LITERAL(7, 59, 6), // "param2"
QT_MOC_LITERAL(8, 66, 19), // "Signal_DeleteFinger"
QT_MOC_LITERAL(9, 86, 2), // "id"
QT_MOC_LITERAL(10, 89, 20), // "Signal_RefreshFinger"
QT_MOC_LITERAL(11, 110, 23), // "Signal_UpdateIndexTable"
QT_MOC_LITERAL(12, 134, 25), // "Signal_SetWindowsPassword"
QT_MOC_LITERAL(13, 160, 8), // "password"
QT_MOC_LITERAL(14, 169, 10), // "fingertype"
QT_MOC_LITERAL(15, 180, 5), // "index"
QT_MOC_LITERAL(16, 186, 18), // "Signal_SetPassword"
QT_MOC_LITERAL(17, 205, 26), // "Signal_SetAccount_Password"
QT_MOC_LITERAL(18, 232, 7), // "account"
QT_MOC_LITERAL(19, 240, 19), // "Signal_SetBreathRGB"
QT_MOC_LITERAL(20, 260, 7), // "color_R"
QT_MOC_LITERAL(21, 268, 7), // "color_G"
QT_MOC_LITERAL(22, 276, 7), // "color_B"
QT_MOC_LITERAL(23, 284, 8), // "interval"
QT_MOC_LITERAL(24, 293, 18), // "Signal_SetShortcut"
QT_MOC_LITERAL(25, 312, 4), // "func"
QT_MOC_LITERAL(26, 317, 5), // "char*"
QT_MOC_LITERAL(27, 323, 3), // "key"
QT_MOC_LITERAL(28, 327, 7), // "key_len"
QT_MOC_LITERAL(29, 335, 34), // "Signal_UpdateHideWindowChecke..."
QT_MOC_LITERAL(30, 370, 3), // "dir"
QT_MOC_LITERAL(31, 374, 20), // "Signal_SetActionFunc"
QT_MOC_LITERAL(32, 395, 6), // "action"
QT_MOC_LITERAL(33, 402, 19), // "Signal_SetFingerRGB"
QT_MOC_LITERAL(34, 422, 4), // "mode"
QT_MOC_LITERAL(35, 427, 10), // "startColor"
QT_MOC_LITERAL(36, 438, 9), // "stopColor"
QT_MOC_LITERAL(37, 448, 25), // "Signal_OpenHidewindowItem"
QT_MOC_LITERAL(38, 474, 21), // "Signal_GetFirmwareMsg"
QT_MOC_LITERAL(39, 496, 21), // "Signal_GetHardwareMsg"
QT_MOC_LITERAL(40, 518, 14), // "Slot_AddFinger"
QT_MOC_LITERAL(41, 533, 16), // "Slot_EnrollState"
QT_MOC_LITERAL(42, 550, 6), // "state1"
QT_MOC_LITERAL(43, 557, 6), // "state2"
QT_MOC_LITERAL(44, 564, 17), // "Slot_DeleteFinger"
QT_MOC_LITERAL(45, 582, 18), // "Slot_RefreshFinger"
QT_MOC_LITERAL(46, 601, 21), // "Slot_UpdateIndexTable"
QT_MOC_LITERAL(47, 623, 23), // "Slot_SetWindowsPassword"
QT_MOC_LITERAL(48, 647, 16), // "Slot_SetPassword"
QT_MOC_LITERAL(49, 664, 24), // "Slot_SetAccount_Password"
QT_MOC_LITERAL(50, 689, 17), // "Slot_SetBreathRGB"
QT_MOC_LITERAL(51, 707, 16), // "Slot_RGB_Display"
QT_MOC_LITERAL(52, 724, 20), // "Slot_ChangeItemValue"
QT_MOC_LITERAL(53, 745, 18), // "Slot_SaveItemValue"
QT_MOC_LITERAL(54, 764, 16), // "Slot_SetShortcut"
QT_MOC_LITERAL(55, 781, 25), // "Slot_DeleteListWidgetItem"
QT_MOC_LITERAL(56, 807, 18), // "Slot_Chat_Send_Msg"
QT_MOC_LITERAL(57, 826, 17), // "Slot_SetFingerRGB"
QT_MOC_LITERAL(58, 844, 28), // "Slot_Popup_ChatSettingWindow"
QT_MOC_LITERAL(59, 873, 23), // "Slot_Update_FirmwareMsg"
QT_MOC_LITERAL(60, 897, 4), // "date"
QT_MOC_LITERAL(61, 902, 7), // "version"
QT_MOC_LITERAL(62, 910, 23), // "Slot_Update_HardwareMsg"
QT_MOC_LITERAL(63, 934, 8), // "uint8_t*"
QT_MOC_LITERAL(64, 943, 7), // "flashId"
QT_MOC_LITERAL(65, 951, 10), // "ch9329_ver"
QT_MOC_LITERAL(66, 962, 9), // "fpm383cSN"
QT_MOC_LITERAL(67, 972, 14), // "SlotNotifyText"
QT_MOC_LITERAL(68, 987, 9), // "uint8_t[]"
QT_MOC_LITERAL(69, 997, 4), // "text"
QT_MOC_LITERAL(70, 1002, 3), // "len"
QT_MOC_LITERAL(71, 1006, 15), // "Slot_ShowWindow"
QT_MOC_LITERAL(72, 1022, 9), // "Slot_Exit"
QT_MOC_LITERAL(73, 1032, 47), // "on_listWidget_task_1_customCo..."
QT_MOC_LITERAL(74, 1080, 52), // "on_listWidget_table_state_cus..."
QT_MOC_LITERAL(75, 1133, 47), // "on_listWidget_task_2_customCo..."
QT_MOC_LITERAL(76, 1181, 47), // "on_listWidget_task_3_customCo..."
QT_MOC_LITERAL(77, 1229, 47), // "on_listWidget_task_4_customCo..."
QT_MOC_LITERAL(78, 1277, 47), // "on_listWidget_task_5_customCo..."
QT_MOC_LITERAL(79, 1325, 56), // "on_listWidget_table_state_key..."
QT_MOC_LITERAL(80, 1382, 47), // "on_listWidget_task_6_customCo..."
QT_MOC_LITERAL(81, 1430, 50), // "on_listWidget_hidwindow_custo..."
QT_MOC_LITERAL(82, 1481, 38), // "on_pushButton_chat_copy_answe..."
QT_MOC_LITERAL(83, 1520, 39), // "on_pushButton_action_play_pau..."
QT_MOC_LITERAL(84, 1560, 39), // "on_pushButton_action_calculat..."
QT_MOC_LITERAL(85, 1600, 33), // "on_pushButton_action_chat_cli..."
QT_MOC_LITERAL(86, 1634, 37), // "on_pushButton_action_computer..."
QT_MOC_LITERAL(87, 1672, 34), // "on_pushButton_action_email_cl..."
QT_MOC_LITERAL(88, 1707, 37), // "on_pushButton_action_explorer..."
QT_MOC_LITERAL(89, 1745, 33), // "on_pushButton_action_mute_cli..."
QT_MOC_LITERAL(90, 1779, 34), // "on_pushButton_action_power_cl..."
QT_MOC_LITERAL(91, 1814, 40), // "on_pushButton_action_screen_s..."
QT_MOC_LITERAL(92, 1855, 34), // "on_pushButton_action_sleep_cl..."
QT_MOC_LITERAL(93, 1890, 35), // "on_pushButton_action_weakup_c..."
QT_MOC_LITERAL(94, 1926, 35), // "on_pushButton_RGB_next_page_c..."
QT_MOC_LITERAL(95, 1962, 35), // "on_pushButton_RGB_last_page_c..."
QT_MOC_LITERAL(96, 1998, 38), // "on_pushButton_get_hardware_ms..."
QT_MOC_LITERAL(97, 2037, 38), // "on_pushButton_get_firmware_ms..."
QT_MOC_LITERAL(98, 2076, 28) // "on_pushButton_update_clicked"

    },
    "Form_MainWindow\0Signal_AddFinger\0\0"
    "uint8_t\0pos\0times\0param1\0param2\0"
    "Signal_DeleteFinger\0id\0Signal_RefreshFinger\0"
    "Signal_UpdateIndexTable\0"
    "Signal_SetWindowsPassword\0password\0"
    "fingertype\0index\0Signal_SetPassword\0"
    "Signal_SetAccount_Password\0account\0"
    "Signal_SetBreathRGB\0color_R\0color_G\0"
    "color_B\0interval\0Signal_SetShortcut\0"
    "func\0char*\0key\0key_len\0"
    "Signal_UpdateHideWindowCheckedItem\0"
    "dir\0Signal_SetActionFunc\0action\0"
    "Signal_SetFingerRGB\0mode\0startColor\0"
    "stopColor\0Signal_OpenHidewindowItem\0"
    "Signal_GetFirmwareMsg\0Signal_GetHardwareMsg\0"
    "Slot_AddFinger\0Slot_EnrollState\0state1\0"
    "state2\0Slot_DeleteFinger\0Slot_RefreshFinger\0"
    "Slot_UpdateIndexTable\0Slot_SetWindowsPassword\0"
    "Slot_SetPassword\0Slot_SetAccount_Password\0"
    "Slot_SetBreathRGB\0Slot_RGB_Display\0"
    "Slot_ChangeItemValue\0Slot_SaveItemValue\0"
    "Slot_SetShortcut\0Slot_DeleteListWidgetItem\0"
    "Slot_Chat_Send_Msg\0Slot_SetFingerRGB\0"
    "Slot_Popup_ChatSettingWindow\0"
    "Slot_Update_FirmwareMsg\0date\0version\0"
    "Slot_Update_HardwareMsg\0uint8_t*\0"
    "flashId\0ch9329_ver\0fpm383cSN\0"
    "SlotNotifyText\0uint8_t[]\0text\0len\0"
    "Slot_ShowWindow\0Slot_Exit\0"
    "on_listWidget_task_1_customContextMenuRequested\0"
    "on_listWidget_table_state_customContextMenuRequested\0"
    "on_listWidget_task_2_customContextMenuRequested\0"
    "on_listWidget_task_3_customContextMenuRequested\0"
    "on_listWidget_task_4_customContextMenuRequested\0"
    "on_listWidget_task_5_customContextMenuRequested\0"
    "on_listWidget_table_state_key_customContextMenuRequested\0"
    "on_listWidget_task_6_customContextMenuRequested\0"
    "on_listWidget_hidwindow_customContextMenuRequested\0"
    "on_pushButton_chat_copy_answer_clicked\0"
    "on_pushButton_action_play_pause_clicked\0"
    "on_pushButton_action_calculator_clicked\0"
    "on_pushButton_action_chat_clicked\0"
    "on_pushButton_action_computer_clicked\0"
    "on_pushButton_action_email_clicked\0"
    "on_pushButton_action_explorer_clicked\0"
    "on_pushButton_action_mute_clicked\0"
    "on_pushButton_action_power_clicked\0"
    "on_pushButton_action_screen_save_clicked\0"
    "on_pushButton_action_sleep_clicked\0"
    "on_pushButton_action_weakup_clicked\0"
    "on_pushButton_RGB_next_page_clicked\0"
    "on_pushButton_RGB_last_page_clicked\0"
    "on_pushButton_get_hardware_msg_clicked\0"
    "on_pushButton_get_firmware_msg_clicked\0"
    "on_pushButton_update_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Form_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      63,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,  329,    2, 0x06 /* Public */,
       8,    1,  338,    2, 0x06 /* Public */,
      10,    0,  341,    2, 0x06 /* Public */,
      11,    0,  342,    2, 0x06 /* Public */,
      12,    3,  343,    2, 0x06 /* Public */,
      16,    3,  350,    2, 0x06 /* Public */,
      17,    4,  357,    2, 0x06 /* Public */,
      19,    4,  366,    2, 0x06 /* Public */,
      24,    5,  375,    2, 0x06 /* Public */,
      29,    1,  386,    2, 0x06 /* Public */,
      31,    2,  389,    2, 0x06 /* Public */,
      33,    4,  394,    2, 0x06 /* Public */,
      37,    0,  403,    2, 0x06 /* Public */,
      38,    0,  404,    2, 0x06 /* Public */,
      39,    0,  405,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      40,    0,  406,    2, 0x0a /* Public */,
      41,    2,  407,    2, 0x0a /* Public */,
      44,    0,  412,    2, 0x0a /* Public */,
      45,    0,  413,    2, 0x0a /* Public */,
      46,    0,  414,    2, 0x0a /* Public */,
      47,    0,  415,    2, 0x0a /* Public */,
      48,    0,  416,    2, 0x0a /* Public */,
      49,    0,  417,    2, 0x0a /* Public */,
      50,    0,  418,    2, 0x0a /* Public */,
      51,    0,  419,    2, 0x0a /* Public */,
      52,    0,  420,    2, 0x0a /* Public */,
      53,    0,  421,    2, 0x0a /* Public */,
      54,    0,  422,    2, 0x0a /* Public */,
      55,    0,  423,    2, 0x0a /* Public */,
      56,    0,  424,    2, 0x0a /* Public */,
      57,    0,  425,    2, 0x0a /* Public */,
      58,    0,  426,    2, 0x0a /* Public */,
      59,    2,  427,    2, 0x0a /* Public */,
      62,    3,  432,    2, 0x0a /* Public */,
      67,    2,  439,    2, 0x0a /* Public */,
      71,    0,  444,    2, 0x08 /* Private */,
      72,    0,  445,    2, 0x08 /* Private */,
      73,    1,  446,    2, 0x08 /* Private */,
      74,    1,  449,    2, 0x08 /* Private */,
      75,    1,  452,    2, 0x08 /* Private */,
      76,    1,  455,    2, 0x08 /* Private */,
      77,    1,  458,    2, 0x08 /* Private */,
      78,    1,  461,    2, 0x08 /* Private */,
      79,    1,  464,    2, 0x08 /* Private */,
      80,    1,  467,    2, 0x08 /* Private */,
      81,    1,  470,    2, 0x08 /* Private */,
      82,    0,  473,    2, 0x08 /* Private */,
      83,    0,  474,    2, 0x08 /* Private */,
      84,    0,  475,    2, 0x08 /* Private */,
      85,    0,  476,    2, 0x08 /* Private */,
      86,    0,  477,    2, 0x08 /* Private */,
      87,    0,  478,    2, 0x08 /* Private */,
      88,    0,  479,    2, 0x08 /* Private */,
      89,    0,  480,    2, 0x08 /* Private */,
      90,    0,  481,    2, 0x08 /* Private */,
      91,    0,  482,    2, 0x08 /* Private */,
      92,    0,  483,    2, 0x08 /* Private */,
      93,    0,  484,    2, 0x08 /* Private */,
      94,    0,  485,    2, 0x08 /* Private */,
      95,    0,  486,    2, 0x08 /* Private */,
      96,    0,  487,    2, 0x08 /* Private */,
      97,    0,  488,    2, 0x08 /* Private */,
      98,    0,  489,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3,    4,    5,    6,    7,
    QMetaType::Void, 0x80000000 | 3,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3, 0x80000000 | 3,   13,   14,   15,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3, 0x80000000 | 3,   13,   14,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 3, 0x80000000 | 3,   18,   13,   14,   15,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3,   20,   21,   22,   23,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 26, 0x80000000 | 3, 0x80000000 | 3,   14,   25,   27,   28,   15,
    QMetaType::Void, QMetaType::Int,   30,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,   25,   32,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3,   34,   35,   36,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,   42,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 26, 0x80000000 | 26,   60,   61,
    QMetaType::Void, 0x80000000 | 63, QMetaType::Char, 0x80000000 | 26,   64,   65,   66,
    QMetaType::Void, 0x80000000 | 68, QMetaType::Int,   69,   70,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void, QMetaType::QPoint,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Form_MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Form_MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Signal_AddFinger((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3])),(*reinterpret_cast< uint8_t(*)>(_a[4]))); break;
        case 1: _t->Signal_DeleteFinger((*reinterpret_cast< uint8_t(*)>(_a[1]))); break;
        case 2: _t->Signal_RefreshFinger(); break;
        case 3: _t->Signal_UpdateIndexTable(); break;
        case 4: _t->Signal_SetWindowsPassword((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3]))); break;
        case 5: _t->Signal_SetPassword((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3]))); break;
        case 6: _t->Signal_SetAccount_Password((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3])),(*reinterpret_cast< uint8_t(*)>(_a[4]))); break;
        case 7: _t->Signal_SetBreathRGB((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3])),(*reinterpret_cast< uint8_t(*)>(_a[4]))); break;
        case 8: _t->Signal_SetShortcut((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< char*(*)>(_a[3])),(*reinterpret_cast< uint8_t(*)>(_a[4])),(*reinterpret_cast< uint8_t(*)>(_a[5]))); break;
        case 9: _t->Signal_UpdateHideWindowCheckedItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->Signal_SetActionFunc((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2]))); break;
        case 11: _t->Signal_SetFingerRGB((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3])),(*reinterpret_cast< uint8_t(*)>(_a[4]))); break;
        case 12: _t->Signal_OpenHidewindowItem(); break;
        case 13: _t->Signal_GetFirmwareMsg(); break;
        case 14: _t->Signal_GetHardwareMsg(); break;
        case 15: _t->Slot_AddFinger(); break;
        case 16: _t->Slot_EnrollState((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2]))); break;
        case 17: _t->Slot_DeleteFinger(); break;
        case 18: _t->Slot_RefreshFinger(); break;
        case 19: _t->Slot_UpdateIndexTable(); break;
        case 20: _t->Slot_SetWindowsPassword(); break;
        case 21: _t->Slot_SetPassword(); break;
        case 22: _t->Slot_SetAccount_Password(); break;
        case 23: _t->Slot_SetBreathRGB(); break;
        case 24: _t->Slot_RGB_Display(); break;
        case 25: _t->Slot_ChangeItemValue(); break;
        case 26: _t->Slot_SaveItemValue(); break;
        case 27: _t->Slot_SetShortcut(); break;
        case 28: _t->Slot_DeleteListWidgetItem(); break;
        case 29: _t->Slot_Chat_Send_Msg(); break;
        case 30: _t->Slot_SetFingerRGB(); break;
        case 31: _t->Slot_Popup_ChatSettingWindow(); break;
        case 32: _t->Slot_Update_FirmwareMsg((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< char*(*)>(_a[2]))); break;
        case 33: _t->Slot_Update_HardwareMsg((*reinterpret_cast< uint8_t*(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2])),(*reinterpret_cast< char*(*)>(_a[3]))); break;
        case 34: _t->SlotNotifyText((*reinterpret_cast< uint8_t(*)[]>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 35: _t->Slot_ShowWindow(); break;
        case 36: _t->Slot_Exit(); break;
        case 37: _t->on_listWidget_task_1_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 38: _t->on_listWidget_table_state_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 39: _t->on_listWidget_task_2_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 40: _t->on_listWidget_task_3_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 41: _t->on_listWidget_task_4_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 42: _t->on_listWidget_task_5_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 43: _t->on_listWidget_table_state_key_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 44: _t->on_listWidget_task_6_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 45: _t->on_listWidget_hidwindow_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 46: _t->on_pushButton_chat_copy_answer_clicked(); break;
        case 47: _t->on_pushButton_action_play_pause_clicked(); break;
        case 48: _t->on_pushButton_action_calculator_clicked(); break;
        case 49: _t->on_pushButton_action_chat_clicked(); break;
        case 50: _t->on_pushButton_action_computer_clicked(); break;
        case 51: _t->on_pushButton_action_email_clicked(); break;
        case 52: _t->on_pushButton_action_explorer_clicked(); break;
        case 53: _t->on_pushButton_action_mute_clicked(); break;
        case 54: _t->on_pushButton_action_power_clicked(); break;
        case 55: _t->on_pushButton_action_screen_save_clicked(); break;
        case 56: _t->on_pushButton_action_sleep_clicked(); break;
        case 57: _t->on_pushButton_action_weakup_clicked(); break;
        case 58: _t->on_pushButton_RGB_next_page_clicked(); break;
        case 59: _t->on_pushButton_RGB_last_page_clicked(); break;
        case 60: _t->on_pushButton_get_hardware_msg_clicked(); break;
        case 61: _t->on_pushButton_get_firmware_msg_clicked(); break;
        case 62: _t->on_pushButton_update_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Form_MainWindow::*)(uint8_t , uint8_t , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_AddFinger)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_DeleteFinger)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_RefreshFinger)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_UpdateIndexTable)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(QString , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_SetWindowsPassword)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(QString , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_SetPassword)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(QString , QString , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_SetAccount_Password)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(uint8_t , uint8_t , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_SetBreathRGB)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(uint8_t , uint8_t , char * , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_SetShortcut)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_UpdateHideWindowCheckedItem)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_SetActionFunc)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)(uint8_t , uint8_t , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_SetFingerRGB)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_OpenHidewindowItem)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_GetFirmwareMsg)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (Form_MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Form_MainWindow::Signal_GetHardwareMsg)) {
                *result = 14;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Form_MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_Form_MainWindow.data,
    qt_meta_data_Form_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Form_MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Form_MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Form_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Form_MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 63)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 63;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 63)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 63;
    }
    return _id;
}

// SIGNAL 0
void Form_MainWindow::Signal_AddFinger(uint8_t _t1, uint8_t _t2, uint8_t _t3, uint8_t _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Form_MainWindow::Signal_DeleteFinger(uint8_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Form_MainWindow::Signal_RefreshFinger()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Form_MainWindow::Signal_UpdateIndexTable()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Form_MainWindow::Signal_SetWindowsPassword(QString _t1, uint8_t _t2, uint8_t _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Form_MainWindow::Signal_SetPassword(QString _t1, uint8_t _t2, uint8_t _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Form_MainWindow::Signal_SetAccount_Password(QString _t1, QString _t2, uint8_t _t3, uint8_t _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Form_MainWindow::Signal_SetBreathRGB(uint8_t _t1, uint8_t _t2, uint8_t _t3, uint8_t _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Form_MainWindow::Signal_SetShortcut(uint8_t _t1, uint8_t _t2, char * _t3, uint8_t _t4, uint8_t _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Form_MainWindow::Signal_UpdateHideWindowCheckedItem(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Form_MainWindow::Signal_SetActionFunc(uint8_t _t1, uint8_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Form_MainWindow::Signal_SetFingerRGB(uint8_t _t1, uint8_t _t2, uint8_t _t3, uint8_t _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Form_MainWindow::Signal_OpenHidewindowItem()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void Form_MainWindow::Signal_GetFirmwareMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void Form_MainWindow::Signal_GetHardwareMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
