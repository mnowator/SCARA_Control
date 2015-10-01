#-------------------------------------------------
#
# Project created by QtCreator 2015-09-19T00:31:38
#
#-------------------------------------------------

QT       += core

TARGET = ScaraRobot
TEMPLATE = lib

DEPENDPATH += . ../ScaraCommunicationLayer\
   ../ScaraLogic
INCLUDEPATH += ../ScaraCommunicationLayer\
    ../ScaraLogic
LIBS += -L../ScaraCommunicationLayer/debug -lScaraCommunicationLayer\
    -L../ScaraLogic/debug -lScaraLogic

SOURCES += scararobot.cpp

HEADERS += scararobot.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
