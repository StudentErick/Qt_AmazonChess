#ifndef TESTENGINE_H
#define TESTENGINE_H

#include <vector>
#include "basicengine.h"
#include "movegenerator.h"

/*
*   这是一个用来测试人机交互可行的类，所有的步法都是随机的
*   @author: Erick Lv
*   @time: 2018/10/03
*/

class TestEngine:public BasicEngine
{
public:
    TestEngine();
    virtual ~TestEngine()override;
    // 重写基类的函数
    virtual ChessMove getBestMove(const ChessMove& mv)override;
protected:
    // 走法队列
    std::vector<ChessMove>m_MoveList;
};

#endif // TESTENGINE_H
