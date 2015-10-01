#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T14:50:33
#
#-------------------------------------------------

QT       += core

TARGET = ScaraLogic
TEMPLATE = lib

SOURCES += scaralogic.cpp

HEADERS += scaralogic.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
