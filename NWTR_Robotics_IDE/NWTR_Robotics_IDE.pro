#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T14:24:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NWTR_Robotics_IDE
TEMPLATE = app

DEPENDPATH += . ../NWTR_Robotics_GUI\
../Robot_Project\
../ScaraCommunicationLayer\
../ScaraLogic

INCLUDEPATH += ../NWTR_Robotics_GUI\
../Robot_Project\
../ScaraCommunicationLayer\
../ScaraLogic

LIBS += -L../NWTR_Robotics_GUI/debug -lNWTR_Robotics_GUI\
-L../Robot_Project/debug -lRobot_Project\
-L../ScaraCommunicationLayer/debug -lScaraCommunicationLayer\
-L../ScaraLogic/debug -lScaraLogic


SOURCES += main.cpp

HEADERS  +=

FORMS    +=
