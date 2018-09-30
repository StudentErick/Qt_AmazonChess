#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <cmath>
/*
* 下面是棋子基本的数据结构
*/
const int BLACK=1;
const int WHITE=-1;
const int BARRIER=2;
const int EMPTY=0;

// 初始化棋盘位置坐标
const int POSW=30;
const int POSH=30;

// 初始化对弈信息位置坐标
const int MSG_POSW=700;
const int MSG_POSH=10;

// 对弈信息显示的长宽
const int MSG_WIDTH=280;
const int MSG_HIGHT=800;

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

#endif // BASICDATA_H
