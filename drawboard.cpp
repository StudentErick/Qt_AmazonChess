#include "drawboard.h"
#include <QDebug>

DrawBoard::DrawBoard(QWidget *parent) : QWidget(parent)
{
    m_side=BLACK;  // 走棋的一方，初始化空的
    m_turn=0;      // 应当取棋子
    m_PVC=true;    // 默认人机对战
    // 设置初始化位置
    this->setGeometry(POSW,POSH,BOARD_SIZE,BOARD_SIZE);
    // 初始化局面
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            m_nBoard[i][j]=EMPTY;
        }
    }
    m_nBoard[0][3]=BLACK;m_nBoard[0][6]=BLACK;
    m_nBoard[3][0]=BLACK;m_nBoard[3][9]=BLACK;
    m_nBoard[6][0]=WHITE;m_nBoard[6][9]=WHITE;
    m_nBoard[9][3]=WHITE;m_nBoard[9][7]=WHITE;

    // 加载棋盘的图片
    m_board.setParent(this);
    m_board.setGeometry(0,0,BOARD_SIZE,BOARD_SIZE);
    m_boardPic.load(BOARD_PATH);
    m_boardPic.scaled(BOARD_SIZE,BOARD_SIZE);
    m_board.setPixmap(m_boardPic);

    // 4个黑子和白子
    for(int i=0;i<4;++i){
        m_nchessBlack[i].setParent(this);
        m_nchessBlackPic[i].load(BLACK_PATH);
        m_nchessBlackPic[i].scaled(CHESS_SIZE,CHESS_SIZE);

        m_nchessWhite[i].setParent(this);
        m_nchessWhitePic[i].load(WHITE_PATH);
        m_nchessWhitePic[i].scaled(CHESS_SIZE,CHESS_SIZE);
    }
    // 最多92个障碍
    for(int i=0;i<92;++i){
        m_nchessBarrier[i].setParent(this);
        m_nchessBarrierPic[i].load(BARRIER_PATH);
        m_nchessBarrierPic[i].scaled(CHESS_SIZE,CHESS_SIZE);
    }
    // 绘制棋盘
    drawBoard();
}

void DrawBoard::drawBoard(){
    m_blackN=0;
    m_whiteN=0;
    m_barrierN=0;
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            if(m_nBoard[i][j]==EMPTY){  // 空的不放置任何东西
                continue;
            }
            // 注意下面的i与j是正好相反的
            if(m_nBoard[i][j]==BLACK){ // 黑子
                m_nchessBlack[m_blackN].setGeometry(CHESS_SIZE*j,CHESS_SIZE*i,CHESS_SIZE,CHESS_SIZE);
                m_nchessBlack[m_blackN].setPixmap(m_nchessBlackPic[m_blackN]);
                m_blackN++;
            }else if(m_nBoard[i][j]==WHITE){  // 白子
                m_nchessWhite[m_whiteN].setGeometry(CHESS_SIZE*j,CHESS_SIZE*i,CHESS_SIZE,CHESS_SIZE);
                m_nchessWhite[m_whiteN].setPixmap(m_nchessWhitePic[m_whiteN]);
                m_whiteN++;
            }else{  // 障碍
                m_nchessBarrier[m_barrierN].setGeometry(CHESS_SIZE*j,CHESS_SIZE*i,CHESS_SIZE,CHESS_SIZE);
                m_nchessBarrier[m_barrierN].setPixmap(m_nchessBarrierPic[m_barrierN]);
                m_barrierN++;
            }
        }
    }

}

void DrawBoard::refreshBoard(const ChessMove& chessMove){
    int side=m_nBoard[chessMove.FromX][chessMove.FromY];
    m_nBoard[chessMove.FromX][chessMove.FromY]=EMPTY;
    m_nBoard[chessMove.ToX][chessMove.ToY]=side;
    m_nBoard[chessMove.BarX][chessMove.BarY]=BARRIER;
    drawBoard();  // 重新绘制棋盘
}

void DrawBoard::mousePressEvent(QMouseEvent *event){
    // 不是当前行棋方或者不是人机对战，点击无效
    if(m_side==EMPTY||m_PVC==false){
        return;
    }
    if(event->button()==Qt::LeftButton){
        auto x=event->localPos().x();
        auto y=event->localPos().y();
        // 注意屏幕坐标与数据坐标是反着的
        int ix=static_cast<int>(y)/CHESS_SIZE;
        int iy=static_cast<int>(x)/CHESS_SIZE;
        if(m_turn==0){  // 取走皇后
            if(m_nBoard[ix][iy]==m_side){
                m_FromX=ix;
                m_FromY=iy;
                m_nBoard[m_FromX][m_FromY]=EMPTY;
                m_turn=1;
                QString str(tr("选中己方皇后"));
                emit sendMessage(str);
            }else{
                QString str(tr("未选中己方的棋子！"));
                emit sendMessage(str);  // 向外部发送消息
            }
        }else if(m_turn==1){ // 放置皇后
            m_ToX=ix;
            m_ToY=iy;
            if(m_nBoard[ix][iy]!=EMPTY&&m_nBoard[ix][iy]!=m_side){
                QString str(tr("当前位置不是空的！"));
                emit sendMessage(str);
                return;
            }
            if(m_nBoard[ix][iy]==m_side){  // 如果有重新点击己方皇后，相当于重新选择
                m_nBoard[m_FromX][m_FromY]=m_side;  // 别忘记恢复
                m_FromX=ix;
                m_FromY=iy;
                m_nBoard[m_FromX][m_FromY]=EMPTY;   // 重新置空
                QString str(tr("选中己方皇后！"));
                emit sendMessage(str);
                m_turn=1;
                return;
            }
            if(judgeOnline(m_FromX,m_FromY,m_ToX,m_ToY)){
                m_nBoard[m_ToX][m_ToY]=m_side;
                m_turn=2;
                drawBoard();
            }else{
                QString str(tr("放置皇后的方式非法！"));
                emit sendMessage(str);
                return;
            }
        }else if(m_turn==2){  // 放置障碍
            m_BarX=ix;
            m_BarY=iy;
            if(m_nBoard[ix][iy]!=EMPTY){
                QString str(tr("当前位置不是空的！"));
                emit sendMessage(str);
                return;
            }
            if(judgeOnline(m_ToX,m_ToY,m_BarX,m_BarY)){
                m_nBoard[ix][iy]=BARRIER;
                drawBoard();
                m_turn=0;
                m_side=-m_side;   // 应该是对面走了///////////////////////////////////////
                drawBoard();
                // 获取用户完整的一步走法，并且通过信号发送出去
                m_chessMove.side=m_side;
                m_chessMove.FromX=m_FromX;
                m_chessMove.FromY=m_FromY;
                m_chessMove.ToX=m_ToX;
                m_chessMove.ToY=m_ToY;
                m_chessMove.BarX=m_BarX;
                m_chessMove.BarY=m_BarY;
                QString str=QString(tr("从位置(%1,%2)移动到(%3,%4)，放置障碍(%5,%6)")).
                        arg(QString::number(m_FromX,10)).arg(QString::number(m_FromY,10))
                        .arg(QString::number(m_ToX,10)).arg(QString::number(m_ToY,10))
                        .arg(QString::number(m_BarX,10)).arg(QString::number(m_BarX,10));
                emit sendMessage(str);
                emit sendMove(m_chessMove);  // 发送走棋的数据
            }else{
                QString str(tr("放置障碍的方式非法！"));
                emit  sendMessage(str);
                return;
            }
        }
    }
}

void DrawBoard::setMove(int side){
    m_side=side;
}

void DrawBoard::setPVC(bool flag){
    m_PVC=flag;
}

bool DrawBoard::judgeOnline(int lx,int ly,int x,int y){
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
