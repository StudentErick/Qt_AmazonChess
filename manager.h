#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <vector>
#include <QString>
#include <stack>
#include "utils.h"
/*
*  对弈逻辑控制的部分，处理所有的逻辑控制
*  包括选择人人、人机和联网对战，处理悔棋，局面初始化、先后手处理等
*  人机模式下，计算机永远是黑子
*  Erick Lv 09/30/2018
*/

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = nullptr);

signals:
    // 发送显示消息数据
    void sendMessage(QString str);
    // 发送走子的数据，给界面
    void sendMoveMsgToBoard(ChessMove move);
    // 发送走子的数据，给AI
    void sendMoveMsgToAI(ChessMove move);
    // 发送走子的数据，给网络用户
    void sendMoveMsgToNet(ChessMove move);
    // 发送悔棋数据
    void sendRetractMessage(ChessMove move);
    // 发送下一步走子的一方，如果side是EMPTY，说明结束了
    void sendNextSide(int side);
    // 初始化棋盘
    void sendInitBoard();
    // 设计界面是否可以点击
    void sendCheckable(bool flag);
    // 发送引擎编号
    void sendEngineNumber(int num);
public slots:
    // 前一步
    void previousMove();
    // 后一步
    void nextMove();
    // 添加新的走法
    void getMove(ChessMove move);
    // 对弈开始
    void startGame();
    // 人人对弈，黑方先手
    void PVP_BlackFirst();
    // 人人对弈，白方先手
    void PVP_WhiteFirst();
    // 人机对弈，计算机先手
    void PVC_CFirst();
    // 人机对弈，人先手
    void PVC_PFirst();
    // 初始化局面函数
    void initBoard();
    // 获取引擎的类型
    void getEngineNumber(int num);
private:
    // 判断共线函数，这里和界面的那个重复了，后期需要改进结构
    bool judgeOnline(int lx,int ly,int x,int y);
    // 人人模式
    void PVPMode();
    // 人机模式
    void PVCMode();
    // 联机模式
    void CVCMode()=delete;

    // 判断胜负，返回获胜一方的颜色
    int JudgeResult();

    int m_side;   // 判断第一步应该是哪一方走

    // 获取的步法
    ChessMove m_getMove;
    // 对弈模式
    GameMode m_GameMode;

    // 存储步法的栈
    std::stack<ChessMove>m_newMove;
    std::stack<ChessMove>m_pastMove;

    // 对弈的棋盘
    int m_nBoard[10][10];

    // AI引擎的类型
    int m_EngineNumber;
};

#endif // MANAGER_H
