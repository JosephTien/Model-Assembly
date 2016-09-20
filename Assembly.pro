#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T10:01:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Assembly
TEMPLATE = app
CONFIG += c++11 precompile_header

SOURCES += main.cpp\
        mainwindow.cpp\
        viewmanager.cpp\
        modelmanager.cpp\
        modelviewer.cpp\
        standarshader.cpp\
        objLoader.cpp \
        modelutility.cpp

HEADERS  += mainwindow.h \
        pch.h\
        viewmanager.h\
        modelmanager.h\
        modelviewer.h\
        standarshader.h\
        iglmachine.h \
        tiny_obj_loader.h \
    looplist.h

PRECOMPILED_HEADER = pch.h
QMAKE_CXXFLAGS_DEBUG -= -g
QMAKE_CXXFLAGS_RELEASE -= -O2

FORMS    += mainwindow.ui

RESOURCES += \
    standarshader.qrc \
    object.qrc

win32 {
    message(win)
    INCLUDEPATH += $$PWD/externals/include/glew/
    INCLUDEPATH += $$PWD/externals/include/eigen3/
    INCLUDEPATH += $$PWD/externals/include/glm/
    INCLUDEPATH += $$PWD/externals/include/CGAL-4_8_1/
    INCLUDEPATH += $$PWD/externals/include/boost-1_61/
    INCLUDEPATH += $$PWD/externals/include/gmp/
    INCLUDEPATH += $$PWD/externals/include/mpfr/
    LIBS += -lopengl32
    LIBS += $$PWD/externals/lib/x64/glew32s.lib
    LIBS += $$PWD/externals/lib/x64/tinyobjloader_debug.lib
    LIBS += $$PWD/externals/lib/x64/libboost_thread-vc120-mt-1_61.lib
    LIBS += $$PWD/externals/lib/x64/libboost_system-vc120-mt-1_61.lib
    LIBS += $$PWD/externals/lib/x64/CGAL-vc120-mt-4.8.1.lib
    #LIBS += $$PWD/externals/lib/x64/mpfr.lib
    #LIBS += $$PWD/externals/lib/x64/gmp.lib
    #LIBS += $$PWD/externals/lib/x64/glew32.lib
    #LIBS += $$PWD/externals/lib/x64/tinyobjloader.lib

    QMAKE_LFLAGS_DEBUG = /NODEFAULTLIB:libcmt.lib
    QMAKE_LFLAGS_RELEASE = /NODEFAULTLIB:libcmt.lib

}
macx {
    message(macx)
    INCLUDEPATH += /usr/local/include
    INCLUDEPATH += /usr/local/include/eigen3
    LIBS += -L/usr/local/lib -lglew
    LIBS += -lCGAL -lgmp -lmpfr -lboost_thread-mt -lboost_system
    LIBS += $$PWD/externals/lib/mac/libtinyobjloader.a
}
INCLUDEPATH += $$PWD/externals/include/igl

DISTFILES +=
