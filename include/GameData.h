//
// Created by Decmo on 25-1-9.
//

#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <vector>

#include <QJsonObject>

inline int dot_radius = 3;
inline int BOARD_SIZE = 15;
inline int MAX_DEPTH = 30;
inline int MAX_SIMULATION = 3000;
inline double C = 1.414;
inline int MAX_TIME = 10000; // Milliseconds

enum class PlayerOccupy {
    BLACK = 2,
    WHITE = 1,
    NONE = 0
};


class MoveInfo {
public:
    int x;
    int y;
    PlayerOccupy player;
    bool operator<(MoveInfo x) const;
    QJsonObject to_json() const;
    static MoveInfo json_to_move_info(const QJsonObject & obj);
};

inline PlayerOccupy cur_player = PlayerOccupy::BLACK;
inline PlayerOccupy AI_player = PlayerOccupy::WHITE;
inline PlayerOccupy human_player = PlayerOccupy::BLACK;
inline bool is_AI_thinking = false;

inline int game_over;
inline int game_result; // 0: draw, 1: black win, 2: white win
inline int initialized = 0;

inline int radio_black_checked = 1;

inline int is_file_op_finished = 1;
#endif //GAMEDATA_H
