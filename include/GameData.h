//
// Created by Decmo on 25-1-9.
//

#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <limits>
#include <utility>
#include <cstddef>
constexpr int BOARD_SIZE = 15;
constexpr int MAX_DEPTH = 8;
constexpr int MAX_SIMULATION = 1000;
constexpr double C = 1.414;
constexpr double EPS = std::numeric_limits<float>::min();
constexpr int MAX_TIME = 1000; // Milliseconds

enum class PlayerOccupy {
    BLACK,
    WHITE,
    NONE
};

inline PlayerOccupy cur_player = PlayerOccupy::BLACK;
#endif //GAMEDATA_H
