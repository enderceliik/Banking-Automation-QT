QT       += core gui
QT += sql
QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    interface.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    database.h \
    interface.h \
    mainwindow.h

FORMS += \
    interface.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc \
    res/resources.qrc

DISTFILES += \
    res/bank_icon.png \
    res/dark_blur_wallpaper.jpg \
    res/dollar_design.png \
    res/github.png \
    res/interface_background3.jpg
