#ifndef TREENODE_H
#define TREENODE_H

#include <memory>
#include <vector>
#include "utils.h"
/*
* 这个文件定义了搜索树节点的数据结构，
* 自定义的新节点类型也在这里添加
* @author: Erick Lv
* @time: 2018/10/04
*/

// 蒙特卡洛搜索树的节点，只要使用了MC大类的搜索算法都可以用这个
class MCTSNode{
public:
    int Ns;               // 总共访问的次数
    double Qsa;       // Q(s,a)
    double value;    // 节点最终的评估值
    bool is_firstVisited; // 是否第一次发现这个孩子，总是优先搜索未经过拓展的孩子
    bool is_leaf;   // 是否为叶子，不能扩展叶子节点
    ChessMove mv; // 节点的步法

    // 父节点指针，避免循环引用
    std::weak_ptr<MCTSNode>pParent;
    // 使用智能指针指向孩子节点，省去析构操作
    std::vector<std::shared_ptr<MCTSNode>>pChild;

    // 需要构造传入移动和父节点指针
    MCTSNode(const ChessMove& m, std::shared_ptr<MCTSNode>par=nullptr){
        pParent=par;
        mv=m;
        Ns=0;
        Qsa=0.0;
        value=0.0;
    }

    bool operator<(const MCTSNode& m){
        if(m.is_firstVisited){  // 未展开的总是优先访问
            return true;
        }
        return value<m.value;
    }
};

#endif // TREENODE_H
