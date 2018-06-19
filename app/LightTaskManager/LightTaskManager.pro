#-------------------------------------------------
#
# Project created by QtCreator 2018-03-06T12:31:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LightTaskManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    gui/mainwindow.cpp \
    models/todolistadapter.cpp \
    models/settingsmanager.cpp \
    gui/widgets/mylistwidget.cpp \
    gui/adddialog.cpp \
    gui/deletetaskdialog.cpp \
    gui/settingsdialog.cpp \
    models/taskmanager.cpp \
    gui/widgets/mylistwidgetitem.cpp \
    gui/archivebystatusdialog.cpp \
    gui/showdialog.cpp

HEADERS += \
        gui/mainwindow.h \
    models/todolistadapter.h \
    models/settingsmanager.h \
    gui/widgets/mylistwidget.h \
    gui/adddialog.h \
    gui/deletetaskdialog.h \
    gui/settingsdialog.h \
    models/taskmanager.h \
    gui/widgets/mylistwidgetitem.h \
    gui/archivebystatusdialog.h \
    gui/showdialog.h

FORMS += \
        gui/mainwindow.ui \
    gui/adddialog.ui \
    gui/deletetaskdialog.ui \
    gui/settingsdialog.ui \
    gui/archivebystatusdialog.ui \
    gui/showdialog.ui

RESOURCES += \
    gui/resources/resources.qrc
