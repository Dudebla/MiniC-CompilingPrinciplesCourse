#-------------------------------------------------
#
# Project created by QtCreator 2020-04-14T19:47:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MiniCCompiling
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#copy by dude----------------------------------------------------------------------------------------
#CONFIG += c++11
#####################################################################
## Stanford CS 106 B/X Qt Creator project file                     ##
## (now compatible with Qt-based GUI system!)                      ##
#####################################################################
#
# This file specifies the information about your project to Qt Creator.
# You should not need to modify this file to complete your assignment.
#
#####################################################################
## If you need to add files or folders to your project, we suggest ##
## that you "re-initialize" your project by doing the following:   ##
##                                                                 ##
## 1- close Qt Creator.                                            ##
## 2- delete your ".pro.user" file and "build_xxxxxxx" directory.  ##
## 3- place the new files/folders into your project directory.     ##
## 4- re-open and "Configure" your project again.                  ##
#####################################################################
#
# @author Marty Stepp
#     (past authors/support by Reid Watson, Rasmus Rygaard, Jess Fisher, etc.)
# @version 2018/10/23
# - added Qt multimedia flag for sound and video playback
###############################################################################
# BEGIN SECTION FOR SPECIFYING COMPILER/LINKER FLAGS AND LIBRARIES            #
###############################################################################

# set up flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off for simplicity.)
CONFIG += no_include_pwd         # make sure we do not accidentally #include files placed in 'resources'
CONFIG += sdk_no_version_check   # removes spurious warnings on Mac OS X

win32 {
    # some Windows systems have old MinGW compilers, so be safe and use C++11
    CONFIG += c++11
} else {
    # Mac/Linux should support the latest C++ features
    CONFIG += c++14
}

# uncomment this if you want to dump the preprocessor output into the .o files
# (useful when debugging advanced preprocessor hacking)
# QMAKE_CXXFLAGS += -E

QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -Wcast-align
#QMAKE_CXXFLAGS += -Wfloat-equal
QMAKE_CXXFLAGS += -Wformat=2
QMAKE_CXXFLAGS += -Wlogical-op
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
QMAKE_CXXFLAGS += -Wno-old-style-cast
QMAKE_CXXFLAGS += -Wno-sign-compare
QMAKE_CXXFLAGS += -Wno-sign-conversion
QMAKE_CXXFLAGS += -Wno-write-strings
QMAKE_CXXFLAGS += -Wreturn-type
QMAKE_CXXFLAGS += -Werror=return-type
QMAKE_CXXFLAGS += -Werror=uninitialized
QMAKE_CXXFLAGS += -Wunreachable-code

#win32 {
#    # additional flags for Windows
#    LIBS += -lDbghelp
#    LIBS += -lbfd
#    LIBS += -limagehlp
#    cache()
#} else {
#    # flags that don't work on Windows MinGW compiler
#    # QMAKE_CXXFLAGS += -Wno-keyword-macro
#    # QMAKE_CXXFLAGS += -Wno-reserved-id-macro
#    QMAKE_CXXFLAGS += -Wno-unused-const-variable
#    LIBS += -ldl
#}

# additional flags for Mac OS X
macx {
    # calling cache() reduces warnings on Mac OS X systems
    cache()
    QMAKE_MAC_SDK = macosx
}

# additional flags for Linux
unix:!macx {
    cache()
}

# libraries for all OSes
LIBS += -lpthread

# additional flags for clang compiler (default on Mac)
COMPILERNAME = $$QMAKE_CXX
COMPILERNAME ~= s|.*/|
equals(COMPILERNAME, clang++) {
    QMAKE_CXXFLAGS += -Wno-format-nonliteral
    QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override
    QMAKE_CXXFLAGS += -Wno-overloaded-virtual
    QMAKE_CXXFLAGS += -Wno-unknown-warning-option
}

# set up configuration flags used internally by the Stanford C++ libraries
# These flags are all optional but can simplify project configuration.
# (setting x/y to 999999 centers the window)
# (see platform.cpp/h for descriptions of some of these flags)

# what version of the Stanford .pro is this? (kludgy integer YYYYMMDD format)
DEFINES += SPL_PROJECT_VERSION=20181023

# wrapper name for 'main' function (needed so student can write 'int main'
# but our library can grab the actual main function to initialize itself)
DEFINES += REPLACE_MAIN_FUNCTION=1
DEFINES += main=qMain

# x/y location and w/h of the graphical console window; set to -1 to center
DEFINES += SPL_CONSOLE_X=-1
DEFINES += SPL_CONSOLE_Y=-1
DEFINES += SPL_CONSOLE_WIDTH=800
DEFINES += SPL_CONSOLE_HEIGHT=500

# font size of the font in the graphical console window; can also be set via window menu
#DEFINES += SPL_CONSOLE_FONTSIZE=18

# echo graphical console onto the plain text console as well?
DEFINES += SPL_CONSOLE_ECHO

# quit the C++ program when the graphical console is closed?
DEFINES += SPL_CONSOLE_EXIT_ON_CLOSE

# crash if the .pro is older than the minimum specified in version.h? (SPL_PROJECT_VERSION)
DEFINES += SPL_VERIFY_PROJECT_VERSION

# allow clients to access the internal data inside the heap of PriorityQueue?
# (used for some practice exam exercises/demos)
DEFINES += PQUEUE_ALLOW_HEAP_ACCESS

# should toString / << of a PriorityQueue display the elements in sorted order,
# or in heap internal order? the former is more expected by client; the latter
# is faster and avoids a deep-copy
DEFINES += PQUEUE_PRINT_IN_HEAP_ORDER

# flag to throw exceptions when a collection iterator is used after it has
# been invalidated (e.g. if you remove from a Map while iterating over it)
DEFINES += SPL_THROW_ON_INVALID_ITERATOR

# flag to add members like 'cost', 'visited', etc. to BasicGraph Vertex/Edge
# (we are going to disable these to force more interesting implementations)
# DEFINES += SPL_BASICGRAPH_VERTEX_EDGE_RICH_MEMBERS

# should we throw an error() when operator >> fails on a collection?
# for years this was true, but the C++ standard says you should just silently
# set the fail bit on the stream and exit, so that has been made the default.
# DEFINES += SPL_ERROR_ON_STREAM_EXTRACT

# scale up the console window on high-DPI screens?
# DEFINES += SPL_SCALE_HIGH_DPI_SCREEN

# is the .cpp portion of the library merged into a single .cpp file
# to speed up compilation?
DEFINES += SPL_MERGED_LIBRARY_SINGLE_FILE

# should we attempt to precompile the Qt moc_*.cpp files for speed?
DEFINES += SPL_PRECOMPILE_QT_MOC_FILES

# build-specific options (debug vs release)

# make 'debug' target (default) use no optimization, generate debugger symbols,
# and catch/print to console any uncaught exceptions thrown by the program
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g3
    QMAKE_CXXFLAGS += -fno-inline
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer

    unix:!macx {
        equals(COMPILERNAME, g++) {
            # on Linux g++, these flags help us gather line numbers for stack traces
            QMAKE_CXXFLAGS += -rdynamic
            QMAKE_CXXFLAGS += -export-dynamic
            QMAKE_CXXFLAGS += -Wl,--export-dynamic
            QMAKE_LFLAGS += -export-dynamic
            QMAKE_LFLAGS += -rdynamic
        }
    }
    unix:macx {
        equals(COMPILERNAME, clang++) {
            QMAKE_LFLAGS += -rdynamic
        }
    }

    # print details about uncaught exceptions with red error text / stack trace
    DEFINES += SPL_CONSOLE_PRINT_EXCEPTIONS
}

# make 'release' target be statically linked so it is a stand-alone executable
CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -O2
    macx {
        QMAKE_POST_LINK += 'macdeployqt $${OUT_PWD}/$${TARGET}.app'
        #QMAKE_POST_LINK += 'macdeployqt $${OUT_PWD}/$${TARGET}.app && rm $${OUT_PWD}/*.o && rm $${OUT_PWD}/Makefile'
    }
    unix:!macx {
        # commenting out -static because it doesn't link to Qt libraries properly
        #QMAKE_LFLAGS += -static
        QMAKE_LFLAGS += -static-libgcc
        QMAKE_LFLAGS += -static-libstdc++
    }
    win32 {
        TARGET_PATH = $${OUT_PWD}/release/$${TARGET}.exe
        TARGET_PATH ~= s,/,\\,g

        OUT_PATH = $${OUT_PWD}/
        OUT_PATH ~= s,/,\\,g

        REMOVE_DIRS += '"'$${OUT_PWD}/release'"'
        REMOVE_DIRS += '"'$${OUT_PWD}/debug'"'
        REMOVE_FILES += '"'$${OUT_PWD}/Makefile'"'
        REMOVE_FILES += '"'$${OUT_PWD}/Makefile.Debug'"'
        REMOVE_FILES += '"'$${OUT_PWD}/Makefile.Release'"'
        REMOVE_FILES += '"'$${OUT_PWD}/object_script.$${TARGET}.Debug'"'
        REMOVE_DIRS ~= s,/,\\,g
        REMOVE_FILES ~= s,/,\\,g

        #QMAKE_LFLAGS += -static
        QMAKE_LFLAGS += -static-libgcc
        QMAKE_LFLAGS += -static-libstdc++
        QMAKE_POST_LINK += copy '"'$${TARGET_PATH}'"' '"'$${OUT_PATH}'"'
    }
}

###############################################################################
# END SECTION FOR SPECIFYING COMPILER/LINKER FLAGS AND LIBRARIES              #
###############################################################################


#----------------------------------------------------------------------------------------------------


SOURCES += \
        main.cpp \
        mainwindow.cpp \
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

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
