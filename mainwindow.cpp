#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QResizeEvent>
#include <algorithm>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow){

    setWindowIcon(QIcon(":/resources/icons/GoMonte.png"));

    ui->setupUi(this);
    setWindowTitle("GoMonte v1.0");

    setMinimumSize(1000,800);
}

MainWindow::~MainWindow(){

    delete ui;
}


void MainWindow::set_board(MainBoardWidget *board){

    m_Board = board;
    //setCentralWidget(m_Board);

}

void MainWindow::resizeEvent(QResizeEvent *event){

    QMainWindow::resizeEvent(event);
    int w = width(), h = height();
    int s = std::min(w, h);
    s = std::max(750, (s*4/5)/(2*BOARD_SIZE)*(2*BOARD_SIZE));

    if (m_Board != nullptr){
        m_Board->setFixedSize(s, s);
        m_Board->move((w-s)/2, (h-s)/2);
    }
}


