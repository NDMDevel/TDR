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

include("flowgui_macros.mk")

SOURCES += \
    FlowGUI/Block.cpp \
    FlowGUI/BlockImp/BlockImp.cpp \
    FlowGUI/BlockImp/PortType.cpp \
    FlowGUI/LinkImp/LinkContainerInterface.cpp \
    FlowGUI/LinkImp/LinkImp.cpp \
    FlowGUI/LinkImp/LinkTreeArray.cpp \
    FlowGUI/Exceptions.cpp \
    FlowGUI/LinkImp/NodeType.cpp \
    FlowGUI/LinkImp/NodeTypeIll.cpp \
    FlowGUI/StateMachine.cpp \
    FlowGUI/Style.cpp \
    FlowGUI/Utils.cpp \
    FlowGUI/View.cpp \
    FlowGUI/Link.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FlowGUI/Block.h \
    FlowGUI/BlockImp/BlockImp.h \
    FlowGUI/BlockImp/PortType.h \
    FlowGUI/LinkImp/LinkContainerInterface.h \
    FlowGUI/LinkImp/LinkImp.h \
    FlowGUI/LinkImp/LinkTreeArray.h \
    FlowGUI/Exceptions.h \
    FlowGUI/LinkImp/NodeType.h \
    FlowGUI/LinkImp/NodeTypeIll.h \
    FlowGUI/StateMachine.h \
    FlowGUI/Style.h \
    FlowGUI/Utils.h \
    FlowGUI/View.h \
    FlowGUI/Link.h \
    mainwindow.h \
    sml_114.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    flowgui_macros.mk

INCLUDEPATH += "FlowGUI/"

RESOURCES += \
    Icons.qrc
