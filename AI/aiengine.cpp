#include "aiengine.h"
#include "testengine.h"
#include "MCTS/mcrave.h"

AIEngine::AIEngine()
{
    m_basciEngine=nullptr;
}

AIEngine::~AIEngine(){
    if(m_basciEngine!=nullptr){
        delete m_basciEngine;
        m_basciEngine=nullptr;
    }
}

void AIEngine::getEngineNumber(int num){
    m_EngineNum=num;
    if(m_basciEngine!=nullptr){  // 先删除之前的引擎
        delete m_basciEngine;
        m_basciEngine=nullptr;
    }

    /*************************************************************
     * 在这里重新加载需要的引擎，后期所有的引擎都要在这里进行扩充 *
    **************************************************************/
    switch (m_EngineNum) {
    case 1:
        m_basciEngine=new TestEngine();
        break;
    case 2:
        m_basciEngine=new MCRave();
        break;
    default:
        m_basciEngine=nullptr;
    }
}

void AIEngine::getMove(ChessMove chessmove){
    m_getMove=chessmove;  // 获取步法
    run();   // 在这里进行运算
}

void AIEngine::run(){
    auto m=m_basciEngine->getBestMove(m_getMove);
    emit sendResult(m);         // 发送搜索结果
}
