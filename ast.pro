QT += core
QT -= gui

TARGET = ast
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ast.cpp\
 stable.cpp\
symbol.cpp\
fd.cpp \
    scanner.cpp \
    parser.cpp

HEADERS += \
stentry.h \
    stlist.h \
    stable.h\
    ast.h\
symbol.h\
fd.h \
    scanner.h \
    parser.h

CONFIG += c++11

DISTFILES += \
    input.txt
