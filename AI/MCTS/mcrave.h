#ifndef MCRAVE_H
#define MCRAVE_H

#include "utils.h"
#include "AI/basicengine.h"
#include "AI/treenode.h"

/*
* MCRave搜索算法，第一个UCT测试案例，具体参考论文：
* Monte-Carlo Tree Search and Rapid Action Value Estimation in Computer Go (Algorithms1)
* 区别在于这里使用了负极大搜索，黑方获胜返回1，白方获胜返回0，其余更改参照代码
* @author: Erick Lv
* @time: 2018/10/04
*/

class MCRave:public BasicEngine
{
public:
    MCRave();
};

#endif // MCRAVE_H
