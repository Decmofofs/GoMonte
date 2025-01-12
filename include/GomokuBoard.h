//
// Created by Decmo on 25-1-8.
//

#ifndef GOMOKUBOARD_H
#define GOMOKUBOARD_H

#include <vector>
#include <set>
#include <utility>
#include "GameData.h"
#include <QJsonArray>





class GomokuBoard {

public:
    GomokuBoard() : board(S, std::vector<PlayerOccupy>(S, PlayerOccupy::NONE)) {}
    PlayerOccupy at(int x, int y) const;
    int set(int x, int y, PlayerOccupy state);
    int reset();

    int check_win(MoveInfo move) const;
    int is_full() const;
    int is_forbidden(MoveInfo move) const;
    std::set<std::pair<int,int>> get_legal_moves() const;
    int heuristic_evaluation(PlayerOccupy player);

    QJsonObject to_json() const;
    static GomokuBoard json_to_gomoku_board(const QJsonObject & arr);

private:
    int S = BOARD_SIZE;
    int places = 0;
    std::vector<std::vector<PlayerOccupy>> board;
};



#endif //GOMOKUBOARD_H
