QT       += core gui
QT       += widgets

CONFIG   += c++11

TARGET = qrtsp-viewer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

RC_FILE = qrtsp-viewer.rc

SOURCES += main.cpp \
        mainwindow.cpp \
        rtspreader.cpp

HEADERS  += mainwindow.h \
         rtspreader.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

win32 {
    # Some of the following libraries may be necessary for a Debug build
    # only, but let's keep them...
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_core3416.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_video3416.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_videoio3416.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_imgcodecs3416.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_imgproc3416.dll.a"
    LIBS += "C:\lib\build_opencv\install\x64\mingw\lib\libopencv_highgui3416.dll.a"
    INCLUDEPATH += C:\lib\build_opencv\install\include
}

unix{
    LIBS += -L/usr/local/lib
    LIBS += -lopencv_highgui -lopencv_videostab -lopencv_imgcodecs -lopencv_video -lopencv_videoio -lopencv_imgproc -lopencv_core
    INCLUDEPATH += /usr/local/include/opencv2
    # These 3 lines are equal to
    # g++ example.cpp -o example -I/usr/local/include/opencv2 -L/usr/local/lib
    # -lopencv_highgui -lopencv_videostab -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_imgproc -lopencv_core
}
