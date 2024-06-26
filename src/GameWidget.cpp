#include "GameWidget.hpp"

GameWidget::GameWidget(int WIDTH, int HEIGHT, QWidget *parent) : QWidget(parent)
{
  Window_Width = WIDTH;
  Window_Height = HEIGHT;
  setFixedSize(WIDTH, HEIGHT);

  AutoButton = new QPushButton("自动寻路", this);
  PauseButton = new QPushButton("暂停游戏", this);
  ExitButton = new QPushButton("退出游戏", this);

  connect(AutoButton, &QPushButton::clicked, this, &GameWidget::gameAuto);
  connect(PauseButton, &QPushButton::clicked, this, &GameWidget::gamePause);
  connect(ExitButton, &QPushButton::clicked, this, &GameWidget::gameExit);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GameWidget::updateGameState); // 更新游戏状态
  countdown = new QTimer(this);
  connect(countdown, &QTimer::timeout, this, &GameWidget::TimeCountDown); // 游戏倒计时
  monstersTimer = new QTimer(this);
  connect(monstersTimer, &QTimer::timeout, this, &GameWidget::MonstersMove); // 手动操作 怪兽移动
  autoTimer = new QTimer(this);
  connect(autoTimer, &QTimer::timeout, this, &GameWidget::AutoMove); // 自动移动

  setFocusPolicy(Qt::StrongFocus);

  loadImages();
}

void GameWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // 设置背景颜色
  painter.fillRect(this->rect(), Qt::white);

  switch (gameState)
  {
  case GameIng:
  case GamePause:
  case GameAuto:
    gameDraw(painter);
    break;
  case GameOver:
    this->hide();
    break;
  case GameSuccess:
    Current_Level++;
    if (Current_Level > Level_NUM)
    {
      gameWin();
    }
    else
    {
      gameSuccess();
    }
    break;
  default:
    qDebug() << "Not tested Game State";
    exit(0);
    break;
  }

  painter.end();
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
  if (gameState != GameIng)
  {
    return;
  }
  switch (event->key())
  {
  case Qt::Key_W:
  case Qt::Key_Up:
    // 向上移动
    Player.dir = UP;
    break;
  case Qt::Key_S:
  case Qt::Key_Down:
    // 向下移动
    Player.dir = DOWN;
    break;
  case Qt::Key_A:
  case Qt::Key_Left:
    Player.dir = LEFT;
    // 向左移动
    break;
  case Qt::Key_D:
  case Qt::Key_Right:
    Player.dir = RIGHT;
    // 向右移动
    break;
  default:
    break;
  }
  MovePlayer();
}

void GameWidget::MonstersMove()
{
  for (int i = 0; i < m; i++)
  {
    Node next = MoveNode(Monsters[i]);

    switch (Mp[next.r][next.c])
    {
    case WALL:
    case MONSTER:
      Monsters[i].dir = getOppositeDirection(Monsters[i].dir);
      break;
    case SPACE:
      Mp[Monsters[i].r][Monsters[i].c] = SPACE;
      Mp[next.r][next.c] = MONSTER;
      Monsters[i] = next;
      break;
    case PLAYER:
      Mp[Monsters[i].r][Monsters[i].c] = SPACE;
      Player.r = Start.r;
      Player.c = Start.c;
      Mp[Player.r][Player.c] = PLAYER;
      Mp[next.r][next.c] = MONSTER;
      Monsters[i] = next;
      LifeReduce();
      break;
    }
  }
}

void GameWidget::LifeReduce()
{
  LIFE -= 1;
  if (!LIFE)
  {
    gameOver();
  }
}

void GameWidget::gameOver()
{
  gameState = GameOver;
  QMessageBox::about(this, tr("游戏结束"),
                     tr(""));
}

Node MoveNode(Node curr)
{
  static const int deltaR[] = {-1, 1, 0, 0};
  static const int deltaC[] = {0, 0, -1, 1};
  // 经典方向向量
  Node next = curr;
  next.r += deltaR[curr.dir];
  next.c += deltaC[curr.dir];
  return next;
}

int getOppositeDirection(int dir)
{
  switch (dir)
  {
  case UP:
    return DOWN;
  case DOWN:
    return UP;
  case LEFT:
    return RIGHT;
  case RIGHT:
    return LEFT;
  default:
    return dir;
  }
}

void GameWidget::MovePlayer()
{
  Node next = MoveNode(Player);

  switch (Mp[next.r][next.c])
  {
  case WALL:
    return;
  case SPACE:
    Mp[Player.r][Player.c] = SPACE;
    Mp[next.r][next.c] = PLAYER;
    Player = next;
    break;
  case EXIT:
    gameState = GameSuccess;
    break;
  }
  assert(next.r >= 0);
  assert(next.c >= 0);
}

void GameWidget::loadMap(const QString &fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Failed to open map file: " << fileName;
    return;
  }

  QTextStream in(&file);
  in >> ROW >> COL >> TIME;
  Time = TIME;
  WIDTH = GRID_SIZE * COL;
  HEIGHT = GRID_SIZE * ROW;

  // Check if reading ROW, COL, and TIME was successful
  if (in.status() != QTextStream::Ok)
  {
    qDebug() << "Failed to read ROW, COL, or TIME from map file: " << fileName;
    file.close();
    return;
  }

  for (int i = 0; i < ROW; ++i)
  {
    for (int j = 0; j < COL; ++j)
    {
      in >> Mp[i][j];
      if (in.status() != QTextStream::Ok)
      {
        qDebug() << "Failed to read map data from map file: " << fileName;
        file.close();
        return;
      }
      if (Mp[i][j] == PLAYER)
      {
        Player.r = i;
        Player.c = j;
        Start.r = i;
        Start.c = j;
      }
      if (Mp[i][j] == EXIT)
      {
        Exit.r = i;
        Exit.c = j;
      }
    }
  }

  in >> m;
  // Check if reading m was successful
  if (in.status() != QTextStream::Ok)
  {
    qDebug() << "Failed to read m from map file: " << fileName;
    file.close();
    return;
  }

  for (int i = 0; i < m; ++i)
  {
    in >> Monsters[i].r >> Monsters[i].c >> Monsters[i].dir;
    // Check if reading monster data was successful
    if (in.status() != QTextStream::Ok)
    {
      qDebug() << "Failed to read monster data from map file: " << fileName;
      file.close();
      return;
    }
  }

  // change GRID_SIZE
  int maxCellWidth = (Window_Width * 2 / 3) / COL;
  int maxCellHeight = Window_Height / ROW;
  GRID_SIZE = qMin(maxCellWidth, maxCellHeight);

  // generate random monsters
  srand(time(0));
  for (int i = 0; i < m; i++)
  {
    int x = rand() % 6 + 1;
    Monsters[i].type = x;
    // qDebug() << "monsters type :" << Monsters[i].type;
  }
  file.close();
}

void GameWidget::loadImages()
{
  const QString imgPath = "./img/game/";
  const QString numPath = "./img/num/";
  const QString monstersPath = "./img/monster/";

  // LIFE_IMAGE.load(imgPath + "life.png");
  KEY_IMAGE.load(imgPath + "key.png");
  START_IMAGE.load(imgPath + "start.png");
  END_IMAGE.load(imgPath + "end.png");

  for (int i = 0; i < 10; ++i)
  {
    NUMBERS_IMAGE[i].load(numPath + QString::number(i) + ".png");
  }

  for (int i = 1; i <= 6; ++i)
  {
    MONSTERS_IMAGE[i].load(monstersPath + "monster" + QString::number(i) + ".png");
  }
}

extern int Player_type_global;
void GameWidget::gameBegin()
{
  const QString playerPath = "./img/player/";
  const QString musicPath = "./music/";

  loadMap(mapFiles[Current_Level - 1]);

  Player.type = 1;
  Player.type = Player_type_global;
  // qDebug() << "Player type" << Player.type;
  bgmPlayer = new QMediaPlayer;

  loadBGM(musicPath + "BGM" + QString::number(Current_Level) + ".mp3");

  PLAYER_IMAGE.load(playerPath + "player" + QString::number(Player.type) + ".png");

  countdown->start(1000); // 设置定时器每1000毫秒（1秒）触发一次
  monstersTimer->start(MONSTER_SPEED);
}

void GameWidget::loadBGM(const QString &fileName)
{
  bgmPlayer->setMedia(QUrl::fromLocalFile(QFileInfo(fileName).absoluteFilePath())); // 一定要用绝对路径
  bgmPlayer->setVolume(50);
  bgmPlayer->play();
}

void GameWidget::gameDraw(QPainter &painter)
{
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(this->rect(), Qt::white);

  drawGameInfo(painter);

  drawGameMap(painter);
}

void GameWidget::updateGameState()
{
  update(); // 调用update()会触发paintEvent()重绘窗口
}

void GameWidget::TimeCountDown()
{
  Time--;
  if (Time <= 0)
  {
    countdown->stop();
    monstersTimer->stop();
    gameOver();
  }
}

void GameWidget::gamePause()
{
  if (gameState == GamePause)
  {
    gameState = GameIng;
    bgmPlayer->play();
    PauseButton->setText("暂停游戏");
    assert(countdown);
    countdown->start(1000);
    monstersTimer->start(MONSTER_SPEED);
  }
  else if (gameState == GameIng)
  {
    gameState = GamePause;
    bgmPlayer->pause();
    PauseButton->setText("继续游戏");
    assert(countdown);
    countdown->stop();
    monstersTimer->stop();
  }
}

void GameWidget::AutoFindWay()
{
  memset(To, 0, sizeof(To));

  // EXIT的位置初始位置
  int tx = Exit.r, ty = Exit.c;
  To[0][Player.r][Player.c] = 1;
  path_len = 0;

  static const int deltaR[] = {-1, 1, 0, 0};
  static const int deltaC[] = {0, 0, -1, 1};
  while (To[path_len][tx][ty] == 0)
  {
    // 四个方向移动
    for (int i = 0; i < ROW; i++)
    {
      for (int j = 0; j < COL; j++)
      {
        if (To[path_len][i][j] == 0)
        {
          // 不能走
          continue;
        }
        for (int dir = 0; dir < 4; ++dir)
        {
          int newRow = i + deltaR[dir];
          int newCol = j + deltaC[dir];
          if (Mp[newRow][newCol] != WALL)
          {
            To[path_len + 1][newRow][newCol] = 1;
          }
        }
      }
    }
    // 怪物存在的格子不能走
    for (int i = 0; i < m; ++i)
    {
      To[path_len + 1][Monsters_cpy[i].r][Monsters_cpy[i].c] = 0;
    }
    // 怪物移动
    for (int i = 0; i < m; ++i)
    {
      Node next = MoveNode(Monsters_cpy[i]);
      if (Mp[next.r][next.c] == WALL)
      {
        Monsters_cpy[i].dir = getOppositeDirection(next.dir);
      }
      else
      {
        Monsters_cpy[i] = next;
      }
    }
    // 怪物存在的格子不能走
    for (int i = 0; i < m; ++i)
    {
      To[path_len + 1][Monsters_cpy[i].r][Monsters_cpy[i].c] = 0;
    }

    ++path_len;
  }

  // get answer
  Node next;
  next.r = tx;
  next.c = ty;

  for (int j = path_len; j > 0; --j)
  {
    Road[j] = next;
    for (int dir = 0; dir < 4; ++dir)
    {
      int new_r = next.r + deltaR[dir];
      int new_c = next.c + deltaC[dir];
      if (To[j - 1][new_r][new_c] == 1)
      {
        next.r = new_r;
        next.c = new_c;
      }
    }
  }
  Road[0] = next;
  return;
}

// 从自动游戏恢复到手动操作
void GameWidget::gameContinue()
{
  autoTimer->stop();
  // restore game state
  LIFE = preLIFE;
  Time = preTime;
  Player = pre_Player;
  Mp[Player.r][Player.c] = PLAYER;
  // for (int i = 0; i < m; ++i)
  // {
  //   Mp[Monsters[i].r][Monsters[i].c] = SPACE;
  //   Monsters[i] = Monsters_cpy[i];
  //   Mp[Monsters[i].r][Monsters[i].c] = MONSTER;
  // }
  cur = 0;

  Mp[AutoPoint.r][AutoPoint.c] = SPACE;
  countdown->start(1000);
  monstersTimer->start(MONSTER_SPEED);

  AutoButton->setText("自动寻路");
  gameState = GameIng;
}

void GameWidget::gameAuto()
{
  if (gameState == GameIng)
  {
    // save game State
    preLIFE = LIFE;
    preTime = Time;
    pre_Player = Player;
    Mp[Player.r][Player.c] = SPACE;
    for (int i = 0; i < m; ++i)
    {
      Monsters_cpy[i] = Monsters[i];
    }

    countdown->stop();
    monstersTimer->stop();
    AutoFindWay();
    AutoPoint = Road[0];

    gameState = GameAuto; // 进入自动游戏状态

    autoTimer->start(100);
    AutoButton->setText("继续操作");
  }
  else if (gameState == GameAuto)
  {
    gameContinue();
  }
}

void GameWidget::AutoMove()
{
  // qDebug() << "AutoMove";
  assert(Mp[AutoPoint.r][AutoPoint.c] != MONSTER);
  Mp[AutoPoint.r][AutoPoint.c] = SPACE;
  AutoPoint = Road[cur++];
  Mp[AutoPoint.r][AutoPoint.c] = AUTO;
  MonstersMove();
  if (cur == path_len)
  {
    gameContinue();
  }
}

void GameWidget::setPlayerType(int type)
{
  Player.type = type;
}

void GameWidget::gameExit()
{
  gameState = NotStarted;
  bgmPlayer->pause();
  countdown->stop();
  monstersTimer->stop();
  this->hide();
}

void GameWidget::drawGameInfo(QPainter &painter)
{
  int WINDOW_WIDTH = Window_Width;
  int WINDOW_HEIGHT = Window_Height;
  int INFO_AREA_START_X = WINDOW_WIDTH * 2 / 3; // 信息区域的起始X坐标

  int yPos = 40; // 用于动态计算绘制项的Y坐标

  // 设置画笔和字体以适应更大的尺寸
  QPen pen(Qt::black, 2);
  painter.setPen(pen);
  QFont font("宋体", 24); // 增大字体大小
  painter.setFont(font);

  int centerAlign = WIDTH / 3 / 2;

  // 绘制关卡数
  painter.drawText(INFO_AREA_START_X + centerAlign, yPos, QString("第 %1 关").arg(Current_Level));
  yPos += 60; // 根据需要调整间距

  // 绘制生命值
  painter.drawText(INFO_AREA_START_X + 20, yPos, "当前生命值为：");
  yPos += 30;

  QString imagePath = "./img/game/life.png";
  QPixmap lifePixmap(imagePath);
  for (int i = 0; i < LIFE; ++i)
  {
    painter.drawPixmap(INFO_AREA_START_X + 20 + i * 70, yPos, lifePixmap);
  }

  yPos += 100;
  // 绘制剩余时间
  painter.drawText(INFO_AREA_START_X + 20, yPos, "剩余时间为：");
  yPos += 30;
  QString timeString = QString::number(Time);
  QSize targetSize(TIME_DIGIST_SIZE, TIME_DIGIST_SIZE);

  const QString numPath = "./img/num/";
  for (int i = 0; i < timeString.length(); ++i)
  {
    int digit = timeString.at(i).digitValue();
    imagePath = numPath + QString::number(digit) + ".png";
    QPixmap numPixmap(imagePath);
    painter.drawPixmap(INFO_AREA_START_X + centerAlign - 30 + i * 60, yPos, numPixmap.scaled(60, 60));
  }
  yPos += 40;

  // 绘制"自动寻路"和"游戏暂停"按钮
  const int buttonWidth = 100;
  const int buttonHeight = 60;
  yPos += 60;
  AutoButton->setGeometry(INFO_AREA_START_X + 20, yPos, buttonWidth, buttonHeight);
  PauseButton->setGeometry(INFO_AREA_START_X + 20 + 120, yPos, buttonWidth, buttonHeight);
  ExitButton->setGeometry(INFO_AREA_START_X + 20 + 240, yPos, buttonWidth, buttonHeight);
  yPos += 80;

  // 绘制游戏规则说明，支持自动换行
  QFont font_rules("宋体", 15);
  painter.setFont(font_rules);
  QRect rulesRect(INFO_AREA_START_X + 20, yPos, WINDOW_WIDTH / 3 - 40, WINDOW_HEIGHT - yPos); // 定义文本绘制区域
  QString rulesText =
      "游戏规则：\n"
      "1. 整局游戏你有5条命，碰到怪物生命值减1，并回到起点。\n"
      "2. 到达终点进入下一关。\n"
      "3. 使用方向键进行移动。\n"
      "4. 可以暂停、继续、退出、自动寻路。";
  painter.drawText(rulesRect, Qt::AlignLeft | Qt::TextWordWrap, rulesText);
  yPos += 200;

  // 绘制方向键图示
  QPixmap directionKeys = KEY_IMAGE.scaled(WIDTH / 3 + 60, HEIGHT / 8 + 60); // 示例，根据实际情况调整
  painter.drawPixmap(INFO_AREA_START_X + (WINDOW_WIDTH / 3 - directionKeys.width()) / 2, yPos, directionKeys);
}

void GameWidget::drawGameMap(QPainter &painter)
{
  QSize targetSize(GRID_SIZE - 2, GRID_SIZE - 2);
  // 绘制怪物
  for (int i = 0; i < m; ++i)
  {
    // 假设每个怪物在monsters数组中有r（行）和c（列）属性，以及type属性决定怪物的类型
    int x = Monsters[i].c * GRID_SIZE;
    int y = Monsters[i].r * GRID_SIZE;
    if (Monsters[i].type >= 1 && Monsters[i].type <= MONSTERS_MAX_NUM)
    { // 确保怪物类型有效
      painter.drawPixmap(QPoint(x, y), MONSTERS_IMAGE[Monsters[i].type].scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
      qDebug() << "Draw Monsters , Unknown Type : " << Monsters[i].type;
      // exit(0);
    }
  }

  // 绘制玩家、墙壁、起点和终点

  for (int i = 0; i < ROW; ++i)
  {
    for (int k = 0; k < COL; ++k)
    {
      QPoint pos(k * GRID_SIZE, i * GRID_SIZE);

      switch (Mp[i][k])
      {
      case SPACE:
        break;
      case WALL:
        painter.setBrush(QBrush(QColor(132, 132, 132)));
        painter.drawRect(k * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE);
        break;
      case PLAYER:
        assert(!PLAYER_IMAGE.isNull());
        painter.drawPixmap(pos, PLAYER_IMAGE.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        break;
      case EXIT:
        assert(!END_IMAGE.isNull());
        painter.drawPixmap(pos, END_IMAGE.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        break;
      case BEGIN:
        assert(!START_IMAGE.isNull());
        painter.drawPixmap(pos, START_IMAGE.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        break;
      case MONSTER:
        // has handled
        break;
      case AUTO:
        painter.setBrush(QBrush(QColor(255, 0, 0)));
        painter.drawRect(k * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE);
        break;
      default:
        qDebug() << "Draw Mp , Unknown Type : " << Mp[i][k];
        exit(0);
      }
    }
  }
}

void GameWidget::gameWin()
{
  QMessageBox::about(this, tr("游戏通关"),
                     tr("<h2>祝贺你！</h2>"));
  assert(bgmPlayer);
  assert(countdown);
  assert(monstersTimer);
  bgmPlayer->pause();
  countdown->stop();
  monstersTimer->stop();
  gameState = NotStarted;
  this->hide();
}

void GameWidget::gameInit()
{
  LIFE = 5;
  Current_Level = 1;
  gameState = GameState::GameIng;
  timer->start(16);
}

void GameWidget::gameSuccess()
{
  QMessageBox::about(this, tr("闯关成功"),
                     tr("<h2>点击进入下一关</h2>"));
  bgmPlayer->stop();
  gameState = GameState::GameIng;
  countdown->stop();
  monstersTimer->stop();
  gameBegin();
}