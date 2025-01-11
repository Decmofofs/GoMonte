//
// Created by Decmo on 25-1-11.
//
#include "GameData.h"
#include "GameController.h"
#include "SearchAgent.h"
#include <iostream>
GameController::GameController(QObject *parent, MainBoardWidget * bd_widget) : QObject(parent), board_widget(bd_widget) {
    search_thread = new QThread();
    search_agent = new SearchAgentWorker();
    search_agent->moveToThread(search_thread);
    search_thread->start();
    connect(this, &GameController::send_to_search_agent, search_agent, &SearchAgentWorker::search_best_move);
    connect(search_agent, &SearchAgentWorker::send_best_move, this, &GameController::receive_best_move);
    connect(board_widget, &MainBoardWidget::cell_clicked, this, &GameController::handle_player_move);
    connect(this, &GameController::send_state_to_widget, board_widget, &MainBoardWidget::receive_instruction);
}

GameController::~GameController() {
    search_thread->quit();
    search_thread->wait();
    delete search_thread;
    delete search_agent;
}

void GameController::process_game_over() {
    initialized = 0;
    QString result;
    if (game_result == 0) {
        result = "平局！";
    } else if (game_result == 1) {
        result = "玩家(";
        if (human_player == PlayerOccupy::BLACK) {
            result += "黑方";
        } else {
            result += "白方";
        }
        result += ")获胜！";
    } else {
        result = "AI(";
        if (AI_player == PlayerOccupy::BLACK) {
            result += "黑方";
        } else {
            result += "白方";
        }
        result += ")获胜！";
    }
    QMessageBox::information(dynamic_cast<QWidget*>(this->parent()), "游戏结束", result);
}

void GameController::process_AI_response(int x, int y) {
    if (game_board.check_win({x,y, AI_player})) {
        game_board.set(x, y, AI_player);
        emit send_state_to_widget(x, y, AI_player);
        game_over = 1;
        game_result = 2;
        process_game_over();
        return;
    }
    game_board.set(x, y, AI_player);
    if (game_board.is_full()) {
        game_over = 1;
        game_result = 0;
    }
    emit send_state_to_widget(x, y, AI_player);
}

void GameController::process_player_move(int x, int y) {
    is_AI_thinking = true;
    if (game_board.is_forbidden({x,y,human_player})) {
        process_forbidden_move(x,y);
        return;
    }
    if (game_board.check_win({x,y, human_player})) {

        game_board.set(x, y, human_player);
        emit send_state_to_widget(x, y, human_player);
        game_over = 1;

        game_result = 1;
        process_game_over();
        return;
    }
    game_board.set(x, y, human_player);
    if (game_board.is_full()) {
        game_over = 1;
        game_result = 0;
    }



    emit send_state_to_widget(x, y, human_player);
    emit send_to_search_agent(game_board);
}

void GameController::process_forbidden_move(int x, int y) {
    // TODO
}

void GameController::receive_best_move(MoveInfo move) {
    is_AI_thinking = false;
    process_AI_response(move.x, move.y);
}

void GameController::handle_player_move(int x, int y) {

    if (game_over) return;

    process_player_move(x, y);

}

void GameController::initialize_game() {
    game_board = GomokuBoard();
    game_over = 0;
    game_result = 0;
    is_AI_thinking = false;
    if (radio_black_checked == 1) {
        AI_player = PlayerOccupy::WHITE;
        human_player = PlayerOccupy::BLACK;
        cur_player = PlayerOccupy::BLACK;
    }
    else {
        AI_player = PlayerOccupy::BLACK;
        human_player = PlayerOccupy::WHITE;
        cur_player = PlayerOccupy::WHITE;
        game_board.set(7, 7, AI_player);
    }
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            emit send_state_to_widget(i, j, game_board.at(i,j));
        }
    }
    initialized = 1;
}

void GameController::restart_game() {
    initialize_game();
}
