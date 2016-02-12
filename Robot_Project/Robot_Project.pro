#-------------------------------------------------
#
# Project created by QtCreator 2015-09-19T00:31:38
#
#-------------------------------------------------

QT       += core widgets

TARGET = Robot_Project
TEMPLATE = lib

DEPENDPATH += . ../ScaraCommunicationLayer\
   ../ScaraLogic
INCLUDEPATH += ../ScaraCommunicationLayer\
    ../ScaraLogic
LIBS += -L../ScaraCommunicationLayer/debug -lScaraCommunicationLayer\
    -L../ScaraLogic/debug -lScaraLogic

SOURCES += \
    project.cpp

HEADERS += \
    project.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
