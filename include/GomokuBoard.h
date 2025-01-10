//
// Created by Decmo on 25-1-8.
//

#ifndef GOMOKUBOARD_H
#define GOMOKUBOARD_H

#include <vector>
#include <set>
#include <utility>
#include <GameData.h>


class MoveInfo {
public:
    int x;
    int y;
    PlayerOccupy player;
    bool operator<(MoveInfo x) const;
};



class GomokuBoard {

public:
    GomokuBoard() : board(S, std::vector<PlayerOccupy>(S, PlayerOccupy::NONE)) {}
    [[nodiscard]] PlayerOccupy at(int x, int y) const;
    int set(int x, int y, PlayerOccupy state);
    int reset();

    [[nodiscard]] int check_win(MoveInfo move) const;
    [[nodiscard]] int is_full() const;
    [[nodiscard]] int is_forbidden(MoveInfo move) const;
    [[nodiscard]] std::set<std::pair<int,int>> get_legal_moves() const;
    int heuristic_evaluation(PlayerOccupy player);

private:
    int S = BOARD_SIZE;
    int places = 0;
    std::vector<std::vector<PlayerOccupy>> board;
};

#endif //GOMOKUBOARD_H
