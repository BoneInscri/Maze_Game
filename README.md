# Maze_Game

这是一个迷宫小游戏，可以在Linux下运行。

图形库通过Qt创建。

### 环境搭建

Ubuntu下安装QT依赖

```
sudo apt-get install qtbase5-dev qt5-qmake libqt5widgets5
```

使用 `qmake -v` 找到qmake在哪里？

```
$ qmake -v
QMake version 3.1
Using Qt version 5.15.3 in /usr/lib/x86_64-linux-gnu
```

```
# find / -name qmake
/usr/bin/qmake
/usr/lib/x86_64-linux-gnu/qt5/bin/qmake
/usr/lib/qt5/bin/qmake
```

```
$ /usr/bin/qmake -query QT_INSTALL_PREFIX
/usr
```

创建一个.pro文件

下面是一个示例：

```
QT       += core gui widgets
CONFIG   += c++11
TARGET   = MyQtApp
TEMPLATE = app
SOURCES += main.cpp 
    mainwindow.cpp
HEADERS += mainwindow.h
FORMS   += mainwindow.ui
```

TEMPLATE

* = app，表示的是一个应用程序。
* = lib，表示的是一个库。
* = subdirs，表示的是一个包含子项目的容器。

将.pro转化为Makefile

```
qmake
```

如果是vscode开发，那么需要配置一下，防止爆红。

Ctrl+Shift+P

C/C++: Edit Configurations (UI)，进入c_cpp_properties.json文件

向Include path中添加：

```
"/usr/include/x86_64-linux-gnu/qt5",
"/usr/include/x86_64-linux-gnu/qt5/QtCore",
"/usr/include/x86_64-linux-gnu/qt5/QtGui",
"/usr/include/x86_64-linux-gnu/qt5/QtWidgets"
```


### 项目构建并运行

```
sh run.sh
```


### 项目清理

```
sh clean.sh
```
