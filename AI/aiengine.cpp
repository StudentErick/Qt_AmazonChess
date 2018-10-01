#include "./AI/aiengine.h"

AIEngine::AIEngine(QObject *parent) : QObject(parent)
{

}

void AIEngine::getEngineNumber(int num){
    m_EngineNum=num;
}

void AIEngine::getMove(ChessMove chessmove){
    m_getMove=chessmove;
}
