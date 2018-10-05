#ifndef BASICENGINE_H
#define BASICENGINE_H

#include "utils.h"
#include "movegenerator.h"

/*
*   这是一个虚基类，所有的搜索引擎都必须继承这个类，该类提供了统一的接口
*   @author: Erick Lv
*   @time: 2018/10/03
*/

class BasicEngine
{
public:
    BasicEngine();
    virtual ~BasicEngine();
    // 传出最佳走法，这是对外接口
    virtual ChessMove getBestMove(const ChessMove& mv)=0;
    // 设置继续搜索或者停止
    void setSearchFlag(bool flag){m_searchFlag=flag;}
protected:

    // 调试棋盘用
    void DebugBoard();

    void MakeMove(int Board[10][10],const ChessMove& mv);
    void UnMakeMove(int Board[10][10],const ChessMove& mv);

    // 判断结果，返回获胜的一方或者EMPTY
    int JudgeResult(int Board[10][10]);

    // 步法生成器
    MoveGenerator* m_MoveGenerator;
    // 是否可以继续搜索的标志
    bool m_searchFlag;
    // 最佳步法
    ChessMove m_bestMove;
    // 搜索引擎真正使用到的棋盘结构
    int m_nBoard[10][10];
};

#endif // BASICENGINE_H
