#ifndef _MENU_H_
#define _MENU_H_


#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPixmap menuPixmap;
};


#endif // _MENU_H_