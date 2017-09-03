QT  += core

PROHECT_PATH = $$PWD

INCLUDEPATH += $${PROHECT_PATH}

SOURCES += $${PROHECT_PATH}\expansion.cpp \
    $$PWD/defineexpansion.cpp \
    $$PWD/defineprivatedata.cpp

HEADERS  += $${PROHECT_PATH}\expansion.h \
    $$PWD/defineexpansion.h \
    $$PWD/defineprivatedata.h
