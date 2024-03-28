QT       += core gui widgets

CONFIG   += c++11

TARGET   = MazeGame
TEMPLATE = app

INCLUDEPATH += $$PWD/include

SOURCES += $$PWD/src/*.cpp
HEADERS += $$PWD/include/*.hpp

OBJECTS_DIR = ./build
DESTDIR = ./target
MOC_DIR = ./moc

QMAKE_CLEAN += rm -rf $$OBJECTS_DIR
QMAKE_CLEAN += rm -rf $$DESTDIR
QMAKE_CLEAN += rm -rf $$MOC_DIR
QMAKE_CLEAN += rm Makefile

mkdircmd = mkdir -p
system($${mkdircmd} $${OBJECTS_DIR})
system($${mkdircmd} $${DESTDIR})
system($${mkdircmd} $${MO_CDIR})


# RESOURCES += resources.qrc