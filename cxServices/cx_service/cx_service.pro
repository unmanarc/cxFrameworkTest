TEMPLATE = app
#CONFIG += static
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

isEmpty(PREFIX) {
    PREFIX = /usr/local
}
# includes dir
LIBS += -L$$PREFIX/lib

QMAKE_INCDIR += $$PREFIX/include
INCLUDEPATH += $$PREFIX/include

QMAKE_CXX += -Wno-write-strings -Wno-unused-parameter -Wno-unused-function -O3 -std=c++11 -Wunused -Wno-unused-result

DESTDIR=bin #Target file directory
OBJECTS_DIR=obj #Intermediate object files directory

# INSTALLATION:
target.path = $$PREFIX/bin
INSTALLS += target

################################################
LIBS += -lcx_srv_service -lcx_mem_abstracts
LIBS += -lpthread -lcx_thr_mutex

TARGET = cx_service

SOURCES += main.cpp
