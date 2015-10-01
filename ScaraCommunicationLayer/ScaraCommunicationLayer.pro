#-------------------------------------------------
#
# Project created by QtCreator 2015-09-19T00:58:10
#
#-------------------------------------------------

QT       += core

TARGET = ScaraCommunicationLayer
TEMPLATE = lib

DEFINES += SCARACOMMUNICATIONLAYER_LIBRARY

SOURCES += \
    abstractcommunicationlayer.cpp \
    comcommunicationlayer.cpp

HEADERS += \
    abstractcommunicationlayer.h \
    comcommunicationlayer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
