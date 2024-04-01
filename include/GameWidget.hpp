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
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>

enum CellType
{
  SPACE,   // 0
  WALL,    // 1
  PLAYER,  // 2
  EXIT,    // 3
  MONSTER, // 4
  BEGIN,   // 5
  AUTO     // 6
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
  NotStarted, // 还未开始 0
  GameIng,    // 正在游戏 1
  GamePause,  // 游戏暂停 2
  GameOver,   // 游戏失败 3
  GameAuto,   // 自动寻路 4
  GameSuccess // 闯关成功 5
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
  explicit GameWidget(int WIDTH, int HEIGHT, QWidget *parent = nullptr);
  GameState gameState = NotStarted; // 游戏状态
  void gameInit();
  void gameBegin();
  void setPlayerType(int type);

protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  int Window_Width, Window_Height;
  void loadMap(const QString &fileName);
  void loadImages();
  void loadBGM(const QString &fileName);
  void MovePlayer();
  void LifeReduce();
  void gameOver();
  void gameDraw(QPainter &painter);
  void drawGameInfo(QPainter &painter);
  void drawGameMap(QPainter &painter);
  void gameAuto();
  void gamePause();
  void gameExit();
  void gameWin();
  void gameSuccess();
  void AutoFindWay();
  void AutoMove();
  void gameContinue();

  QPushButton *AutoButton;
  QPushButton *PauseButton;
  QPushButton *ExitButton;

  static const int MAP_MAX_SIZE = 2000;
  static const int MONSTERS_MAX_NUM = 10;
  static const int LIFE_IMAGE_SIZE = 60;
  static const int TIME_DIGIST_SIZE = 60;
  static const int MONSTER_SPEED = 250;
  static const int Level_NUM = 2; // 关卡数

  const QString mapFiles[2] = {"./map/map1.txt", "./map/map2.txt"};

  int Mp[MAP_MAX_SIZE][MAP_MAX_SIZE]; // 地图矩阵
  int LIFE;                           // 动态生命值
  int m;                              // 怪物数量
  int ROW, COL;                       // 行与列
  int WIDTH, HEIGHT;                  // 像素的宽与高
  int GRID_SIZE = 30;                 // 每一个方格的大小，根据地图的大小可以动态调节，让左侧的2/3区域填满迷宫地图
  int TIME;                           // 起始时间值
  int Time;                           // 动态时间值
  int Current_Level;                  // 当前关卡
  int cur = 1;

  int preLIFE; // 自动寻路前的生命值
  int preTime; // 自动寻路前的时间

  Node Player;
  Node pre_Player;
  Node Start;
  Node Exit;
  Node Monsters[MONSTERS_MAX_NUM];

  // QPixmap LIFE_IMAGE;        // life image
  QPixmap NUMBERS_IMAGE[10]; // 十个数字图片
  QPixmap MONSTERS_IMAGE[7]; // 怪物图片
  QPixmap PLAYER_IMAGE;      // 角色图片
  QPixmap KEY_IMAGE;         // 键盘提示
  QPixmap START_IMAGE;       // 起点的图片
  QPixmap END_IMAGE;         // 终点的图片

  QMediaPlayer *bgmPlayer;
  QTimer *timer;
  QTimer *countdown;
  QTimer *monstersTimer;
  QTimer *autoTimer; // 自动寻路定时器

  // Auto game
  int To[35 * 35][35][35];             // DP数组
  int path_len;                        // 路径长度
  Node Monsters_cpy[MONSTERS_MAX_NUM]; // 复制一群怪物（先找到一条路径）
  Node Road[35 * 35];                  // 记录最优解的路径，从0开始
  Node AutoPoint;

private slots:
  void updateGameState();
  void TimeCountDown();
  void MonstersMove();
};

#endif // _GAMEWIDGET_H_