#include "./GUI/gamemessage.h"

GameMessage::GameMessage(QWidget* parent):QTextBrowser (parent)
{
    this->setGeometry(MSG_POSW,MSG_POSH,MSG_WIDTH,MSG_HIGHT);
    this->setReadOnly(true);  //
}

void GameMessage::receiveMessage(QString str){
    this->append(str);
}

void GameMessage::clearMessage(){
    this->clear();
}
