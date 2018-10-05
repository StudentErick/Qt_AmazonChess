#ifndef MCRAVE_H
#define MCRAVE_H

#include <vector>
#include <queue>
#include "utils.h"
#include "AI/basicengine.h"
#include "AI/treenode.h"

/*
* MCRave搜索算法，第一个UCT测试案例，具体参考论文：
* "Monte-Carlo Tree Search and Rapid Action Value Estimation in Computer Go" (Algorithms1)
* 区别在于这里使用了负极大搜索，黑方获胜返回1，白方获胜返回-1，其余更改参照代码
* @author: Erick Lv
* @time: 2018/10/04
*/
class MCRave:public BasicEngine
{
public:
    MCRave(double constant=1.414);
    ChessMove getBestMove(const ChessMove& mv);
protected:

    // Tree policy，按照评估一直搜寻到叶子
    void TreePolicy();
    // default policy，扩展叶子，如果当前状态是叶子，直接返回true
    void DefaultPolicy(std::shared_ptr<MCTSNode>node, int side);
    // 回溯分数，使用负极大的方式
    void BackUp(std::shared_ptr<MCTSNode>node,int leaf_value);

    ChessMove m_move;  // 保存传来的走法
    double m_Const;         // 扩展的常数
    std::shared_ptr<MCTSNode>root;  // 根节点
};

#endif // MCRAVE_H
