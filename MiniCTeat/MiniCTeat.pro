QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MiniCTeat
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.C \
        mainwindow.C \
        ANALYZE.C \
        CGEN.C \
        CODE.C \
        PARSE.C \
        SCAN.C \
        SYMTAB.C \
        UTIL.C

HEADERS += \
        mainwindow.h \
        ANALYZE.H \
        CGEN.H \
        CODE.H \
        GLOBALS.H \
        PARSE.H \
        SCAN.H \
        SYMTAB.H \
        UTIL.H

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui

