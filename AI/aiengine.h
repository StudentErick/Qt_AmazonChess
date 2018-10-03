#ifndef AIENGINE_H
#define AIENGINE_H

#include <QObject>
#include "utils.h"
#include "basicengine.h"

/*
* AI引擎，这里可以加载不同类型的AI引擎
*/

class AIEngine : public QObject
{
    Q_OBJECT
public:
    explicit AIEngine(QObject *parent = nullptr);
    ~AIEngine();

signals:
    // 发送计算的结果
    void sendResult(ChessMove move);
public slots:
    // 获取使用的AI引擎的编号
    void getEngineNumber(int num);
    // 获取步法开始计算
    void getMove(ChessMove move);

protected:
    // 搜索引擎
    BasicEngine* m_basciEngine;
    // 引擎编号
    int m_EngineNum;
    // 获取当前一个移动
    ChessMove m_getMove;
};

#endif // AIENGINE_H
