QT       += core gui
QT       += widgets
QT       += network
QT       += multimediawidgets

CONFIG   += c++11

TARGET = qrtsp-viewer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
        rtspreader.cpp

HEADERS  += mainwindow.h \
    rtspreader.h

FORMS    += mainwindow.ui

RESOURCES +=

DISTFILES += \
    README.md

win32 {
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_core455.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_video455.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_videoio455.dll.a"
    INCLUDEPATH += C:\lib\build_opencv\install\include
}

unix{
    LIBS += -lopencv_videostab -lopencv_videoio -lopencv_core
    # equal to g++ -lopencv_videostab -lopencv_videoio -lopencv_core
    INCLUDEPATH += /usr/local/include/opencv4
    # equal to g++ -I/usr/local/include/opencv4
}
