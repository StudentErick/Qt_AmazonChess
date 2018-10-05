#include "mcrave.h"
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cassert>
#include <QDebug>

MCRave::MCRave(double contant)
{
    m_Const=contant;  // 设置扩展常数
    m_timeDiff=5.0;     // 5秒的时间限制
    m_searchFlag=true;
}

ChessMove MCRave::getBestMove(const ChessMove& mv){
    m_move=mv;
    if(mv.side!=COMPUTER_FIRST_STEP){
        MakeMove(m_nBoard,mv);  // 走出传来的步法
    }
    // 构造根节点
    root=std::make_shared<MCTSNode>(m_nBoard,BLACK,m_Const);
    DefaultPolicy(root);// 根节点至少一次rollout out 操作
    decltype (root) bestChild;
    time(&startTime);  // 开始计时
    while(m_searchFlag){
        if(root->isFullyExpanded){ // 完全展开的根节点直接获取结果即可
            bestChild=getBestChild(root);
            break;
        }
        auto leaf=TreePolicy(root);
        int value=DefaultPolicy(leaf);
        BackUp(leaf,value);
        time(&endTime);
        if(difftime(endTime,startTime)>m_timeDiff){
            break;
        }
    }

    bestChild=getBestChild(root);

    // 从前后的状态差计算出一次移动
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            if(m_nBoard[i][j]==BLACK&&bestChild->state[i][j]!=BLACK){
                m_bestMove.FromX=i;
                m_bestMove.FromY=j;
            }else if(m_nBoard[i][j]!=BLACK&&bestChild->state[i][j]==BLACK){
                m_bestMove.ToX=i;
                m_bestMove.ToY=j;
            }else if(m_nBoard[i][j]!=BARRIER&&bestChild->state[i][j]==BARRIER){
                m_bestMove.BarX=i;
                m_bestMove.BarY=j;
            }
        }
    }

    root.reset(); // 销毁搜索树
    m_bestMove.side=BLACK;
    MakeMove(m_nBoard,m_bestMove);  // 别忘了走出
    return m_bestMove;
}

std::shared_ptr<MCTSNode>& MCRave::getBestChild(std::shared_ptr<MCTSNode>&node){
    auto index=static_cast<std::vector<MCTSNode>::size_type>(0);
    auto len=node->pChild.size();
    auto i=static_cast<std::vector<MCTSNode>::size_type>(0);
    // 寻找最佳的孩子
    for(i=0;i<len;++i){
        if(node->pChild[i]->value>node->pChild[index]->value){
            index=i;
        }
    }
    ++node->pChild[index]->n_visits;   // 节点访问次数+1，回溯时就不用再+1了
    return node->pChild[index];
}

std::shared_ptr<MCTSNode> MCRave::TreePolicy(std::shared_ptr<MCTSNode>& node){
    ++node->n_visits;  // 节点访问次数+1，回溯时就不用再+1了
    if(! node->UnTriedMove.empty()){  // 返回未展开的节点，总是从末尾取出
        auto mv=node->UnTriedMove.back();
        node->UnTriedMove.erase(node->UnTriedMove.end()-1);  // 删除
        MakeMove(node->state,mv);  // 模拟走出
        node->pChild.emplace_back(std::make_shared<MCTSNode>(node->state,-node->side,node->nUct));
        UnMakeMove(node->state,mv);  // 恢复状态
        ++node->pChild.back()->n_visits;
        return node->pChild.back();  // 返回没有探索的节点
    }else{
        auto curNode=node;
        bool flag=true;
        // 判断当前节点是否是完全展开的
        for(const auto& child:node->pChild){
            flag=(flag&child->isFullyExpanded);
        }
        if(flag){ // 完全展开的节点没有往下搜索的必要了
            curNode->isFullyExpanded=true;
            return getBestChild(curNode);
        }
        while(!curNode->is_leaf()){  //一直搜寻到叶子节点
            curNode=getBestChild(curNode);
            if(curNode->isFullyExpanded){
                return curNode;
            }
        }
        return curNode;
    }
}

int MCRave::DefaultPolicy(std::shared_ptr<MCTSNode>& node){
    decltype (node->state) Board;
    memcpy(Board,node->state,10*10*sizeof (int));
    int n=m_MoveGenerator->CreatePossibleMove(Board,node->side,node->UnTriedMove);

    int res=JudgeResult(Board);
    if(n==0){  // 已经是最终状态了
        node->isFullyExpanded=true;  // 彻底的展开状态，不需要后续搜索了
        return res;
    }
    using type=std::vector<ChessMove>::size_type;
    n=rand()%n;   // 获取随机的一步
    auto mv=node->UnTriedMove[static_cast<type>(n)];  // 获取随机的一步
    std::vector<ChessMove>MoveList;
    int side=node->side;
    while(res==EMPTY){
        MakeMove(Board,mv);
        res=JudgeResult(Board);
        if(res!=EMPTY){
            return res;
        }
        n=m_MoveGenerator->CreatePossibleMove(Board,side,MoveList);
        n=rand()%n;  // 获取随机一步
        mv=MoveList[static_cast<type>(n)];  // 获取随机的一步
        MoveList.clear();
        side=-side;  // 转换次序
    }
    return res;
}

void MCRave::BackUp(std::shared_ptr<MCTSNode>&node, int leaf_value){
    while(node->pParent.lock()!=nullptr){
        // 非根节点
        node->Qsa=(leaf_value-node->Qsa)/node->n_visits;
        node->u=node->Qsa+
                node->nUct*std::sqrt(std::log(node->pParent.lock()->n_visits)/node->n_visits);
        leaf_value=-leaf_value;
        node=node->pParent.lock();
    }
}
