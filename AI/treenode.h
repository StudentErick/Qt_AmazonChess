#ifndef TREENODE_H
#define TREENODE_H

#include <memory>
#include <vector>
#include <cstring>
#include "utils.h"
/*
* 这个文件定义了搜索树节点的数据结构，
* 自定义的新节点类型也在这里添加
* @author: Erick Lv
* @time: 2018/10/04
*/

// 蒙特卡洛搜索树的节点，只要使用了MC大类的搜索算法都可以用这个
struct MCTSNode{
    int n_visits;     // 访问的次数
    double Qsa;    // 评估分数，基于MC的
    double u;        // 先验概率
    double value; // 评估值，=Qsa+u
    double nUct;  // UCT常数，控制搜索的
    int side;         // 轮到哪一方走
    bool isFullyExpanded;  // 是否完全展开，如果完全展开了就不用rollout out了
    int leaf_value;  // 存储叶子节点的值，不进行rollout out就返回该值

    int state[10][10];  // 当前的局面状态

    // 父节点与孩子节点
    std::weak_ptr<MCTSNode>pParent;
    std::vector<std::shared_ptr<MCTSNode>>pChild;

    // 所有的未经过拓展的步法
    std::vector<ChessMove>UnTriedMove;

    // 必须指定父节点，防止出错
    MCTSNode(std::shared_ptr<MCTSNode>par,int Board[10][10],int _side,double n_uct=1.41){
        Qsa=0.0;
        n_visits=1;  // 只要构造，就表示访问过一次
        u=0.0;
        nUct=n_uct;
        isFullyExpanded=false;
        side=_side;
        value=INF;   // 未被搜索过的节点评估值是最大的
        pParent=par;
        memcpy(state,Board,10*10*sizeof (int));  // 复制状态
    }

    bool operator<(const MCTSNode& node){
        return value<node.value;
    }
};

#endif // TREENODE_H
