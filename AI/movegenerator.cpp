#include "movegenerator.h"

MoveGenerator::MoveGenerator()
{
    m_Count=0;
}

MoveGenerator::~MoveGenerator(){

}

int MoveGenerator::CreatePossibleMove(int Board[10][10], int side,std::vector<ChessMove>& MoveList) {
    memcpy(m_Position, Board, 10 * 10 * sizeof(int));
    m_Count = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (m_Position[i][j] == side) {
                int k = i, l = j;
                // 遍历8个方向
                for (int t = 0; t < 8; ++t) {
                    k += Offset[t][0];
                    l += Offset[t][1];
                    while (k >= 0 && k < 10 && l >= 0 && l < 10 &&
                           m_Position[k][l] == EMPTY) {
                        CreatePossibleBar(i, j, k, l, side, MoveList);
                        k += Offset[t][0];
                        l += Offset[t][1];
                    }
                    k = i;
                    l = j;
                }
            }
        }
    }
    return m_Count;
}

void MoveGenerator::CreatePossibleBar(int lx, int ly, int x, int y, int side,
                                      std::vector<ChessMove>& MoveList) {
    // 模拟走皇后
    m_Position[lx][ly] = EMPTY;
    m_Position[x][y] = side;
    int k = x, l = y;
    // 遍历8个方向
    for (int t = 0; t < 8; ++t) {
        k += Offset[t][0];
        l += Offset[t][1];
        while (k >= 0 && k < 10 && l >= 0 && l < 10 && m_Position[k][l] == EMPTY) {
            ChessMove cm;
            cm.FromX = lx;
            cm.FromY = ly;
            cm.ToX=x;
            cm.ToY=y;
            cm.BarX=k;
            cm.BarY=l;
            cm.side=side;
            // 加入步法队列
            MoveList.emplace_back(cm);

            ++m_Count;
            k += Offset[t][0];
            l += Offset[t][1];
        }
        k = x;
        l = y;
    }
    // 恢复模拟前的状态
    m_Position[x][y] = EMPTY;
    m_Position[lx][ly] = side;
}
