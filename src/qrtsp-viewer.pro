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

RESOURCES += \
    resources.qrc

DISTFILES += \
    README.md

win32 {
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_core455.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_video455.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_videoio455.dll.a"
    INCLUDEPATH += C:\lib\build_opencv\install\include
}

unix{
    LIBS += -L/usr/local/lib
    LIBS += -lopencv_highgui -lopencv_videostab -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_imgproc -lopencv_core
    INCLUDEPATH += /usr/local/include/opencv2
    # These 3 lines are equal to
    # g++ example.cpp -o example -I/usr/local/include/opencv2 -L/usr/local/lib
    # -lopencv_highgui -lopencv_videostab -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_imgproc -lopencv_core
}
