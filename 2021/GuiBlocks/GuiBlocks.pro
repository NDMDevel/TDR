QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include("guiblock_macros.mk")

SOURCES += \
    GuiBlocks/Block.cpp \
    GuiBlocks/Clipboard.cpp \
    GuiBlocks/Exceptions.cpp \
    GuiBlocks/Link.cpp \
    GuiBlocks/MouseTracker.cpp \
    GuiBlocks/Node.cpp \
    GuiBlocks/Painter.cpp \
    GuiBlocks/Scene.cpp \
    GuiBlocks/Style.cpp \
    GuiBlocks/Utils.cpp \
    GuiBlocks/View.cpp \
    GuiBlocks/Links.cpp \
    main.cpp \
    mainwindow.cpp


HEADERS += \
    GuiBlocks/Block.h \
    GuiBlocks/Clipboard.h \
    GuiBlocks/Exceptions.h \
    GuiBlocks/Link.h \
    GuiBlocks/MouseTracker.h \
    GuiBlocks/Node.h \
    GuiBlocks/Painter.h \
    GuiBlocks/Scene.h \
    GuiBlocks/Style.h \
    GuiBlocks/TypeID.h \
    GuiBlocks/Utils.h \
    GuiBlocks/View.h \
    GuiBlocks/Links.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    guiblock_macros.mk
