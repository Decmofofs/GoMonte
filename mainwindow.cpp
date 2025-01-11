#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QResizeEvent>
#include <algorithm>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow){

    setWindowIcon(QIcon(":/resources/icons/GoMonte.png"));

    ui->setupUi(this);
    setWindowTitle("GoMonte v1.0");

    setMinimumSize(1200,1000);
}

MainWindow::~MainWindow(){

    delete ui;
}


void MainWindow::set_board(MainBoardWidget *board){

    m_Board = board;
    //setCentralWidget(m_Board);

}

void MainWindow::set_main_menu(MainMenuWidget *menu){

    m_MainMenu = menu;
    //setCentralWidget(m_MainMenu);
}

void MainWindow::resizeEvent(QResizeEvent *event){

    QMainWindow::resizeEvent(event);
    int w = width(), h = height();
    int s = std::min(w, h);
    s = std::max(750, (s*4/5)/(2*BOARD_SIZE)*(2*BOARD_SIZE));

    if (m_Board != nullptr){
        m_Board->setFixedSize(s, s);
        m_Board->move((w-s)/2+(w-s)/4, (h-s)/4);
    }

    if (m_MainMenu != nullptr){
        m_MainMenu->setFixedSize(3*(w-s)/8,s );
        m_MainMenu->move(3*(w-s)/16, (h-s)/4);
    }
}


