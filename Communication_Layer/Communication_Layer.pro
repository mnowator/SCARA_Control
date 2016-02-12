#-------------------------------------------------
#
# Project created by QtCreator 2015-09-19T00:58:10
#
#-------------------------------------------------

QT       += core network widgets

TARGET = Communication_Layer
TEMPLATE = lib

SOURCES += \
    ethernetcommunicationwidget.cpp

HEADERS += \
    ethernetcommunicationwidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
