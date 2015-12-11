#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T15:09:16
#
#-------------------------------------------------

QT       += widgets xml

CONFIG += c++11

TARGET = ScaraControlGUI
TEMPLATE = lib

DEPENDPATH += . ../ScaraRobot\
../ScaraCommunicationLayer\
../ScaraLogic

INCLUDEPATH += ../ScaraRobot\
../ScaraCommunicationLayer\
../ScaraLogic

LIBS += -L../ScaraRobot/debug -lScaraRobot\
-L../ScaraCommunicationLayer/debug -lScaraCommunicationLayer\
-L../ScaraLogic/debug -lScaraLogic


SOURCES += \
    mainwindow.cpp \
    newprojectdialog.cpp \
    newfiledialog.cpp \
    renamefiledialog.cpp \
    styles.cpp \
    savechangesdialog.cpp \
    removefiledialog.cpp \
    codeeditor.cpp

HEADERS += \
    mainwindow.h \
    newprojectdialog.h \
    newfiledialog.h \
    savechangesdialog.h \
    renamefiledialog.h \
    styles.h \
    removefiledialog.h \
    codeeditor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    mainwindow.ui \
    newprojectdialog.ui \
    newfiledialog.ui \
    savechangesdialog.ui \
    renamefiledialog.ui \
    removefiledialog.ui

RESOURCES += \
    resource.qrc
