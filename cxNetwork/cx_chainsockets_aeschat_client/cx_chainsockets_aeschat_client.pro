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


################################################
LIBS += -lcx_net_sockets -lcx_mem_streams -lcx_net_chains -lcx_net_chains_tls -lcx_net_tls -lssl -lcrypto -lpthread

TARGET = cx_aeschat_client

SOURCES += main.cpp

HEADERS +=
################################################

# INSTALLATION:
target.path = $$PREFIX/bin
INSTALLS += target
