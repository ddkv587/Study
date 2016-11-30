TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    source/Cell.cpp \
    source/Control.cpp \
    source/Snake.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    include/Cell.h \
    include/Control.h \
    include/Snake.h \
    SnakeGlobal.h
