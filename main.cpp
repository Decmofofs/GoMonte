#include "mainwindow.h"

#include <QApplication>
#include <GameData.h>
#include <GomokuBoard.h>
#include <BoardCellWidget.h>
#include <MainBoardWidget.h>
#include "GameController.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    MainBoardWidget  * Board = new MainBoardWidget(&w, BOARD_SIZE);

    GameController * gameController = new GameController(nullptr, Board);

    w.set_board(Board);
    w.show();
    int val = a.exec();

    return val;
}
