#include "mcrave.h"
#include <algorithm>
#include <random>
#include <cstring>
#include <cmath>

MCRave::MCRave(double contant)
{
    m_Const=contant;  // 设置扩展常数
}

ChessMove MCRave::getBestMove(const ChessMove& mv){
    m_move=mv;
    MakeMove(mv);  // 走出传来的步法
    root=std::make_shared<MCTSNode>(mv,nullptr);  // 构造根节点
    TreePolicy();
    m_bestMove=root->pChild[0]->mv;
    root.reset(); // 销毁搜索树
    return m_bestMove;
}


void MCRave::TreePolicy(){
    std::vector<ChessMove>MoveList;
    auto node=root;
    int side=BLACK;
    // 自定义的堆比较函数
    auto comp=[](const std::shared_ptr<MCTSNode>&n1,std::shared_ptr<MCTSNode>&n2){
        return *n1<*n2;
    };
    while (m_searchFlag) {
        std::shared_ptr<MCTSNode>bestChild;
        if(node->is_firstVisited){  // 第一次访问全部进行展开
            node->is_firstVisited=false;
            m_MoveGenerator->CreatePossibleMove(m_nBoard,side,MoveList);
            for(const auto& mv:MoveList){  // 构建所有的孩子节点
                node->pChild.emplace_back(std::make_shared<MCTSNode>(mv,node));
            }
            bestChild=node->pChild[0];  // 获取最佳的孩子
            MoveList.erase(MoveList.begin(),MoveList.end());  // 清空步法队列
            std::make_heap(node->pChild.begin(),node->pChild.end(),comp);  // 建堆
        }else{
            bestChild=node->pChild[0];  // 获取最佳的孩子
            std::pop_heap(node->pChild.begin(),node->pChild.end(),comp);  // 弹出
            node->pChild.erase(node->pChild.end()-1);  // 从堆中彻底删除这个元素
        }
        MakeMove(bestChild->mv);
        ++bestChild->Ns;  // 增加访问次数
        bestChild->is_leaf=DefaultPolicy(bestChild);  // Default policy
        // UnMakeMove(bestChild->mv);///////////////////////////////////////////////////////////////////////////////////////////////////////
        std::push_heap(node->pChild.begin(),node->pChild.end(),comp);  // 更新后的孩子重新加入堆中
        if(bestChild->is_leaf){  // 搜索到叶子就终止
            break;  // 值的回溯可能还要在这里处理。。。。。
        }else{
            node=bestChild;   // 更新当前节点为最佳孩子
        }
    }
    side=-side;  // 交换走子方
    node=root;
}

bool MCRave::DefaultPolicy(std::shared_ptr<MCTSNode>node){
    int res=JudgeResult(m_nBoard);
    if(res!=EMPTY){  // 传入的是叶子节点
        if(res==BLACK){
            node->value=1;
        }else{
            node->value=-1;
        }
        BackUp(node);
        return true;
    }

    int Board[10][10];
    memcpy(Board,m_nBoard,10*10*sizeof (int));  // 复制棋盘状态

    // 随机走出一步
    std::mt19937 rng;
    rng.seed(std::random_device()());

    std::vector<ChessMove>MoveList;
    int side=node->mv.side;
    while (res==EMPTY) {
        int n=m_MoveGenerator->CreatePossibleMove(m_nBoard,side,MoveList);
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, static_cast<unsigned long>(n));
        n=static_cast<int>(dist6(rng));
        auto m=MoveList[static_cast<std::vector<ChessMove>::size_type>(n)];  // 获取随机的一步
        // 走出一步
        Board[m.FromX][m.FromY]=EMPTY;
        Board[m.ToX][m.ToY]=side;
        Board[m.BarX][m.BarY]=BARRIER;
        side=-side;
        res=JudgeResult(Board);
        if(res==BLACK){
            node->value=1.0;
        }else if(res==WHITE){
            node->value=-1.0;
        }
    }
    BackUp(node);   // 回溯分数
    return false;
}

void MCRave::BackUp(std::shared_ptr<MCTSNode>leaf){
    int side=leaf->mv.side;
    if(side==BLACK){
        leaf->value=1.0;
    }else{
        leaf->value=-1.0;
    }
    UnMakeMove(leaf->mv);   // 恢复局面
    auto p=leaf->pParent.lock();
    double value=-leaf->value;
    while(p!=nullptr){
        if(p->pParent.lock()!=nullptr){  // 不是根节点
            int N=p->pParent.lock()->Ns;
            p->value=(value-p->Qsa)/p->Ns+
                    m_Const*std::sqrt(std::log(1.0*N)/(1+p->Ns));
        }else{  // 根节点
             p->value=(value-p->Qsa)/p->Ns;
        }
        value=-value;
        UnMakeMove(p->mv);  // 恢复局面
    }
}
