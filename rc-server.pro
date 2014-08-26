QT       += core network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = server
TEMPLATE = app
SOURCES += main.cpp\
    server.cpp
HEADERS  += \
    server.h\
    defines.h
CONFIG -= console
RESOURCES += \
    icons.qrc
RC_FILE += rc-server.rc
