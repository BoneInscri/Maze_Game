#ifndef CHOOSEPLAYERWIDGET_H
#define CHOOSEPLAYERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

class ChoosePlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChoosePlayerWidget(int WIDTH, int HEIGHT, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onPrevClicked();
    void onNextClicked();
    void finalizeSelection();

private:
    QPixmap backgroundPixmap;
    QPixmap playerPixmap;

    QRect playerArea;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *selectButton;

    QLabel *playerLabel;
    QLabel *nameLabel;

    int currentItem = 0;
    const int itemCount = 6; // 假设有6个角色

    static const char *name[];
    void setupUI();
    void updatePlayerImage();

signals:
    void characterChosen(int characterIndex);
};

#endif // CHOOSEPLAYERWIDGET_H
