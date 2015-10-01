#-------------------------------------------------
#
# Project created by QtCreator 2015-09-19T00:17:06
#
#-------------------------------------------------

QT       += core

TARGET = ScaraComunicationLayer
TEMPLATE = lib

DEFINES += SCARACOMUNICATIONLAYER_LIBRARY

SOURCES += \
    abstractlayer.cpp \
    comlayer.cpp

HEADERS += \
    comlayer.h \
    abstractcommlayer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
