QT       += core gui opengl printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../controller/controller.cc \
    ../model/model.cc \
    ../main.cpp \
    mainwindow.cpp \
    openglwidget.cpp \

HEADERS += \
    ../controller/controller.h \
    ../model/command.h \
    ../model/model.h \
    mainwindow.h \
    openglwidget.h \

FORMS += \
    mainwindow.ui

macx:ICON = ../img/icon_3D_macos.icns

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
