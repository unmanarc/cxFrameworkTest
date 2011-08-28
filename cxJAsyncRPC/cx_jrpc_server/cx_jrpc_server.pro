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
LIBS += -lcx_thr_threadpool -lcx_jasyncrpc_common -lcx_jasyncrpc_tserver -lcx_thr_mutex_map -lcx_mem_streamparser -lcx_net_sockets -lcx_net_threadedacceptor -lcx_mem_streams -lcx_mem_containers -lcx_auth_sqlite3 -lcx_auth_fs -lcx_auth
LIBS += -ljsoncpp
LIBS += -lsqlite3
LIBS += -lcrypto
LIBS += -lboost_system -lboost_filesystem -lpthread -lcx_thr_mutex

TARGET = cx_jrpc_server

SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.h
