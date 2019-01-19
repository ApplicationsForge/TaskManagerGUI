#-------------------------------------------------
#
# Project created by QtCreator 2018-03-06T12:31:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TasksForge
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
    models/taskmanager.cpp \
    models/types/task/task.cpp \
    models/router.cpp \
    models/repository/repository.cpp \
    models/types/settings_manager/settings_manager.cpp \
    models/types/task_terminal_adapter/task_terminal_adapter.cpp \
    gui/widgets/my_list_widget.cpp \
    gui/widgets/my_list_widget_item.cpp \
    gui/add_dialog.cpp \
    gui/archive_dialog.cpp \
    gui/delete_task_dialog.cpp \
    gui/settings_dialog.cpp

HEADERS += \
        gui/mainwindow.h \
    models/taskmanager.h \
    models/types/task/task.h \
    models/router.h \
    models/repository/repository.h \
    models/types/settings_manager/settings_manager.h \
    models/types/task_terminal_adapter/task_terminal_adapter.h \
    gui/add_dialog.h \
    gui/archive_dialog.h \
    gui/delete_task_dialog.h \
    gui/settings_dialog.h \
    gui/widgets/my_list_widget.h \
    gui/widgets/my_list_widget_item.h

FORMS += \
        gui/mainwindow.ui \
    gui/add_dialog.ui \
    gui/archive_dialog.ui \
    gui/delete_task_dialog.ui \
    gui/settings_dialog.ui

RESOURCES += \
    gui/resources/resources.qrc
