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
    SearchAgentWorker * worker = new SearchAgentWorker();

    QThread * thread = new QThread();
    worker->moveToThread(thread);
    thread->start();

    QObject::connect(Board, &MainBoardWidget::send_to_search_agent, worker, &SearchAgentWorker::search_best_move);
    QObject::connect(worker, &SearchAgentWorker::send_best_move, Board, &MainBoardWidget::receive_best_move);

    Board->setParent(&w);
    w.set_board(Board);
    w.show();
    int val = a.exec();
    thread->quit();
    return val;
}
