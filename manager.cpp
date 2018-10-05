#include "manager.h"
#include <QDebug>

Manager::Manager(QObject *parent) : QObject(parent){
    initBoard();
    m_GameMode=NoChoice;  // 初始化未选择状态
    m_EngineNumber=0;
}

void Manager::previousMove(){
    if(m_newMove.empty()){
        emit sendMessage(QString(tr("已经是第一步了!")));
        return;
    }

    auto m=m_newMove.top();
    m_newMove.pop();
    m_pastMove.push(m);  // 悔棋的放到第二个栈中

    if(m_GameMode==PVP){
        emit sendMoveMsgToBoard(m); // 向界面发送即可
    }
    emit sendRetractMessage(m);     // 向外部发送信息

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
    if(m_pastMove.empty()){
        emit sendMessage(QString(tr("已经是最后一步了!")));
        return;
    }
    auto m=m_pastMove.top();
    m_pastMove.pop();
    m_newMove.push(m);  // 放到之前的栈中

    if(m_GameMode==PVP||m_GameMode==PVC){
        emit sendMoveMsgToBoard(m); // 向界面发送即可
    }


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
    }else if(m_GameMode==PVC){
        PVCMode();
    }
}

void Manager::PVPMode(){
    // 人人模式不用判断共线，因为点击的时候就确认了
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
    m_newMove.push(m);
}

void Manager::PVCMode(){
    // 人机模式也不用判断共线
    const auto& m=m_getMove;
    // 移动棋子
    m_nBoard[m.FromX][m.FromY]=EMPTY;
    m_nBoard[m.ToX][m.ToY]=m.side;
    m_nBoard[m.BarX][m.BarY]=BARRIER;

    m_newMove.push(m);  // 添加到步法队列

    int res=JudgeResult();  // 判别是否分出胜负
    if(res==EMPTY){
        if(m.side==BLACK){  // 收到AI发来的步法
            emit sendMoveMsgToBoard(m);  // 通知界面进行移动
            emit sendNextSide(WHITE);         // 通知界面轮到白子走
            emit sendCheckable(true);          // 通知界面可以进行点击
            QString str=QString(tr("计算机从位置(%1,%2)移动到(%3,%4)，放置障碍(%5,%6)")).
                    arg(QString::number(m.FromX,10)).arg(QString::number(m.FromY,10))
                    .arg(QString::number(m.ToX,10)).arg(QString::number(m.ToY,10))
                    .arg(QString::number(m.BarX,10)).arg(QString::number(m.BarY,10));
            emit sendMessage(str);
        }else if(m.side==WHITE){  // 收到人发来的步法
            emit sendNextSide(BLACK);     // 轮到黑方走
            emit sendCheckable(false);      // AI 计算时不能点击
            emit sendMoveMsgToAI(m);     // 给AI计算
        }
    }else if(res==BLACK){
        emit sendMoveMsgToBoard(m);  // 通知界面进行移动
        QString str=QString(tr("计算机获胜！"));
        emit sendMessage(str);
        emit sendNextSide(EMPTY);
    }else{
        emit sendMoveMsgToBoard(m);  // 通知界面进行移动
        QString str=QString(tr("人获胜！"));
        emit sendMessage(str);
        emit sendNextSide(EMPTY);
    }
}

void Manager::PVP_BlackFirst(){
    m_GameMode=PVP;
    m_side=BLACK;
    QString str=QString(tr("人人对弈模式，黑方先行！"));
    emit sendMessage(str);
    emit sendNextSide(m_side);  // 向棋盘界面发送消息
}

void Manager::PVP_WhiteFirst(){
    m_GameMode=PVP;
    m_side=WHITE;
    QString str=QString(tr("人人对弈模式，白方先行！"));
    emit sendMessage(str);
    emit sendNextSide(m_side);
}

void Manager::PVC_CFirst(){
    if(m_EngineNumber==0){
        QString str=QString(tr("请先选择引擎！"));
        emit sendMessage(str);
        return;
    }
    m_GameMode=PVC;
    QString str=QString(tr("人机对弈模式，计算机先行！"));
    emit sendMessage(str);
    m_side=BLACK;
    emit sendNextSide(m_side);  // 人机模式，计算机永远是黑子
    emit sendCheckable(false); // 不能点击棋盘操作
}

void Manager::PVC_PFirst(){
    if(m_EngineNumber==0){
        QString str=QString(tr("请先选择引擎！"));
        emit sendMessage(str);
        return;
    }
    m_GameMode=PVC;
    QString str=QString(tr("人机对弈模式，人先行！"));
    emit sendMessage(str);
    m_side=WHITE;
    emit sendNextSide(m_side);  // 人机模式，计算机永远是黑子
    emit sendCheckable(true);  // 可以点击棋盘
}

void Manager::getStopAI(){
    if(m_GameMode==PVP){
        emit sendMessage(tr("当前是PVP模式，终止操作无效！"));
        return;
    }
    else{
        emit sendMessage(tr("停止操作成功！"));
        emit sendAIStop();  // 发送停止信号
    }
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
    m_nBoard[9][3]=WHITE;m_nBoard[9][6]=WHITE;

    while(!m_newMove.empty()){
        m_newMove.pop();
    }
    while(!m_pastMove.empty()){
        m_pastMove.pop();
    }
    emit sendInitBoard(); // 发射初始化信号
    emit sendOnGame(false);  // 发射结束信号
    m_EngineNumber=0;  // 引擎重置
    QString  str=QString(tr("本局结束！"));
    emit sendMessage(str);
}

void Manager::startGame(){
    if(m_GameMode==NoChoice){
        QString str=QString(tr("未选择对弈模式！"));
        emit sendMessage(str);
        return;
    }else if(m_GameMode==PVP){
        sendCheckable(true);   // 可以点击
        QString str=QString(tr("对弈开始！"));
        emit sendMessage(str);
    }else if(m_GameMode==PVC){
        if(m_side==BLACK){
            sendCheckable(false);
            ChessMove mv;
            // 计算机第一步走子标记
            mv.side=COMPUTER_FIRST_STEP;
            QString str=QString(tr("对弈开始！计算机先行"));
            emit sendMessage(str);
            sendMoveMsgToAI(mv);
        }else{
            sendCheckable(true);
            QString str=QString(tr("对弈开始！人先行"));
            emit sendMessage(str);
        }
    }else{

    }
    emit sendOnGame(true);  // 开始对弈
}

void Manager::getTestEngine(){
    m_EngineNumber=1;
    emit sendEngineNumber(m_EngineNumber);
    QString str=QString(tr("选中测试引擎！"));
    emit sendMessage(str);
}

void Manager::getMCRaveEngine(){
    m_EngineNumber=2;
    emit sendEngineNumber(m_EngineNumber);
    QString str=QString(tr("选中MCRave引擎！"));
    emit sendMessage(str);
}

int Manager::JudgeResult(){
    // 双方被困住的棋子的个数
    int nBlack=0,nWhite=0;
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            if(m_nBoard[i][j]==BLACK||m_nBoard[i][j]==WHITE){
                int n=0;
                // 8个方向判别
                for(int k=0;k<8;++k){
                    int a=i+Offset[k][0];
                    int b=j+Offset[k][1];
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

void Manager::DebugBoard(){
    QString str;
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            str+=(QString::number(m_nBoard[i][j],10)+" ");
        }
        qDebug()<<str;
        str.clear();
    }
}

