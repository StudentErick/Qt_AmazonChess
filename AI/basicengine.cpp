#include "basicengine.h"
#include <QDebug>

BasicEngine::BasicEngine()
{
    m_MoveGenerator=new MoveGenerator;

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
    if(m_MoveGenerator!=nullptr){
        delete m_MoveGenerator;
        delete m_MoveGenerator;
    }
}

void BasicEngine::MakeMove(int Board[10][10],const ChessMove& mv){
    Board[mv.FromX][mv.FromY]=EMPTY;
    Board[mv.ToX][mv.ToY]=mv.side;
    Board[mv.BarX][mv.BarY]=BARRIER;
}

void BasicEngine::UnMakeMove(int Board[10][10],const ChessMove& mv){
    Board[mv.FromX][mv.FromY]=mv.side;
    Board[mv.ToX][mv.ToY]=EMPTY;
    Board[mv.BarX][mv.BarY]=EMPTY;
}

void BasicEngine::DebugBoard(int Board[10][10]){
    QString str;
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            str+=(QString::number(Board[i][j],10)+",");
        }
        qDebug()<<str;
        str.clear();
    }
}

int BasicEngine::JudgeResult(int Board[10][10]){
    // 双方被困住的棋子的个数
    int nBlack=0,nWhite=0;
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            if(Board[i][j]==BLACK||Board[i][j]==WHITE){
                int n=0;
                // 8个方向判别
                for(int k=0;k<8;++k){
                    int a=i+Offset[k][0];
                    int b=j+Offset[k][1];
                    if(a<0||a>9||b<0||b>9||Board[a][b]!=EMPTY){
                        ++n;
                    }
                }
                if(n>=8){  // 8个方向都堵死了
                    if(Board[i][j]==BLACK){
                        ++nBlack;
                    }else{
                        ++nWhite;
                    }
                }
            }
        }
    }
    if(nBlack>=4){
        return WHITE;
    }else if(nWhite>=4){
        return BLACK;
    }else{
        return EMPTY;
    }
}
