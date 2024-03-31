#include "menu.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), menuPixmap("./img/menu/Menu.png")
{
    setFixedSize(600, 600);
    // 初始化按钮，并设置它们的文本和位置
    startGameButton = new QPushButton("开始游戏", this);
    choosePlayerButton = new QPushButton("选择角色", this);
    aboutButton = new QPushButton("相关信息", this);
    exitButton = new QPushButton("退出游戏", this);

    choosePlayerWidget = new ChoosePlayerWidget(this);
    gameWidget = new GameWidget(this);

    int buttonWidth = 192;
    int buttonHeight = 48;
    int firstButtonY = 316;
    int buttonSpacing = buttonHeight;

    startGameButton->setGeometry(224, firstButtonY, buttonWidth, buttonHeight);
    choosePlayerButton->setGeometry(224, firstButtonY + buttonSpacing, buttonWidth, buttonHeight);
    aboutButton->setGeometry(224, firstButtonY + 2 * buttonSpacing, buttonWidth, buttonHeight);
    exitButton->setGeometry(224, firstButtonY + 3 * buttonSpacing, buttonWidth, buttonHeight);

    // 连接信号和槽
    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::gameBegin);
    connect(choosePlayerButton, &QPushButton::clicked, this, &MainWindow::showChoosePlayerWidget);
    connect(aboutButton, &QPushButton::clicked, this, &MainWindow::About);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitGame);

    choosePlayerWidget->hide();
    gameWidget->hide();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, menuPixmap); // 绘制背景图

    // 设置初始字体
    painter.setPen(Qt::red);
    QFont font = painter.font();
    font.setFamily("宋体");
    font.setPointSize(12); // 假设这是初始设置，如果只需要一种大小，则不需要再次设置

    {
        QFont tempFont = font; // 复制当前设置
        tempFont.setPointSize(20); // 调整大小
        painter.setFont(tempFont); // 应用到painter

        // 绘制文本
        QRect rectSecondLine(0, height() / 64, width(), 40);
        painter.drawText(rectSecondLine, Qt::AlignCenter, tr("Made by HDU-CS BoneInscri"));
    }
}

void MainWindow::gameBegin()
{
    gameWidget->show();
    gameWidget->gameState = GameState::InGame;
    gameWidget->gameBegin();
}

void MainWindow::showChoosePlayerWidget() {
    choosePlayerWidget->show();
}

void MainWindow::About()
{
    QMessageBox::about(this, tr("关于游戏"), 
        tr("<h2>迷宫闯关游戏 1.0</h2>"
           "<p>版权 &copy; 2024 BoneInscri</p>"
           "<p>游戏名称是一个经典的迷宫闯关游戏，每个关卡需要躲避怪兽在限制的时间内到达终点，并支持自动寻路躲避怪兽到达终点。 </p>"
           "<p>本游戏由 HDU BoneInscri 开发，感谢您的支持！</p>"
           ));
}
void MainWindow::exitGame()
{
    this->close();
}