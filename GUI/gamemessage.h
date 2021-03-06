#ifndef GAMEMESSAGE_H
#define GAMEMESSAGE_H

#include <QTextBrowser>
#include <QString>
#include "utils.h"

/*
* 用于显示所有的游戏信息的类，
* 只要是游戏数据的输出，就可以使用这个类型进行输出
* Erick Lv 09/30/2018
*/

class GameMessage : public QTextBrowser
{
    Q_OBJECT
public:
    explicit GameMessage(QWidget* parent=nullptr);

public slots:
    // 用于接受消息的槽
    void receiveMessage(QString str);
    // 用于清空消息的槽
    void clearMessage();
};

#endif // GAMEMESSAGE_H
