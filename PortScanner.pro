TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    thpool.c \
    main.c

LIBS += -lpthread

HEADERS += \
    thpool.h


