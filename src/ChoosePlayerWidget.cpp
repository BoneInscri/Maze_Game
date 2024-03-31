#include "ChoosePlayerWidget.hpp"

const char* ChoosePlayerWidget::name[] = {"黄菊之臣", "火焰猫灵", "绿风之叶", "狂暴拳手", "勇猛剑士", "激光枪手"};

ChoosePlayerWidget::ChoosePlayerWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(600, 600);
    setupUI();
}

void ChoosePlayerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
}

void ChoosePlayerWidget::onPrevClicked()
{
    currentItem = (currentItem - 1 + itemCount) % itemCount;
    updatePlayerImage();
}

void ChoosePlayerWidget::onNextClicked()
{
    currentItem = (currentItem + 1) % itemCount;
    updatePlayerImage();
}

void ChoosePlayerWidget::setupUI()
{

    // 创建按钮并设置其固定大小
    prevButton = new QPushButton("<", this);
    nextButton = new QPushButton(">", this);
    selectButton = new QPushButton("Select", this);
    int buttonWidth = 100;
    int buttonHeight = 40;
    prevButton->setFixedSize(buttonWidth, buttonHeight);
    nextButton->setFixedSize(buttonWidth, buttonHeight);
    selectButton->setFixedSize(150, buttonHeight);

    // 使用布局管理器定位按钮
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(prevButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(selectButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(nextButton);

    // 创建用于显示当前角色的QLabel
    playerLabel = new QLabel(this);
    playerLabel->setAlignment(Qt::AlignCenter);

    // 角色的name
    QFont nameFont("Arial", 16, QFont::Bold);
    nameLabel = new QLabel(this);
    nameLabel->setFont(nameFont);
    nameLabel->setAlignment(Qt::AlignCenter);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(playerLabel);
    mainLayout->setSpacing(100);
    mainLayout->addLayout(buttonsLayout);    

    setLayout(mainLayout);

    // 连接按钮的点击信号到相应的槽函数
    connect(prevButton, &QPushButton::clicked, this, &ChoosePlayerWidget::onPrevClicked);
    connect(nextButton, &QPushButton::clicked, this, &ChoosePlayerWidget::onNextClicked);
    connect(selectButton, &QPushButton::clicked, this, &ChoosePlayerWidget::finalizeSelection);

    updatePlayerImage();

}

void ChoosePlayerWidget::updatePlayerImage()
{
    // Load the current player's image
    QString imagePath = QString("./img/player/player%1.png").arg(currentItem + 1);
    if (this->playerPixmap.load(imagePath))
    {
        playerLabel->setPixmap(this->playerPixmap.scaled(310, 270, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        printf("can't find the player%d.png", currentItem);
        exit(1);
    }
    nameLabel->setText(QString::fromUtf8(name[currentItem]));
}

extern int Player_type_global;
void ChoosePlayerWidget::finalizeSelection()
{
    emit characterChosen(currentItem);
    this->hide();
    Player_type_global = currentItem + 1;
}
