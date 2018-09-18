#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T06:31:52
#
#-------------------------------------------------

QT       += core gui charts concurrent network

TARGET = PBE
TEMPLATE = app

macos:LIBS += /usr/lib/libcurl.dylib
win32:INCLUDEPATH += "c:\Users\SimCenter\libCurl-7.59.0\include"
win32:LIBS += C:\Users\SimCenter\libCurl-7.59.0/lib/libcurl.lib
linux:LIBS += /usr/lib/x86_64-linux-gnu/libcurl.so

include(../EE-UQ/EE-UQ.pri)
include(../SimCenterCommon/Common/Common.pri)
include(../SimCenterCommon/RandomVariables/RandomVariables.pri)
include(../SimCenterCommon/InputSheetBM/InputSheetBM.pri)
include(../GroundMotionUtilities/UI/GroundMotionWidgets.pri)

INCLUDEPATH += "./Component"

SOURCES += main.cpp \
    WorkflowAppPBE.cpp \
    RunWidget.cpp \
    Components/Component.cpp \
    Components/ComponentContainer.cpp


HEADERS  += \
    WorkflowAppPBE.h \
    RunWidget.h \
    Components/Component.h \
    Coponents/ComponentContainer.h \
    Components/ComponentContainer.h


RESOURCES += \
    ../EE-UQ/images.qrc \

#FORMS    += mainwindow.ui

#RESOURCES += \
#    schema.qrc


