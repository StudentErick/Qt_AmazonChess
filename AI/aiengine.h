#ifndef AIENGINE_H
#define AIENGINE_H

#include <QThread>
#include "utils.h"
#include "basicengine.h"

/*
* AI引擎，这里可以加载不同类型的AI引擎
*/

class AIEngine : public QThread
{
    Q_OBJECT
public:
    AIEngine();
    virtual ~AIEngine();

signals:
    // 发送计算的结果
    void sendResult(ChessMove move);
    // 发送计算结果
    void sendGameMsg(QString str);
public slots:
    // 获取使用的AI引擎的编号
    void getEngineNumber(int num);
    // 获取步法开始计算
    void getMove(ChessMove move);
    // 启动计算线程进行运算
    void run();
    // 获取是否可以进行计算的标记
    void getStopSignal();

protected:
    // 搜索引擎
    BasicEngine* m_basciEngine;
    // 引擎编号
    int m_EngineNum;
    // 获取当前一个移动
    ChessMove m_getMove;
};

#endif // AIENGINE_H
