#include "GameWidget.hpp"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
  setFixedSize(600, 600);
  loadImages();
  loadMap(mapFiles[0]);

  bgmPlayer = new QMediaPlayer;
}

void GameWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.fillRect(rect(), Qt::white);
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
  }

  for (int i = 1; i <= 6; ++i)
  {
    MONSTERS_IMAGE[i].load(monstersPath + "Monster" + QString::number(i) + ".png");
  }
}

void GameWidget::gameBegin()
{
  loadBGM("./music/BGM1.mp3");

  // 开始游戏
}

void GameWidget::loadBGM(const QString &fileName)
{
  bgmPlayer->setMedia(QUrl::fromLocalFile(QFileInfo(fileName).absoluteFilePath()));// 一定要用绝对路径
  bgmPlayer->setVolume(50);
  bgmPlayer->play();

}