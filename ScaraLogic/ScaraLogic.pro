#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T14:50:33
#
#-------------------------------------------------

QT       += core

CONFIG += c++11

TARGET = ScaraLogic
TEMPLATE = lib

SOURCES += scaralogic.cpp \
    steppermotorramp.cpp \
    node.cpp

HEADERS += scaralogic.h \
    steppermotorramp.h \
    node.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
