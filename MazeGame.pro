QT       += core gui widgets

CONFIG   += c++11

TARGET   = MazeGame
TEMPLATE = app

INCLUDEPATH += $$PWD/include

SOURCES += $$PWD/src/*.cpp
HEADERS += $$PWD/include/*.hpp

OBJECTS_DIR = ./build
DESTDIR = ./target

QMAKE_CLEAN += rm -rf ./build
QMAKE_CLEAN += rm -rf ./target

mkdircmd = mkdir -p
system($${mkdircmd} $${OBJECTS_DIR})
system($${mkdircmd} $${DESTDIR})

# RESOURCES += resources.qrc