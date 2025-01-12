//
// Created by Decmo on 25-1-12.
//
#include "GameSaver.h"
#include <QDebug>
GameSaver::GameSaver() {
    savedMoveListPtr = -1;
    savedBoard = GomokuBoard();
    savedMoveList = std::vector<MoveInfo>();
    if (radio_black_checked == 1) {
        savedCurPlayer = PlayerOccupy::BLACK;
        savedAIPlayer = PlayerOccupy::WHITE;
        savedHumanPlayer = PlayerOccupy::BLACK;
    } else {
        savedCurPlayer = PlayerOccupy::WHITE;
        savedAIPlayer = PlayerOccupy::BLACK;
        savedHumanPlayer = PlayerOccupy::WHITE;
        add_move({7,7,PlayerOccupy::BLACK});
    }
}

void GameSaver::add_move(MoveInfo move) {
    if (savedMoveListPtr != savedMoveList.size()-1 && savedMoveListPtr != -1 && savedMoveList.size() > 0) {
        savedMoveList.erase(savedMoveList.begin()+savedMoveListPtr+1, savedMoveList.end());
    }
    savedMoveList.push_back(move);
    savedMoveListPtr++;
    savedBoard.set(move.x, move.y, move.player);
}

MoveInfo GameSaver::undo_move() {
    if (savedMoveListPtr == -1) {
        return {0,0,PlayerOccupy::NONE};
    }

    MoveInfo move = savedMoveList[savedMoveListPtr];
    qInfo() << savedMoveListPtr;
    savedMoveListPtr--;
    savedBoard.set(move.x, move.y, PlayerOccupy::NONE);
    return move;
}

MoveInfo GameSaver::redo_move() {
    if (savedMoveListPtr == savedMoveList.size()-1) {
        return {0,0,PlayerOccupy::NONE};
    }

    savedMoveListPtr++;
    MoveInfo move = savedMoveList[savedMoveListPtr];
    savedBoard.set(move.x, move.y, move.player);
    return move;
}

int GameSaver::get_savedMoveListPtr() const {
    return savedMoveListPtr;
}

int GameSaver::get_savedMoveListSize() const {
    return savedMoveList.size();
}