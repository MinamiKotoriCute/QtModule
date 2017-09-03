#-------------------------------------------------
#
# Project created by QtCreator 2016-11-15T13:37:34
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Web
TEMPLATE = lib

DEFINES += WEB_LIBRARY

SOURCES += web.cpp

HEADERS += web.h\
        web_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
