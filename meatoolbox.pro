QT += core gui widgets

CONFIG += c++11

TARGET = meatoolbox
#CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

#INCLUDEPATH += frostbite

SOURCES += main.cpp \
    meawindow.cpp \
    uberbuffer.cpp \
    frostbite/bson.cpp \
    frostbite/decrypt.cpp
HEADERS += meawindow.hpp \
    frostbite\decrypt.hpp \
    uberbuffer.hpp \
    frostbite/bson.hpp


LIBS += Comdlg32.lib
PRECOMPILED_HEADER = precompiled.hpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
