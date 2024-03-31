#ifndef _MENU_H_
#define _MENU_H_

#include <QMainWindow>
#include <QPainter>
#include <QFont>
#include <QPushButton>
#include <QMessageBox>

#include "ChoosePlayerWidget.hpp"
#include "GameWidget.hpp"

 
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(int WIDTH, int HEIGHT, QWidget *parent = nullptr);
    ChoosePlayerWidget* choosePlayerWidget;
    GameWidget *gameWidget;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void gameBegin();
    void showChoosePlayerWidget();
    void About();
    void exitGame();

private:
    int Window_Width, Window_Height;
    QPixmap menuPixmap;

    QPushButton *startGameButton;
    QPushButton *choosePlayerButton;
    QPushButton *aboutButton;
    QPushButton *exitButton;
};

#endif // _MENU_H_