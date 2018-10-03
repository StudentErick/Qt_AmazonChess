#include "basicengine.h"
#include <QDebug>

BasicEngine::BasicEngine()
{
    // 一定要记得初始化棋盘！！！！！！！
    // 初始化局面
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            m_nBoard[i][j]=EMPTY;
        }
    }
    // 初始化皇后的位置
    m_nBoard[0][3]=BLACK;m_nBoard[0][6]=BLACK;
    m_nBoard[3][0]=BLACK;m_nBoard[3][9]=BLACK;
    m_nBoard[6][0]=WHITE;m_nBoard[6][9]=WHITE;
    m_nBoard[9][3]=WHITE;m_nBoard[9][6]=WHITE;
}

BasicEngine::~BasicEngine(){

}

void BasicEngine::MakeMove(const ChessMove& mv){
    m_nBoard[mv.FromX][mv.FromY]=EMPTY;
    m_nBoard[mv.ToX][mv.ToY]=mv.side;
    m_nBoard[mv.BarX][mv.BarY]=BARRIER;
}

void BasicEngine::UnMakeMove(const ChessMove& mv){
    m_nBoard[mv.FromX][mv.FromY]=mv.side;
    m_nBoard[mv.ToX][mv.ToY]=EMPTY;
    m_nBoard[mv.BarX][mv.BarY]=EMPTY;
}

void BasicEngine::DebugBoard(){
    QString str;
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            str+=(QString::number(m_nBoard[i][j],10)+" ");
        }
        qDebug()<<str;
        str.clear();
    }
}
