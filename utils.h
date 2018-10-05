#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <cmath>
/*
* 下面是棋子基本的数据结构
* Erick Lv 09/30/2018
*/
const int BLACK=1;    // 黑子
const int WHITE=-1;   // 白子
const int BARRIER=2;  // 障碍
const int EMPTY=0;    // 空
const int COMPUTER_FIRST_STEP=-2;  // 计算机第一次走子的策略

// 初始化棋盘位置坐标
const int POSW=30;
const int POSH=30;

// 菜单栏的宽度
const int MENUBAR_WIDTH=20;

// 初始化对弈信息位置坐标
const int MSG_POSW=700;
const int MSG_POSH=30;

// 对弈信息显示的长宽
const int MSG_WIDTH=280;
const int MSG_HIGHT=500;

// 棋盘和棋子的边长
const int BOARD_SIZE=500;
const int CHESS_SIZE=50;

// 资源图片的路径
const QString BOARD_PATH=":/pic/myRes/board.bmp";
const QString BLACK_PATH=":/pic/myRes/black.bmp";
const QString WHITE_PATH=":/pic/myRes/white.bmp";
const QString BARRIER_PATH=":/pic/myRes/barrier.bmp";

// 一次移动的记录
struct ChessMove{
    int side;        // 走子方
    int FromX,FromY; // 取皇后的位置
    int ToX,ToY;     // 放皇后的位置
    int BarX,BarY;   // 放障碍的位置
};

// 封装判断共线的数据
struct LineMove{
    int lx,ly;
    int x,y;
};

// 对弈模式：人人、人机、机机、未选择
enum GameMode{PVP,PVC,CVC,NoChoice};

// 8个偏移方向
const int Offset[8][2]={{-1,-1},{-1,0},{-1,1},
                        {0,1},{0,-1},
                        {1,-1},{1,0},{1,1}};

// 无穷大数值
const double INF=100000000000.0;

#endif // BASICDATA_H
