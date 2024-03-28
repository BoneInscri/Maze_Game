#include "menu.hpp"
#include <QFont>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), menuPixmap("../img/menu/Menu.png") {
    setFixedSize(700, 600);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, menuPixmap);
    
    painter.setPen(Qt::red);
    QFont font = painter.font();
    font.setFamily("宋体");
    font.setPointSize(12);
    painter.setFont(font);

    font.setPointSize(25);
    painter.setFont(font);
    QRect rectSecondLine(0, 1 * height() / 4 + 30, width(), height() / 2);
    painter.drawText(rectSecondLine, Qt::AlignCenter, "HDU-CS");

    QRect rectFirstLine(0, height() / 4, width(), height() / 2);
    painter.drawText(rectFirstLine, Qt::AlignCenter, "本游戏由BoneInscri制作！");
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    if (event->button() == Qt::LeftButton) {
        // 开始游戏
        if (x >= 254 && x <= 443 && y >= 326 && y <= 374) {
            // 关闭当前窗口或清除当前界面
            // gameBegin(); // 这里调用开始游戏的函数
        }
        // 选择角色
        else if (x >= 254 && x <= 445 && y >= 374 && y <= 411) {
            // choosePlayer(); // 这里调用选择角色的函数
            // 回到菜单（或者更新界面）
        }
        // 相关信息
        else if (x >= 254 && x <= 444 && y >= 412 && y <= 447) {
            // About(); // 这里调用显示相关信息的函数
            // 回到菜单（或者更新界面）
        }
        // 退出游戏
        else if (x >= 253 && x <= 445 && y >= 451 && y <= 484) {
            // closegraph(); // 在Qt中，可能是关闭窗口
            this->close(); // 关闭当前窗口
        }
    }
}
