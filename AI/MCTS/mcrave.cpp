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
    MakeMove(m_nBoard,mv);  // 走出传来的步法
    // 构造根节点
    root=std::make_shared<MCTSNode>(m_nBoard,m_Const);
    root.reset(); // 销毁搜索树
    return m_bestMove;
}


void MCRave::TreePolicy(){
    int side=m_move.side;  // 当前走子方
    std::vector<ChessMove>MoveList;
    auto curBoard=m_nBoard;  // 获取当前局面
    auto curNode=root;
    while(m_searchFlag){
        ++curNode->n_visits;  // 访问次数+1
        if(curNode->is_leaf()){
            m_MoveGenerator->CreatePossibleMove(curBoard,side,curNode->UnTriedMove);
            DefaultPolicy(curNode,side); // 只能对叶子节点进行default policy操作
        }else{
            // 有没拓展的节点
            if(! curNode->UnTriedMove.empty()){
                auto mv=curNode->UnTriedMove.back();
                curNode->UnTriedMove.erase(curNode->UnTriedMove.end()-1);  // 删除
                MakeMove(curBoard,mv);  // 模拟走出一步
                curNode->pChild.emplace_back(std::make_shared<MCTSNode>(curBoard,m_Const));
                UnMakeMove(curBoard,mv); // 恢复状态
            }
            else{
                auto index=static_cast<std::vector<MCTSNode>::size_type>(0);
                // 寻找最佳的孩子
                std::for_each(curNode->pChild.begin(),curNode->pChild.end(),[&index,&curNode](const auto& p){
                    if(p->value>curNode->pChild[index]->value){
                        index++;
                    }
                });
                curNode=curNode->pChild[index];  // 下降
                curBoard=curNode->state;  // 保存状态
                side=-side;  // 交换走子方
            }
        }
    }
}

void MCRave::DefaultPolicy(std::shared_ptr<MCTSNode>node, int side){
    int res=JudgeResult(node->state);  // 是否是最终节点
    int Board[10][10];
    memcpy(Board,node->state,10*10*sizeof (int));  // 拷贝棋盘状态
    while(res!=EMPTY){

        std::vector<ChessMove>MoveList;
        int n=m_MoveGenerator->CreatePossibleMove(node->state,side,MoveList);  // 模拟步法
        MoveList.clear();  // 清空
        // 随机走出一步
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, static_cast<unsigned long>(n));
        n=static_cast<int>(dist6(rng));
        auto m=MoveList[static_cast<std::vector<ChessMove>::size_type>(n)];  // 获取随机的一步

        MakeMove(Board,m);    // 模拟向下走
        res=JudgeResult(Board);
    }
    BackUp(node,res);  // 回溯
}

void MCRave::BackUp(std::shared_ptr<MCTSNode>node,int leaf_value){
    while(node->pParent.lock()!=nullptr){
        node->Qsa=(leaf_value-node->Qsa)/node->n_visits;
        node->u=node->nUct*std::sqrt(std::log(node->pParent.lock()->n_visits)/(1+node->n_visits));
        node=node->pParent.lock();
        leaf_value=-leaf_value;
    }
}
