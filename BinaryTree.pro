TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    binarytree.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    binarytree.h \
    consoletree.h \
    prettytree.h

