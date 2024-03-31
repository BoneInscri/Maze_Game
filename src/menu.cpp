#include "menu.hpp"

MainWindow::MainWindow(int WIDTH, int HEIGHT, QWidget *parent) : QMainWindow(parent), menuPixmap("./img/menu/Menu.png")
{
    setFixedSize(WIDTH, HEIGHT);
    Window_Width = WIDTH;
    Window_Height = HEIGHT;
    // 初始化按钮，并设置它们的文本和位置
    startGameButton = new QPushButton("开始游戏", this);
    choosePlayerButton = new QPushButton("选择角色", this);
    aboutButton = new QPushButton("相关信息", this);
    exitButton = new QPushButton("退出游戏", this);

    choosePlayerWidget = new ChoosePlayerWidget(WIDTH, HEIGHT, this);
    gameWidget = new GameWidget(WIDTH, HEIGHT, this);

    int buttonWidth = 350;            // 按钮宽度
    int buttonHeight = 60;            // 按钮高度
    int firstButtonY = 420;           // 第一个按钮的Y坐标
    int buttonSpacing = buttonHeight; // 按钮间的垂直间距

    // 计算按钮居中时的X坐标
    int buttonsCenterX = (Window_Width - buttonWidth) / 2;

    // 设置按钮位置，使其居中
    startGameButton->setGeometry(buttonsCenterX, firstButtonY, buttonWidth, buttonHeight);
    choosePlayerButton->setGeometry(buttonsCenterX, firstButtonY + buttonSpacing, buttonWidth, buttonHeight);
    aboutButton->setGeometry(buttonsCenterX, firstButtonY + 2 * buttonSpacing, buttonWidth, buttonHeight);
    exitButton->setGeometry(buttonsCenterX, firstButtonY + 3 * buttonSpacing, buttonWidth, buttonHeight);

    // 连接信号和槽
    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::gameStart);
    connect(choosePlayerButton, &QPushButton::clicked, this, &MainWindow::showChoosePlayerWidget);
    connect(aboutButton, &QPushButton::clicked, this, &MainWindow::About);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitGame);

    choosePlayerWidget->hide();
    gameWidget->hide();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, menuPixmap.scaled(Window_Width, Window_Height)); // 绘制背景图

    // 设置初始字体
    painter.setPen(Qt::red);
    QFont font = painter.font();
    font.setFamily("宋体");
    font.setPointSize(12);

    {
        QFont tempFont = font;     // 复制当前设置
        tempFont.setPointSize(20); // 调整大小
        painter.setFont(tempFont); // 应用到painter

        // 绘制文本
        QRect rectSecondLine(0, height() / 64, width(), 40);
        painter.drawText(rectSecondLine, Qt::AlignCenter, tr("Made by HDU-CS BoneInscri"));
    }
}

void MainWindow::gameStart()
{
    gameWidget->show();
    gameWidget->gameInit();
    gameWidget->gameBegin();
}

void MainWindow::showChoosePlayerWidget()
{
    choosePlayerWidget->show();
}

void MainWindow::About()
{
    QMessageBox::about(this, tr("关于游戏"),
                       tr("<h2>迷宫闯关游戏 1.0</h2>"
                          "<p>版权 &copy; 2024 BoneInscri</p>"
                          "<p>游戏名称是一个经典的迷宫闯关游戏，每个关卡需要躲避怪兽在限制的时间内到达终点，并支持自动寻路躲避怪兽到达终点。 </p>"
                          "<p>本游戏由 HDU BoneInscri 开发，感谢您的支持！</p>"));
}
void MainWindow::exitGame()
{
    this->close();
}