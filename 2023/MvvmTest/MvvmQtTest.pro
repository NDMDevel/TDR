QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a
CONFIG += no_keywords   # disable emit, signal and slots keywords

QMAKE_CXXFLAGS += -std=gnu++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    blockmodel.cpp \
    blockview.cpp \
    blockviewmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp

HEADERS += \
    NamedProperty.h \
    Utils/SerializableT.hpp \
    bindey/binding.h \
    bindey/property.h \
    blockmodel.h \
    blockview.h \
    blockviewmodel.h \
    mainwindow.h \
    model.h \
    nod/nod.hpp

INCLUDEPATH += bindey
INCLUDEPATH += nod

FORMS += \
    mainwindow.ui

LIBS += -L"C:/Program Files (x86)/jsoncpp/bin/" -ljsoncpp
INCLUDEPATH += "C:/Program Files (x86)/jsoncpp/include/"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
