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
LIBS += -lcx_net_sockets -lcx_mem_streams -lcx_net_threadedacceptor -lcx_mem_streamparser -lcx_protocols_http -lcx_protocols_mime -lcx_protocols_urlvars -lcx_mem_vars -lcx_mem_containers -lcx_mem_streamencoders -lcx_mem_abstracts
LIBS += -lpthread -lcx_thr_mutex -lboost_system

TARGET = cx_http_server

SOURCES += main.cpp \
    httpv1_server_impl.cpp

HEADERS += \
    httpv1_server_impl.h
################################################

# INSTALLATION:
target.path = $$PREFIX/bin
INSTALLS += target
