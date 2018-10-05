#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化所有的模块类
    m_DrawBoard=new DrawBoard(ui->centralWidget);
    m_GameMesage=new GameMessage(ui->centralWidget);
    m_Manager=new Manager;
    m_AIEngine=new AIEngine;

    // 悔棋和后一步在未开始状态不能用
    ui->btnNextStep->setEnabled(false);
    ui->btnPrevStep->setEnabled(false);
    // 终止AI不能用
    ui->btnStopAI->setEnabled(false);


    /***********************
    * Manager与菜单的关联 *
    ***********************/
    // 开始游戏
    QObject::connect(ui->actionBegin,&QAction::triggered,m_Manager,&Manager::startGame);
    // 结束本局
    QObject::connect(ui->actionEndGame,&QAction::triggered,m_Manager,&Manager::initBoard);
    // PVP模式，黑色先走
    QObject::connect(ui->actionBlackFirst,&QAction::triggered,m_Manager,&Manager::PVP_BlackFirst);
    // PVP模式，白色先走
    QObject::connect(ui->actionWhiteFirst,&QAction::triggered,m_Manager,&Manager::PVP_WhiteFirst);
    // PVC模式，计算机先走
    QObject::connect(ui->actionComputerFirst,&QAction::triggered,m_Manager,&Manager::PVC_CFirst);
    // PVC模式，人先走
    QObject::connect(ui->actionPersonFirst,&QAction::triggered,m_Manager,&Manager::PVC_PFirst);
    // 开始或结束对弈信号
    QObject::connect(m_Manager,&Manager::sendOnGame,this,&MainWindow::receiveOnGame);
    // 获取TestEngine
    QObject::connect(ui->actionTestEngine,&QAction::triggered,m_Manager,&Manager::getTestEngine);
    // 获取MCRave引擎
    QObject::connect(ui->actionMCRave,&QAction::triggered,m_Manager,&Manager::getMCRaveEngine);


    /**************************
    * Manager与Button的关联 *
    **************************/
    // 向前走一步，悔棋
    QObject::connect(ui->btnPrevStep,&QPushButton::clicked,m_Manager,&Manager::previousMove);
    // 向后走一步
    QObject::connect(ui->btnNextStep,&QPushButton::clicked,m_Manager,&Manager::nextMove);


    /***********************************
     *  Manager与GameMessage的关联 *
     * **********************************/
    QObject::connect(m_Manager,&Manager::sendMessage,m_GameMesage,&GameMessage::receiveMessage);


    /************************************
    * DrawBoard与GameMessage的关联 *
    *************************************/
    // 关联发送消息
    QObject::connect(m_DrawBoard,&DrawBoard::sendMessage,m_GameMesage,&GameMessage::receiveMessage);


    /******************************
    * Manager与DrawBoard的关联 *
    *******************************/
    // 关联走子
    QObject::connect(m_Manager,&Manager::sendMoveMsgToBoard,m_DrawBoard,&DrawBoard::makeMove);
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
    // 初始化局面
    QObject::connect(m_Manager,&Manager::sendInitBoard,m_DrawBoard,&DrawBoard::initBoard);


    /*********************
     * 按键与桌面控件关联 *
     * ********************/
    // 清空信息的关联
    QObject::connect(ui->btnClearMsg,&QPushButton::clicked,m_GameMesage,&GameMessage::clearMessage);


    /****************************
    * Manager与AIEngine的关联 *
    ****************************/
    // 发送走子的数据
    QObject::connect(m_Manager,&Manager::sendMoveMsgToAI,m_AIEngine,&AIEngine::getMove);
    // 获取AI的走法
    QObject::connect(m_AIEngine,&AIEngine::sendResult,m_Manager,&Manager::getMove);
    // 发送引擎的编号
    QObject::connect(m_Manager,&Manager::sendEngineNumber,m_AIEngine,&AIEngine::getEngineNumber);
    // 发送停止计算的信号
    QObject::connect(m_Manager,&Manager::sendAIStop,m_AIEngine,&AIEngine::getStopSignal);


    /***************
    * 软件信息弹窗 *
    ***************/
    // 关于软件
    QObject::connect(ui->actionAboutThis,&QAction::triggered,this,&MainWindow::showAboutSoftware);
    // 关于作者
    QObject::connect(ui->actionAboutAuthor,&QAction::triggered,this,&MainWindow::showAboutAuthor);
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
    if(m_AIEngine!=nullptr){
        delete m_AIEngine;
        m_AIEngine=nullptr;
    }
}


void MainWindow::receiveOnGame(bool flag){
    if(flag){
        ui->menuSetting->setEnabled(false);
        ui->actionOnline->setEnabled(false);

        ui->actionTestEngine->setEnabled(false);

        ui->btnNextStep->setEnabled(true);
        ui->btnPrevStep->setEnabled(true);
        ui->btnStopAI->setEnabled(true);

        ui->actionAboutThis->setEnabled(false);
        ui->actionAboutAuthor->setEnabled(false);
    }else{
        ui->menuSetting->setEnabled(true);
        ui->actionOnline->setEnabled(true);

        ui->actionTestEngine->setEnabled(true);

        ui->btnNextStep->setEnabled(false);
        ui->btnPrevStep->setEnabled(false);
        ui->btnStopAI->setEnabled(false);

        ui->actionAboutThis->setEnabled(true);
        ui->actionAboutAuthor->setEnabled(true);
    }
}

void MainWindow::showAboutSoftware(){
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("关于Amazon软件"));
    msgBox.setText(tr("该软件由Erick Lv制作<br>Github地址:<br>https://github.com/StudentErick/Qt_AmazonChess<br>"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::showAboutAuthor(){
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("关于作者"));
    msgBox.setText(tr("作者：Erick Lv<br>邮箱地址:<br>Erick_Lv@outlook.com"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
