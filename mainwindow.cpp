#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_DrawBoard=new DrawBoard(ui->centralWidget);
    m_GameMesage=new GameMessage(ui->centralWidget);

    // 关联对弈输出与界面
    QObject::connect(m_DrawBoard,&DrawBoard::sendMessage,m_GameMesage,&GameMessage::receiveMessage);
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
}
