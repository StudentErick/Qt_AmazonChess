#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_DrawBoard=new DrawBoard(ui->centralWidget);
    m_GameMesage=new GameMessage(ui->centralWidget);
    m_Manager=new Manager;

    // Manager与菜单的关联
    // 开始游戏
    QObject::connect(ui->actionBegin,&QAction::triggered,m_Manager,&Manager::startGame);
    // PVP模式，黑色先走
    QObject::connect(ui->actionBlackFirst,&QAction::triggered,m_Manager,&Manager::PVP_BlackFirst);
    // PVP模式，白色先走
    QObject::connect(ui->actionWhiteFirst,&QAction::triggered,m_Manager,&Manager::PVP_WhiteFirst);
    // PVC模式，计算机先走
    QObject::connect(ui->actionComputerFirst,&QAction::triggered,m_Manager,&Manager::PVC_CFirst);
    // PVC模式，人先走
    QObject::connect(ui->actionPersonFirst,&QAction::triggered,m_Manager,&Manager::PVC_PFirst);

    // Manager与Button的关联
    // 向前走一步，悔棋
    QObject::connect(ui->btnPrevStep,&QPushButton::clicked,m_Manager,&Manager::previousMove);
    // 向后走一步
    QObject::connect(ui->btnNextStep,&QPushButton::clicked,m_Manager,&Manager::nextMove);

    // DrawBoard与GameMessage的关联
    // 关联发送消息
    QObject::connect(m_DrawBoard,&DrawBoard::sendMessage,m_GameMesage,&GameMessage::receiveMessage);

    // Manager与GameMessage的关联
    // 关联发送消息
    QObject::connect(m_Manager,&Manager::sendMessage,m_GameMesage,&GameMessage::receiveMessage);

    // Manager与DrawBoard的关联
    // 关联走子
    QObject::connect(m_Manager,&Manager::sendMoveMessage,m_DrawBoard,&DrawBoard::makeMove);
    // 关联悔棋
    QObject::connect(m_Manager,&Manager::sendRetractMessage,m_DrawBoard,&DrawBoard::retractMove);
    // 关联更改走子方
    QObject::connect(m_Manager,&Manager::sendNextSide,m_DrawBoard,&DrawBoard::setSide);
    // 关联初始化
    QObject::connect(m_Manager,&Manager::sendInitBoard,m_DrawBoard,&DrawBoard::initBoard);
    // 关联点击函数
    QObject::connect(m_Manager,&Manager::sendCheckable,m_DrawBoard,&DrawBoard::setCheckable);
    // 获取用户的点击状态
    QObject::connect(m_DrawBoard,&DrawBoard::sendMove,m_Manager,&Manager::getMove);

    // 清空信息的关联
    QObject::connect(ui->btnClearMsg,&QPushButton::clicked,m_GameMesage,&GameMessage::clearMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_DrawBoard!=nullptr){
        delete m_DrawBoard;
        m_DrawBoard=nullptr;
    }
    if(m_GameMesage!=nullptr){
        delete m_GameMesage;
        m_GameMesage=nullptr;
    }
    if(m_Manager!=nullptr){
        delete m_Manager;
        m_Manager=nullptr;
    }
}
