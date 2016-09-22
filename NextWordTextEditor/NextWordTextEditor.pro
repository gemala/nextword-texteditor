TEMPLATE = app

QT += widgets
CONFIG += c++11

SOURCES += main.cpp \
    mainwindow.cpp \
    textedit.cpp \
    highlighter.cpp

RESOURCES += \
    NextWordTextEditor.qrc

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    mainwindow.h \
    textedit.h \
    highlighter.h
