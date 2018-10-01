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
    DrawBoard* m_DrawBoard;
    GameMessage* m_GameMesage;
    Manager* m_Manager;
};

#endif // MAINWINDOW_H
