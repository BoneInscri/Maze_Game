#include <QApplication>
#include "menu.hpp"

int Player_type_global = 1;
int WINDTH = 1200;
int HEIGHT = 800;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow(WINDTH, HEIGHT);
    mainWindow.show();
    return app.exec();
}