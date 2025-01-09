//
// Created by Decmo on 25-1-8.
//

#ifndef GOMOKUBOARD_H
#define GOMOKUBOARD_H

#include <vector>
#include <set>
#include <utility>

enum class PlayerOccupy {
    BLACK,
    WHITE,
    NONE
};

class MoveInfo {
public:
    int x;
    int y;
    PlayerOccupy player;
};

constexpr size_t BOARD_SIZE = 15;

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

private:
    size_t S = BOARD_SIZE;
    int places = 0;
    std::vector<std::vector<PlayerOccupy>> board;
};

#endif //GOMOKUBOARD_H
