#ifndef _MENU_H_
#define _MENU_H_

#include <QMainWindow>
#include <QPainter>
#include <QFont>
#include <QPushButton>
#include <QMessageBox>

#include "ChoosePlayerWidget.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void gameBegin();
    void showChoosePlayerWidget();
    void About();
    void exitGame();

private:
    QPixmap menuPixmap;

    QPushButton *startGameButton;
    QPushButton *choosePlayerButton;
    QPushButton *aboutButton;
    QPushButton *exitButton;
    
    ChoosePlayerWidget* choosePlayerWidget;
};

#endif // _MENU_H_