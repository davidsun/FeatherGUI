QT += network \
    xml
SOURCES += Message/MsgDestination.cpp \
    Message/Msg.cpp \
    Message/InstantSMS.cpp \
    Message/ScheduledSMS.cpp \
    Account/FeatherAccount.cpp \
    Account/AccountManager.cpp \
    Forms/DialogCertification.cpp \
    Message/MsgManager.cpp \
    Message/AbstractMsgList.cpp \
    Message/MsgItem.cpp \
    Message/InstantSMSList.cpp \
    Message/ScheduledSMSList.cpp \
    Account/AccountList_TagItem_AccountItem.cpp \
    Account/AccountList_TagItem.cpp \
    Account/AccountList_TagItem_AccountItem_GroupItem.cpp \
    Account/AccountList.cpp \
    Forms/MainWindow.cpp \
    main.cpp \
    Account/AccountStatisticsList.cpp \
    Account/AccountStatisticsList_TagItem.cpp \
    Account/AccountStatisticsList_TagItem_AccountItem.cpp
HEADERS += Message/Msg.h \
    Message/MsgManager.h \
    Account/FeatherAccount.h \
    Account/AccountManager.h \
    Forms/DialogCertification.h \
    Message/MsgList.h \
    Account/AccountList.h \
    Forms/MainWindow.h \
    Account/AccountStatisticsList.h
FORMS = Forms/MainWindow.ui \
    Forms/DialogCertification.ui
INCLUDEPATH += include/ \
    Feather/include/ \
    .
unix { 
    LIBS += ./Feather/libFeather.a \
			-lssl
    CONFIG += debug
}
win32 { 
    LIBS += ./libcurldll.a \
        ./libeay32.lib \
        ./Feather/libFeather.a
    CONFIG += release
}
TARGET = FeatherGUI
TEMPLATE = app
