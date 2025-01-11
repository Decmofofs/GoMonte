//
// Created by Decmo on 25-1-9.
//

#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <limits>
#include <utility>
#include <cstddef>

constexpr int dot_radius = 3;
constexpr int BOARD_SIZE = 15;
constexpr int MAX_DEPTH = 12;
constexpr int MAX_SIMULATION = 3000;
constexpr double C = 1.414;
constexpr double EPS = std::numeric_limits<float>::min();
constexpr int MAX_TIME = 10000; // Milliseconds

enum class PlayerOccupy {
    BLACK,
    WHITE,
    NONE
};

inline PlayerOccupy cur_player = PlayerOccupy::BLACK;
inline PlayerOccupy AI_player = PlayerOccupy::WHITE;
inline PlayerOccupy human_player = PlayerOccupy::BLACK;
inline bool is_AI_thinking = false;

inline int game_over;
inline int game_result; // 0: draw, 1: black win, 2: white win
#endif //GAMEDATA_H
