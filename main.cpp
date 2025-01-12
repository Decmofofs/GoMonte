#include "mainwindow.h"

#include <QApplication>
#include <GameData.h>
#include <GomokuBoard.h>
#include <BoardCellWidget.h>
#include <MainBoardWidget.h>
#include "GameController.h"
#include "MainMenuWidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    MainBoardWidget  * Board = new MainBoardWidget(&w, BOARD_SIZE);
    MainMenuWidget * MainMenu = new MainMenuWidget(&w);

    GameController * gameController = new GameController(&w, Board);

    QObject::connect(MainMenu, &MainMenuWidget::start_game, gameController, &GameController::initialize_game);
    QObject::connect(MainMenu, &MainMenuWidget::restart_game, gameController, &GameController::initialize_game);
    QObject::connect(MainMenu, &MainMenuWidget::undo, gameController, &GameController::undo_move);
    QObject::connect(MainMenu, &MainMenuWidget::redo, gameController, &GameController::redo_move);

    w.set_board(Board);
    w.set_main_menu(MainMenu);
    w.show();
    int val = a.exec();

    return val;
}
