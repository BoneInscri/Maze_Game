#include <QApplication>
#include "menu.hpp"

int Player_type_global = 1;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}