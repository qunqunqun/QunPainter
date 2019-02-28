#-------------------------------------------------
#
# Project created by QtCreator 2018-10-21T21:25:51
#
#-------------------------------------------------
QT +=core gui opengl
RC_ICONS = images/color.ico
win32:LIBS += -lOpengl32 \
                -lglu32 \
                -lglut

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTpaint
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myopengl.cpp \
    mywidget.cpp \
    thepen.cpp \
    Figure.cpp \
    Line.cpp \
    Ellipse.cpp \
    Circle.cpp \
    Ploygon.cpp \
    Bezier.cpp \
    3DWidget.cpp

HEADERS += \
        mainwindow.h \
    myopengl.h \
    mywidget.h \
    thepen.h \
    Figure.h \
    Line.h \
    Ellipse.h \
    Circle.h \
    Polygon.h \
    Bezier.h \
    3DWidget.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
