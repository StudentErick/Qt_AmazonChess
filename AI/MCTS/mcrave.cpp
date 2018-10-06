#include "mcrave.h"
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cassert>
#include <QDebug>

/*
* 算法的架构仍然存在一些问题，按理说展开节点孩子的操作等应该都是在TreePolicy中进行的，
* 而在这里却放到了DefaultPolicy中；下一步的任务是按照严格按照论文给出的思路进行执行。
*/

// 调试输出
void TestN(int Board[10][10]){
    int b=0,w=0;
    for(int i=0;i<10;++i){
        for(int j=0;j<10;++j){
            if(Board[i][j]==BLACK){
                ++b;
            }
            if(Board[i][j]==WHITE){
                ++w;
            }
        }
    }
    assert(b!=4);
    assert(w!=4);
}

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
    root=std::make_shared<MCTSNode>(nullptr,m_nBoard,BLACK,m_Const);
    // 拓展根节点
    int n=m_MoveGenerator->CreatePossibleMove(m_nBoard,BLACK,root->UnTriedMove);

    if(n==0){
        /*******************************************************
         * 如果要移植算法，需要在这里添加传来终止局面的处理方法 *
         *******************************************************/
    }

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
    return node->pChild[index];
}

std::shared_ptr<MCTSNode> MCRave::TreePolicy(std::shared_ptr<MCTSNode> node){
    // 不是终止状态
    while(JudgeResult(node->state)==EMPTY){
        ++node->n_visits;  // 节点访问次数+1，回溯时就不用再+1了
        if(!node->UnTriedMove.empty()){  // 有未拓展过的节点
            using type=std::vector<ChessMove>::size_type;
            int n=static_cast<int>(node->UnTriedMove.size());
            n=rand()%n;
            auto mv=node->UnTriedMove[static_cast<type>(n)];  // 选择随机的一步
            node->UnTriedMove.erase(node->UnTriedMove.begin()+n); // 清空原来的
            MakeMove(node->state,mv);
            // 加入到末尾，注意转换走子的一方
            node->pChild.emplace_back(std::make_shared<MCTSNode>(node, node->state,-node->side,node->nUct));
            ++m_nCount;  // 记录搜索的节点个数
            UnMakeMove(node->state,mv);
            return node->pChild.back();  // 返回未访问的节点
        }else if(node->pChild.empty()){
            int n=m_MoveGenerator->CreatePossibleMove(node->state,node->side,node->UnTriedMove);
            if(n==0){    // 已经是局面的终止状态了
                node->isFullyExpanded=true;  // 完全展开
                break;
            }
        }else{
            bool flag=true;
            for(const auto& child:node->pChild){
                flag=flag&&child->isFullyExpanded;
            }
            if(flag||node->pChild.empty()){  // 完全拓展过的节点不用继续向下搜索
                node->isFullyExpanded=true;
                return node;
            }
            node=getBestChild(node);  // 继续寻找最佳孩子
        }
    }
    return node;
}

int MCRave::DefaultPolicy(std::shared_ptr<MCTSNode> node){
    if(node->isFullyExpanded){
        return node->leaf_value;
    }
    int Board[10][10];
    memcpy(Board,node->state,10*10*sizeof (int));  // 复制局面状态
    std::vector<ChessMove>MoveList;
    int res=JudgeResult(Board);
    if(res!=EMPTY){
        node->isFullyExpanded=true;
    }
    int side=node->side;
    using type=std::vector<ChessMove>::size_type;
    while(res==EMPTY){
        int n=m_MoveGenerator->CreatePossibleMove(Board,side,MoveList);
        n=rand()%n;
        const auto& mv=MoveList[static_cast<type>(n)];   // 随机走出一步
        MakeMove(Board,mv);
        MoveList.clear();  // 清空
        res=JudgeResult(Board);
        side=-side;  // 转化走子方
    }
    //   DebugBoard(Board);
    return res;
}

void MCRave::BackUp(std::shared_ptr<MCTSNode>&node, int leaf_value){
    while(node->pParent.lock()!=nullptr){
        // 非根节点
        node->Qsa=(leaf_value-node->Qsa)/(node->n_visits);
        node->u=node->Qsa+
                node->nUct*std::sqrt(std::log(node->pParent.lock()->n_visits)/(1+node->n_visits));
        node->leaf_value=leaf_value;
        leaf_value=-leaf_value;
        node=node->pParent.lock();
    }
}
