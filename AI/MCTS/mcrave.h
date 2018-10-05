#ifndef MCRAVE_H
#define MCRAVE_H

#include <vector>
#include <queue>
#include <ctime>
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

    // Tree policy，返回最佳的叶子或者未经扩展的叶子
    std::shared_ptr<MCTSNode> TreePolicy(std::shared_ptr<MCTSNode>& node);
    // default policy，对未经扩展的叶子进行rollout out，返回评估分数1,0,-1
    int DefaultPolicy(std::shared_ptr<MCTSNode>& node);
    // 回溯分数，使用负极大的方式
    void BackUp(std::shared_ptr<MCTSNode>&node, int leaf_value);
    // 获取最佳的孩子节点
    std::shared_ptr<MCTSNode>& getBestChild(std::shared_ptr<MCTSNode>& node);

    ChessMove m_move;  // 保存传来的走法
    double m_Const;         // 扩展的常数
    std::shared_ptr<MCTSNode>root;  // 根节点

    clock_t startTime;
    clock_t endTime;
    double m_timeDiff;    // 搜索的时间限制

};

#endif // MCRAVE_H
