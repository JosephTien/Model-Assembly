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
        cgalholefiller.cpp \
        cgalremesher.cpp \
        #cgalmachine.cpp
        mainwindow_support.cpp \
        modelmanager_support.cpp \
        modelmanager_utility.cpp \
        #modelmanager_utility_unusedcodebackup.cpp
    maintainthread.cpp

HEADERS  += mainwindow.h \
        pch.h\
        viewmanager.h\
        modelmanager.h\
        modelviewer.h\
        standarshader.h\
        iglmachine.h \
        tiny_obj_loader.h \
        looplist.h \
        cgaltool.h \
        pch_cgal.h \
        cgalutility.h \
        #cgalmachine.h
        skelmanager.h \
        maintainthread.h \
        maintainvar.h \
        mainwindow_support.h

PRECOMPILED_HEADER = pch.h #pch_cgal.h
QMAKE_CXXFLAGS_DEBUG -= -g
QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS += -bigobj

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
    #LIBS += $$PWD/externals/lib/x64/glew32.lib
    LIBS += $$PWD/externals/lib/x64/tinyobjloader_140.lib
    #LIBS += $$PWD/externals/lib/x64/tinyobjloader_140_db.lib

    LIBS += $$PWD/externals/lib/x64/libboost_thread-vc140-mt-1_61.lib
    LIBS += $$PWD/externals/lib/x64/libboost_system-vc140-mt-1_61.lib
    LIBS += $$PWD/externals/lib/x64/CGAL-vc140-mt-4.8.1.lib
    #LIBS += $$PWD/externals/lib/x64/CGAL-vc140-mt-gd-4.8.1.lib

    LIBS += $$PWD/externals/lib/x64/mpir.lib # replace gmp for x64?
    #LIBS += $$PWD/externals/lib/x64/mpir_db.lib # replace gmp for x64?
    #LIBS += $$PWD/externals/lib/x64/gmp.lib
    LIBS += $$PWD/externals/lib/x64/mpfr.lib


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
