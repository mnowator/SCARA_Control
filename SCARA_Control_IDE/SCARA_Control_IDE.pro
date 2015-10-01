#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T14:24:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SCARA_Control_IDE
TEMPLATE = app

DEPENDPATH += . ../ScaraControlGUI\
../ScaraRobot\
../ScaraCommunicationLayer\
../ScaraLogic

INCLUDEPATH += ../ScaraControlGUI\
../ScaraRobot\
../ScaraCommunicationLayer\
../ScaraLogic

LIBS += -L../ScaraControlGUI/debug -lScaraControlGUI\
-L../ScaraRobot/debug -lScaraRobot\
-L../ScaraCommunicationLayer/debug -lScaraCommunicationLayer\
-L../ScaraLogic/debug -lScaraLogic


SOURCES += main.cpp

HEADERS  +=

FORMS    +=
