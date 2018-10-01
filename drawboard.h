#ifndef DRAWBOARD_H
#define DRAWBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QString>
#include "utils.h"
/*
* DrawBoard类用于绘制棋盘，同时处理己方点击棋盘的事件。
* 点击进行走子的时候，需要进行简单的逻辑判断
* Erick Lv 09/30/2018
*/
class DrawBoard : public QWidget
{
    Q_OBJECT
public:
    explicit DrawBoard(QWidget *parent = nullptr);

signals:
    // 用于向外界发送走子的策略
    void sendMove(ChessMove move);
    // 向外界发送当前的点击信息
    void sendMessage(QString msg);

public slots:
    // 还原状态
    void initBoard();
    // 设置当前的走子方
    void setSide(int player);
    // 走棋
    void makeMove(const ChessMove& chessMove);
    // 悔棋
    void retractMove(const ChessMove& chessMove);
    // 设置是否可以点击
    void setCheckable(bool flag);

    // QWidget interface
protected:
    // 重写鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override;

private:
    // 共线的判断函数
    bool judgeOnline(int lx,int ly,int x,int y);

    // 绘制棋盘
    void drawBoard();

    // 是否响应鼠标点击
    bool m_checkable;

    // 棋盘的图像和标签
    QPixmap m_boardPic;
    QLabel m_board;

    // 棋子的图像和标签
    QLabel m_nchessBlack[4];
    QPixmap m_nchessBlackPic[4];
    QLabel m_nchessWhite[4];
    QPixmap m_nchessWhitePic[4];
    QLabel m_nchessBarrier[92];
    QPixmap m_nchessBarrierPic[92];

    // 当前使用到第几个图片的标记
    int m_blackN;
    int m_whiteN;
    int m_barrierN;

    // 棋盘的数据结构
    int m_nBoard[10][10];
    // 当前的走子方
    int m_side;
    // 当前应当执行的操作  0 取棋子，1 放棋子，2 放障碍
    int m_turn;

    // 保留点击的坐标
    int m_FromX,m_FromY,m_ToX,m_ToY,m_BarX,m_BarY;
    // 存储整个点击流程，用于向外界发送
    ChessMove m_chessMove;
};

#endif // DRAWBOARD_H
