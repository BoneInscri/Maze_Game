#ifndef _GAMEWIDGET_H_
#define _GAMEWIDGET_H_

#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QMediaPlayer>
#include <QFileInfo>
#include <QTimer>

enum CellType
{
  SPACE,   // 0
  WALL,    // 1
  PLAYER,  // 2
  EXIT,    // 3
  MONSTER, // 4
  BEGIN    // 5
};

enum DIR
{
  UP,    // 0
  DOWN,  // 1
  LEFT,  // 2
  RIGHT, // 3
};

enum GameState
{
  NotStarted,    // 还未开始 0
  InGame,        // 正在游戏 1
  LevelComplete, // 游戏过关 2
  GameOver       // 游戏失败 3
};

struct Node
{
  int r, c;
  int dir;
  int type;
};

Node MoveNode(Node curr);
int getOppositeDirection(int dir);

class GameWidget : public QWidget
{
  Q_OBJECT

public:
  explicit GameWidget(QWidget *parent = nullptr);
  GameState gameState = NotStarted; // 游戏状态
  void gameBegin();
  void setPlayerType(int type);

protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  void loadMap(const QString &fileName);
  void loadImages();
  void loadBGM(const QString &fileName);
  void MonstersMove();
  void MovePlayer();
  void LifeReduce();
  void gameOver();
  // void checkCollisions();
  // void drawGame();

  static const int MAP_MAX_SIZE = 2000;
  static const int MONSTERS_MAX_NUM = 10;
  static const int GRID_SIZE = 25; // 每一个方格的大小
  static const int Level_NUM = 2;  // 关卡数
  const QString mapFiles[2] = {"./map/map1.txt", "./map/map2.txt"};

  int Mp[MAP_MAX_SIZE][MAP_MAX_SIZE]; // 地图矩阵
  int LIFE = 5;                       // 动态生命值
  int m;                              // 怪物数量
  int ROW, COL;                       // 行与列
  int WIDTH, HEIGHT;                  // 像素的宽与高
  int TIME;                           // 起始时间值
  int Time;                           // 动态时间值
  int Current_Level = 1;              // 当前关卡

  Node Player;
  Node Pre_Player;
  Node Start;
  Node Monsters[MONSTERS_MAX_NUM];


  QPixmap LIFE_IMAGE;        // life image
  QPixmap NUMBERS_IMAGE[10]; // 十个数字图片
  QPixmap MONSTERS_IMAGE[7]; // 怪物图片
  QPixmap PLAYER_IMAGE;      // 角色图片
  QPixmap KEY_IMAGE;         // 键盘提示
  QPixmap START_IMAGE;       // 起点的图片
  QPixmap END_IMAGE;         // 终点的图片

  QMediaPlayer *bgmPlayer;
  QTimer *timer;

private slots:
  void updateGameState();

  // int preLIFE;
  // int preTime;   // 自动前的时间和生命值
  // int flag = 1;  // 打开游戏后的状态

  //   void gameOver();
  //   void gamePerfect();
  //   void gameWin();
  //   void FindRoad();
  //   void Autogame();
};

#endif // _GAMEWIDGET_H_