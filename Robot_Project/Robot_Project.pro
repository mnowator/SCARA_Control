#-------------------------------------------------
#
# Project created by QtCreator 2015-09-19T00:31:38
#
#-------------------------------------------------

QT       += core widgets network xml

TARGET = Robot_Project
TEMPLATE = lib

DEPENDPATH += . ../Communication_Layer\
   ../ScaraLogic
INCLUDEPATH += ../Communication_Layer\
    ../ScaraLogic
LIBS += -L../Communication_Layer/debug -lCommunication_Layer\
    -L../ScaraLogic/debug -lScaraLogic

SOURCES += \
    project.cpp

HEADERS += \
    project.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
