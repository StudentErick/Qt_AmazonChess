#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*
* 主界面
* Erick Lv 09/30/2018
*/
#include <QMainWindow>
#include <QObject>
#include <memory>
#include <QWidget>
#include <QPushButton>
#include <QAction>
#include "./AI/aiengine.h"
#include "./GUI/drawboard.h"
#include "./GUI/gamemessage.h"
#include "manager.h"
#include "utils.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DrawBoard* m_DrawBoard;    // 绘制棋盘
    GameMessage* m_GameMesage; // 显示对弈信息
    Manager* m_Manager;        // 逻辑控制
    AIEngine* m_AIEngine;      // AI引擎
};

#endif // MAINWINDOW_H
