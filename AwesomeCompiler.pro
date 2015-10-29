#-------------------------------------------------
#
# Project created by QtCreator 2015-10-03T09:29:59
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AwesomeCompiler
TEMPLATE = app


SOURCES += main.cpp\
    lexicalanalyzer.cpp \
    ui.cpp

HEADERS  += \
    lexicalanalyzer.h \
    ui.h \
    lexemetype.h

FORMS    += \
    ui.ui

TRANSLATIONS += AwesomeCompiler-ru.ts

CONFIG += c++11

RC_ICONS = icon.ico
RESOURCES += \
    resources.qrc
