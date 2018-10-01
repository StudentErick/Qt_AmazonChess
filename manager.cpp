#include "manager.h"

Manager::Manager(QObject *parent) : QObject(parent){
    m_moveItr=m_MoveList.begin();
    initBoard();
    m_GameMode=NoChoice;  // 初始化未选择状态
}

void Manager::previousMove(){
    if(m_MoveList.size()<=1){  // 第一步不能在往后走了
        emit sendMessage(QString(tr("已经是第一步了!")));
        return;
    }
    --m_moveItr;
    const auto& m=*m_moveItr;
    emit sendRetractMessage(m);  // 向外部发送信息

    // 局面数据结构的恢复
    m_nBoard[m.FromX][m.FromY]=m.side;
    m_nBoard[m.ToX][m.ToY]=EMPTY;
    m_nBoard[m.BarX][m.BarY]=EMPTY;

    QString str=QString("从(%1,%2)悔棋到(%3,%4),收回障碍(%5,%6)")
            .arg(m.ToX).arg(m.ToY)
            .arg(m.FromX).arg(m.FromY)
            .arg(m.BarX).arg(m.BarY);
    emit sendMessage(str);
}

void Manager::nextMove(){
    if(m_MoveList.empty()||m_moveItr==m_MoveList.end()-1){
        emit sendMessage(QString(tr("已经是最后一步了!")));
        return;
    }
    ++m_moveItr;
    const auto& m=*m_moveItr;
    emit sendMoveMessage(m);   // 向外部发送信息

    // 局面数据结构的处理
    m_nBoard[m.FromX][m.FromY]=EMPTY;
    m_nBoard[m.ToX][m.ToY]=m.side;
    m_nBoard[m.BarX][m.BarY]=BARRIER;

    QString str=QString("从(%1,%2)复原到(%3,%4),放置障碍(%5,%6)")
            .arg(m.FromX).arg(m.FromY)
            .arg(m.ToX).arg(m.ToY)
            .arg(m.BarX).arg(m.BarY);
    emit sendMessage(str);
}

void Manager::getMove(ChessMove move){
    m_getMove=move;
    if(m_GameMode==NoChoice){
        return;
    }else if(m_GameMode==PVP){
        PVPMode();
    }
}

void Manager::PVPMode(){  // 人人模式不用判断共线，因为点击的时候就确认了
    const auto& m=m_getMove;
    // 移动棋子
    m_nBoard[m.FromX][m.FromY]=EMPTY;
    m_nBoard[m.ToX][m.ToY]=m.side;
    m_nBoard[m.BarX][m.BarY]=BARRIER;
    int res=JudgeResult();
    if(res==EMPTY){  // 没有分出胜负
        emit sendNextSide(-m.side);
    }else if(res==BLACK){
        QString str=QString(tr("黑方获胜！"));
        emit sendMessage(str);
        emit sendNextSide(EMPTY);
    }else{
        QString str=QString(tr("白方获胜！"));
        emit sendMessage(str);
        emit sendNextSide(EMPTY);
    }
}

void Manager::PVP_BlackFirst(){
    m_GameMode=PVP;
    QString str=QString(tr("人人对弈模式，黑方先行！"));
    emit sendMessage(str);
    emit sendNextSide(BLACK);  // 向棋盘界面发送消息
    emit sendCheckable(true);  // 可以点击
}

void Manager::PVP_WhiteFirst(){
    m_GameMode=PVP;
    QString str=QString(tr("人人对弈模式，白方先行！"));
    emit sendMessage(str);
    emit sendNextSide(WHITE);
    emit sendCheckable(true);
}

void Manager::PVC_CFirst(){
    m_GameMode=PVC;
    emit sendNextSide(WHITE);  // 人机模式，计算机永远是黑子
    emit sendCheckable(false); // 计算机先走不能点
}

void Manager::PVC_PFirst(){
    m_GameMode=PVC;
    emit sendNextSide(WHITE);
    emit sendCheckable(true);  // 人先走可以点
}

void Manager::initBoard(){
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
    m_nBoard[9][3]=WHITE;m_nBoard[9][7]=WHITE;
}

void Manager::startGame(){
    if(m_GameMode==NoChoice){
        QString str=QString(tr("未选择游戏模式"));
        emit sendMessage(str);
        return;
    }else if(m_GameMode==PVP){
     //   PVPMode();
        sendCheckable(true);   // 可以点击
    }else if(m_GameMode==PVC){
     //   PVCMode();
    }else{
     //   CVCMode();
    }
}

int Manager::JudgeResult(){
    // 双方被困住的棋子的个数
    int nBlack=0,nWhite=0;
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            if(m_nBoard[i][j]!=EMPTY&&m_nBoard[i][j]!=BARRIER){
                int n=0;
                // 8个方向判别
                for(int k=0;k<8;++k){
                    int a=i+Offset[k][0];
                    int b=i+Offset[k][1];
                    if(a<0||a>9||b<0||b>9||m_nBoard[a][b]!=EMPTY){
                        ++n;
                    }
                }
                if(n>=8){  // 8个方向都堵死了
                    if(m_nBoard[i][j]==BLACK){
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

bool Manager::judgeOnline(int lx,int ly,int x,int y){
    if(lx==x&&ly==y){  // 不能是同一个位置
        return false;
    }else if(lx==x){   // 同一行
        int Max=std::max(ly,y);
        int Min=std::min(ly,y);
        for(int i=Min+1;i<Max;++i){  // 如果中间有棋子，就是非法的
            if(m_nBoard[lx][i]!=EMPTY){
                return false;
            }
        }
        return true;
    }else if(ly==y){   // 同一列
        int Max=std::max(lx,x);
        int Min=std::min(lx,x);
        for(int i=Min+1;i<Max;++i){  // 如果中间有棋子，就是非法的
            if(m_nBoard[i][ly]!=EMPTY){
                return false;
            }
        }
        return true;
    }else if(std::abs(ly-y)==std::abs(lx-x)){ // 共线的情况
        int MaxX=std::max(lx,x);
        int MinX=std::min(lx,x);
        int MaxY=std::max(ly,y);
        int MinY=std::min(ly,y);
        for(int i=MinX+1,j=MinY+1;i<MaxX&&j<MaxY;++i,++j){
            if(m_nBoard[i][j]!=EMPTY){ // 如果中间有棋子，就是非法的
                return false;
            }
        }
        return true;
    }else{
        return false;
    }
}
