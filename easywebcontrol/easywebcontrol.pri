PROHECT_PATH = $$PWD

INCLUDEPATH += $${PROHECT_PATH}

SOURCES += $${PROHECT_PATH}\easywebcontrol.cpp

HEADERS  += $${PROHECT_PATH}\easywebcontrol.h

include("..\webautomaticindexer\webautomaticindexer.pri")
include("..\commonfunction\commonfunction.pri")
