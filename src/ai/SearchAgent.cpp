
#include <vector>





#include "GameData.h"
#include "SearchAgent.h"
#include <random>

inline int random_int(int min_val, int max_val) {
    static thread_local std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<int> dist(min_val, max_val);
    return dist(engine);
}

struct PairHash {

    int  operator()(const std::pair<int, int>& p) const {
        return p.first * 15 + p.second;
    }

};


struct SortScore {
    bool operator()(const std::pair<std::pair<int, int>, int> &a, const std::pair<std::pair<int, int>, int> &b) {
        return a.second > b.second;
    }
};

void SearchAgentWorker::search_best_move(const GomokuBoard &board)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time + std::chrono::milliseconds(MAX_TIME-50);

    std::map<std::pair<int, int>, int> move_scores;

    int visit[15][15] = {};

    while (true){
        std::chrono::system_clock::time_point cur_time = std::chrono::high_resolution_clock::now();
        if (cur_time > end_time) {
            break;
        }

        memset(visit,0,sizeof(visit));

        GomokuBoard sim_board = board;
        PlayerOccupy sim_player = AI_player;
        std::pair<int,int> move;
        int confirm = 0;
        auto possible_moves_set = sim_board.get_legal_moves();
        std::vector<std::pair<int, int>> possible_moves;

        int forbid_skip = 0;

        for (auto x: possible_moves_set) {
            possible_moves.push_back(x);
        }

        for (int d=0; d<60; d++) {

            if (possible_moves.empty()) {
                qInfo() << "No possible moves";
                break;
            }

            int rd_index = random_int(0, possible_moves.size()-1);
            move = possible_moves[rd_index];

            if (sim_player == PlayerOccupy::BLACK && sim_board.is_forbidden({move.first, move.second, sim_player})) {
                forbid_skip = 1;

                continue;
            }

            sim_board.set(move.first, move.second, sim_player);
            visit[move.first][move.second] = 1;

            int bx = move.first, by = move.second;

            swap(possible_moves[rd_index], possible_moves[possible_moves.size()-1]);
            possible_moves.erase(possible_moves.end()-1);
            for (int dx=-2;dx<=2;dx++) {
                for (int dy=-2;dy<=2;dy++) {
                    if ((dx+bx)>=0 && (dx+bx) < BOARD_SIZE && (dy+by)>=0 && (dy+by) < BOARD_SIZE && visit[dx+bx][dy+by] == 0) {
                        if (sim_board.at(dx+bx, dy+by) == PlayerOccupy::NONE) {
                            possible_moves.push_back({dx+bx, dy+by});
                        }
                    }
                }
            }


            if (sim_board.check_win({move.first, move.second,human_player})) {
                confirm = 1;
                break;
            }
            if (sim_board.check_win({move.first,move.second,AI_player})) {
                confirm = 1;
                break;
            }
            sim_player = (sim_player == PlayerOccupy::BLACK) ? PlayerOccupy::WHITE : PlayerOccupy::BLACK;

        }

        if ( !forbid_skip && (confirm || move_scores.empty())) {
            move_scores[move] += 1;
        }

    }

    int best_score = -1;
    std::pair<int, int> best_move;
    if (move_scores.empty()) {
        // Random move
        auto possible_moves_set = board.get_legal_moves();
        std::vector<std::pair<int, int>> possible_moves;
        for (auto x: possible_moves_set) {
            possible_moves.push_back(x);
        }
        if (possible_moves.empty()) {
            return;
        }
        int rd_index = random_int(0, possible_moves.size()-1);
        best_move = possible_moves[rd_index];
        emit send_best_move({best_move.first, best_move.second, AI_player});
        return;
    }

    if (AI_player == PlayerOccupy::BLACK) {
        std::vector<std::pair<std::pair<int,int>,int>> move_scores_vec(move_scores.begin(), move_scores.end());
        std::sort(move_scores_vec.begin(),move_scores_vec.end(),SortScore());
        for (auto & score : move_scores_vec) {
            auto cur_move = score.first;
            if (!board.is_forbidden({cur_move.first,cur_move.second,PlayerOccupy::BLACK})) {
                emit send_best_move({cur_move.first,cur_move.second,PlayerOccupy::BLACK});
                return;
            }
        }

        auto possible_moves_set = board.get_legal_moves();
        auto possible_moves = std::vector<std::pair<int, int>>(possible_moves_set.begin(), possible_moves_set.end());
        auto rand_id = random_int(0, possible_moves.size()-1);
        best_move = possible_moves[rand_id];
        emit send_best_move({best_move.first,best_move.second,PlayerOccupy::BLACK});
        return;
    }


    for (auto ele : move_scores) {
        if (ele.second > best_score) {
            best_score = ele.second;
            best_move = ele.first;
        }
    }


    emit send_best_move({best_move.first, best_move.second, AI_player});
}