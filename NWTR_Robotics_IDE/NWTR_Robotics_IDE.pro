#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T14:24:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NWTR_Robotics_IDE
TEMPLATE = app

DEPENDPATH += . ../NWTR_Robotics_GUI\
../Robot_Project\
../Communication_Layer\
../ScaraLogic

INCLUDEPATH += ../NWTR_Robotics_GUI\
../Robot_Project\
../Communication_Layer\
../ScaraLogic

LIBS += -L../NWTR_Robotics_GUI/debug -lNWTR_Robotics_GUI\
-L../Robot_Project/debug -lRobot_Project\
-L../Communication_Layer/debug -lCommunication_Layer\
-L../ScaraLogic/debug -lScaraLogic


SOURCES += main.cpp

HEADERS  +=

FORMS    +=
