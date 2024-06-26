# Maze_Game

这是一个迷宫小游戏，可以在Linux下运行。

本身是大一程序设计实践大作业在Windows下用EasyX写的一个小游戏，且原先是使用在C语法基础上使用了部分的C++SLT模板，但考虑到之后的开发都在Linux下进行开发，故将代码整理了一下，转化为了Qt的C++版本，打赢复活赛了。

### 环境搭建

Ubuntu下安装QT依赖

```
sudo apt-get install qtbase5-dev qt5-qmake libqt5widgets5 qtmultimedia5-dev 
sudo apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
sudo apt-get install libqt5multimedia5-plugins
apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
gst-inspect-1.0 --version
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

如何使用Qt Designer进行UI设计？

```
$ sudo apt-get install qttools5-dev-tools
$ designer
```

**Vscode 插件**

C/C++ Extension Pack

<img src="README.assets/image-20240331094825538.png" alt="image-20240331094825538" style="zoom: 67%;" />

### 项目构建并运行

```
sh run.sh
```

### 项目清理

```
sh clean.sh
```

#### map.txt的格式说明

- ROW COL TIME
- Map data
- number of monsters
- information of monsters : pos + dir

### 游戏效果

<img src="README.assets/image-20240401205256282.png" alt="image-20240401205256282" style="zoom: 50%;" />

<img src="README.assets/image-20240401205322732.png" alt="image-20240401205322732" style="zoom:67%;" />

![image-20240401205338621](README.assets/image-20240401205338621.png)

<img src="README.assets/image-20240401205351076.png" alt="image-20240401205351076" style="zoom:67%;" />

演示视频：

<img src="demo.gif" />



