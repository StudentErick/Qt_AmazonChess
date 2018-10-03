#include "testengine.h"
#include <random>

TestEngine::TestEngine()
{
    m_MoveGenerator=new MoveGenerator;
}

TestEngine::~TestEngine(){
    if(m_MoveGenerator!=nullptr){
        delete m_MoveGenerator;
        m_MoveGenerator=nullptr;
    }
}

ChessMove TestEngine::getBestMove(const ChessMove& mv){
    ChessMove m=mv;
    if(m.side!=COMPUTER_FIRST_STEP){
        MakeMove(m);     // 走出传来的步法
    }else{
        m.side=WHITE;
    }

    // 注意负号，计算机永远是黑色的，传来的步法永远是白色的
    int n=m_MoveGenerator->CreatePossibleMove(m_nBoard,-m.side,m_MoveList);

    // 随机走出一步
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, static_cast<unsigned long>(n));
    n=static_cast<int>(dist6(rng));
    m=m_MoveList[static_cast<std::vector<ChessMove>::size_type>(n)];  // 获取随机的一步

    MakeMove(m);    // 这里一定要走出一步！！！！！
    m_MoveList.erase(m_MoveList.begin(),m_MoveList.end());  // 清空之前的数据
    return m;
}

