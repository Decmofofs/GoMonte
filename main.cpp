#include "mainwindow.h"

#include <QApplication>
#include <GameData.h>
#include <GomokuBoard.h>
#include <BoardCellWidget.h>
#include <MainBoardWidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    MainBoardWidget  * Board = new MainBoardWidget(nullptr, BOARD_SIZE);
    MainWindow w;

    Board->setParent(&w);
    w.set_board(Board);
    w.show();
    return a.exec();
}
