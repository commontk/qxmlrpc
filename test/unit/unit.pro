CONFIG += qtestlib
TEMPLATE = app
TARGET = unit_tests
DEPENDPATH += .
INCLUDEPATH += . ../../xmlrpc/

QT += xml network

# Input
HEADERS += testqxmlrpc.h
SOURCES += testqxmlrpc.cpp

unix:LIBS += ../../libqxmlrpc_debug.a
win32:LIBS += ../../xmlrpc/qxmlrpc.lib

