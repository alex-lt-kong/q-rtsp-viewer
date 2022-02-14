QT       += core gui
QT       += widgets
QT       += network
QT       += multimediawidgets

CONFIG   += c++11

TARGET = qrtsp-viewer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    README.md
