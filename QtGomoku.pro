#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T16:07:02
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGomoku
TEMPLATE = app
CONFIG += C++11


SOURCES += infodialog.cpp \
    connectdialog.cpp \
    createdialog.cpp \
    gameboard.cpp \
    gomoku.cpp \
    main.cpp \
    saveloaddialog.cpp

HEADERS  +=  infodialog.h \
    connectdialog.h \
    createdialog.h \
    gameboard.h \
    gomoku.h \
    saveloaddialog.h

FORMS    += infodialog.ui \
    connectdialog.ui \
    createdialog.ui \
    gomoku.ui \
    saveloaddialog.ui

RESOURCES += \
    resource.qrc
