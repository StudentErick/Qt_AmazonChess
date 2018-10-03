#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include <vector>
#include "utils.h"

/*
*  用于产生下一步所有可能步法的类，传入一个vector，把结果保存到里面
*  @author: Erick Lv
*  @time 2018/10/03
*/

class MoveGenerator
{
public:
    MoveGenerator();
    ~MoveGenerator();
    // 产生的走法可能情况，存储到vector中
    int CreatePossibleMove(int Board[10][10], int side,
                           std::vector<ChessMove> &MoveList);

   private:
    // 产生所有的障碍个数
    void CreatePossibleBar(int lx, int ly, int x, int y, int side,
                           std::vector<ChessMove> &MoveList);
    // 复制棋盘
    int m_Position[10][10];
    // 步法总数
    int m_Count;
};

#endif // MOVEGENERATOR_H
