#include "testengine.h"

TestEngine::TestEngine()
{
}

TestEngine::~TestEngine(){
}

ChessMove TestEngine::getBestMove(const ChessMove& mv){
    ChessMove m=mv;
    if(m.side!=COMPUTER_FIRST_STEP){
        MakeMove(m_nBoard,m);     // 走出传来的步法
    }else{
        m.side=WHITE;
    }

    DebugBoard(m_nBoard);

    std::vector<ChessMove>m_MoveList;

    // 注意负号，计算机永远是黑色的，传来的步法永远是白色的
    int n=m_MoveGenerator->CreatePossibleMove(m_nBoard,-m.side,m_MoveList);

    // 随机走出一步
    n=rand()%n;
    m=m_MoveList[static_cast<std::vector<ChessMove>::size_type>(n)];  // 获取随机的一步

    MakeMove(m_nBoard,m);    // 这里一定要走出一步！！！！！
    m_MoveList.erase(m_MoveList.begin(),m_MoveList.end());  // 清空之前的数据
    return m;
}

