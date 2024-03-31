#include "GameWidget.hpp"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
  setFixedSize(600, 600);
  loadImages();

  loadMap(mapFiles[0]);

  bgmPlayer = new QMediaPlayer;
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GameWidget::updateGameState);
  timer->start(16); // 假设我们希望每16ms更新一次游戏状态，约等于60FPS
  Player.type = 1;
}

void GameWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.fillRect(rect(), Qt::white);
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
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
    QWidget::keyPressEvent(event);
  }
  update();
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
    gameState = GameOver; // 游戏结束
    gameOver();
  }
}

void GameWidget::gameOver()
{
  QMessageBox::about(this, tr("游戏结束"),
                     tr("<h2>5条命已经全部用完</h2>"));
}

Node MoveNode(Node curr)
{
  static const int deltaR[] = {-1, 0, 1, 0}; // 上, 右, 下, 左 的行变化
  static const int deltaC[] = {0, 1, 0, -1}; // 上, 右, 下, 左 的列变化

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
    gameState = LevelComplete;
    break;
  }
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

  file.close();
}

void GameWidget::loadImages()
{
  const QString imgPath = "./img/game/";
  const QString numPath = "./img/num/";
  const QString monstersPath = "./img/monster/";

  LIFE_IMAGE.load(imgPath + "life.png");
  KEY_IMAGE.load(imgPath + "key.png");
  END_IMAGE.load(imgPath + "end.png");
  START_IMAGE.load(imgPath + "start.png");

  for (int i = 0; i < 10; ++i)
  {
    NUMBERS_IMAGE[i].load(numPath + QString::number(i) + ".png");
    NUMBERS_IMAGE[i] = NUMBERS_IMAGE[i].scaled(45, 62);
  }

  for (int i = 1; i <= 6; ++i)
  {
    MONSTERS_IMAGE[i].load(monstersPath + "Monster" + QString::number(i) + ".png");
  }

  srand(time(0));
  for (int i = 0; i < m; i++)
  {
    int x = rand() % 6 + 1;
    Monsters[i].type = x;
  }

}

extern int Player_type_global;
void GameWidget::gameBegin()
{
  loadBGM("./music/BGM1.mp3");
  Player.type = Player_type_global;
  qDebug() << "Player type" << Player.type;
}

void GameWidget::loadBGM(const QString &fileName)
{
  bgmPlayer->setMedia(QUrl::fromLocalFile(QFileInfo(fileName).absoluteFilePath())); // 一定要用绝对路径
  bgmPlayer->setVolume(50);
  bgmPlayer->play();
}

void GameWidget::updateGameState()
{
  update(); // 调用update()会触发paintEvent()重绘窗口
}

void GameWidget::setPlayerType(int type)
{
  Player.type = type;
}