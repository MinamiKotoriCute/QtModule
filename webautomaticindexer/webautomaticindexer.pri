QT       += network

PROHECT_PATH = $$PWD

INCLUDEPATH += $${PROHECT_PATH}

SOURCES += $${PROHECT_PATH}\webautomaticindexer.cpp

HEADERS  += $${PROHECT_PATH}\webautomaticindexer.h

include("../asynchronous/asynchronous.pri")
