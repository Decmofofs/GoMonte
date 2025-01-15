#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <utility>
#include <algorithm> // For std::min, std::max
#include <iterator>  // For std::advance
#include <atomic>    // If you want atomic counters, etc.
#include "GameData.h"
#include "SearchAgent.h"
#include <random>

inline int random_int(int min_val, int max_val) {
    static thread_local std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<int> dist(min_val, max_val);
    return dist(engine);
}

void SearchAgentWorker::search_best_move(const GomokuBoard &board)
{

    std::map<std::pair<int, int>, int> move_scores;

    std::mutex move_scores_mutex;

    const unsigned num_threads = std::max(1u, std::thread::hardware_concurrency())-1;

    const int chunk_size = MAX_SIMULATION / num_threads;


    auto worker_func = [&](int start, int end)
    {
        // Local map for this thread
        std::map<std::pair<int, int>, int> local_scores;

        for (int i = start; i < end; ++i)
        {
            GomokuBoard sim_board = board;
            PlayerOccupy sim_player = AI_player;

            int confirm = 0;
            std::pair<int, int> move{-1, -1};

            for (int d = 0; d < 60; d++)
            {
                auto possible_moves = sim_board.get_legal_moves();
                if (possible_moves.empty())
                    break;

                auto move_it = possible_moves.begin();
                std::advance(move_it, random_int(0, static_cast<int>(possible_moves.size()) - 1));
                move = *move_it;

                sim_board.set(move.first, move.second, sim_player);

                if (sim_board.check_win({ move.first, move.second, human_player }))
                {
                    confirm = 1;
                    break;
                }
                if (sim_board.check_win({ move.first, move.second, AI_player }))
                {
                    confirm = 1;
                    break;
                }

                sim_player = (sim_player == PlayerOccupy::BLACK)
                           ? PlayerOccupy::WHITE
                           : PlayerOccupy::BLACK;
            }

            if (confirm == 1)
            {
                local_scores[move] += 1;
            }
        }

        {
            std::lock_guard<std::mutex> lock(move_scores_mutex);
            for (auto &kv : local_scores)
            {
                move_scores[kv.first] += kv.second;
            }
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    int start_index = 0;
    for (unsigned t = 0; t < num_threads; ++t)
    {
        int simulations_for_this_thread = chunk_size;



        int end_index = start_index + simulations_for_this_thread;
        threads.emplace_back(worker_func, start_index, end_index);

        start_index = end_index;
    }

    for (auto &th : threads)
    {
        th.join();
    }

    int best_score = -1;
    std::pair<int, int> best_move = { -1, -1 };
    for (auto &ele : move_scores)
    {
        if (ele.second > best_score)
        {
            best_score = ele.second;
            best_move = ele.first;
        }
    }

    emit send_best_move({best_move.first, best_move.second, AI_player});
}
