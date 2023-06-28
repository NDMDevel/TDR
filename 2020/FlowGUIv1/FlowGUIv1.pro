QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    baseblock.cpp \
    blockconnector.cpp \
    customgraphicsview.cpp \
    gridgraphicsitem.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    baseblock.h \
    blockconnector.h \
    customgraphicsview.h \
    gridgraphicsitem.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

DISTFILES += \
    IconsPNG/Applications2.png \
    IconsPNG/Burnable2.png \
    IconsPNG/Desktop2.png \
    IconsPNG/Documents2.png \
    IconsPNG/Downloads2.png \
    IconsPNG/Group2.png \
    IconsPNG/Home3.png \
    IconsPNG/Library2.png \
    IconsPNG/Movie2.png \
    IconsPNG/Music2.png \
    IconsPNG/Photos2.png \
    IconsPNG/Public2.png \
    IconsPNG/Server2.png \
    IconsPNG/Server3.png \
    IconsPNG/Sites2.png \
    IconsPNG/Sites3.png \
    IconsPNG/Smart2.png \
    IconsPNG/Software.png \
    IconsPNG/Torrents.png \
    IconsPNG/Users2.png \
    IconsPNG/Utilities2.png \
    IconsPNG/genqt.sh \
    IconsPNG/log.txt

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/tinyxml2/lib/' -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/tinyxml2/lib/' -ltinyxml2d

INCLUDEPATH += 'C:/Program Files (x86)/tinyxml2/include'
DEPENDPATH += 'C:/Program Files (x86)/tinyxml2/include'

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += 'C:/Program Files (x86)/tinyxml2/lib/libtinyxml2.a'
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += 'C:/Program Files (x86)/tinyxml2/lib/libtinyxml2d.a'
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += 'C:/Program Files (x86)/tinyxml2/lib/tinyxml2.lib'
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += 'C:/Program Files (x86)/tinyxml2/lib/tinyxml2d.lib'
