//
// Created by Decmo on 25-1-9.
//
#include <cmath>
#include <SearchAgent.h>
#include <stack>
#include <random>
#include <HelperFunctions.h>
#include <QDebug>
int MCTSTree::is_fully_expanded() const {
    return children.size() == board.get_legal_moves().size();
}


MoveInfo MCTSTree::get_move_to_best_child(const int type)  {
    double best_score = -std::numeric_limits<double>::infinity();
    MoveInfo best_move{};
    for (const auto & [fst, snd] : children) {
        if (snd->visits == 0) return fst;

        const double exploitation = static_cast<double>(snd->wins) / snd->visits;
        const double exploration = type? 0:C * sqrt(log(visits) / snd->visits);
        const double score = exploitation + exploration;
        if (score > best_score) {
            best_score = score;
            best_move = fst;
        }
    }
    return best_move;
}

int MCTSTree::delete_tree() {
    std::stack<MCTSTree*> stack;
    stack.push(this);

    while (!stack.empty()) {
        MCTSTree* node = stack.top();
        stack.pop();

        if (node != nullptr) {
            for (const auto& [fst, snd] : node->children) {
                stack.push(snd);
            }
            delete node;
        }

    }

    return 0;
}

void SearchAgentWorker::search_best_move(const GomokuBoard& board) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time + std::chrono::milliseconds(MAX_TIME-50);
    MCTSTree * root = new MCTSTree(board,AI_player);

    std::vector<MoveInfo> ret;

    for (int i=0;i<MAX_SIMULATION;i++) {
        if (std::chrono::high_resolution_clock::now() > end_time) break;
        mcts_simulate(root);
    }

    MoveInfo best_move = root->get_move_to_best_child(1);
    root->delete_tree();

    emit send_best_move(best_move);
}

void SearchAgentWorker::mcts_simulate(MCTSTree* node) {
    // Selection
    MCTSTree* current = node;

    while (current->is_fully_expanded() && !current->children.empty()) {
        current = current->children[current->get_move_to_best_child(0)];
    }
    std::random_device rd;
    std::mt19937 gen(rd());

    // Expansion
    auto legal_moves = current->board.get_legal_moves();

    MoveInfo this_move;
    if (!legal_moves.empty()) {
        auto iter = legal_moves.begin();
        std::uniform_int_distribution<> dis(0, legal_moves.size() - 1);
        std::advance(iter, dis(gen));

        this_move = {iter->first, iter->second, current->Node_Player};
        current->children[this_move] = new MCTSTree(current->board, get_switched_role(current->Node_Player), current);
        current = current->children[this_move];
        current->board.set(this_move.x, this_move.y, this_move.player);
    }


    // Simulation
    PlayerOccupy winner = PlayerOccupy::NONE, loser = PlayerOccupy::NONE; int confirm_winner = 0;

    if (current->parent->board.check_win(this_move)) {
        winner = current->parent->Node_Player;
        loser = get_switched_role(winner);
        confirm_winner = 1;
    }

    int depth = MAX_DEPTH;

    GomokuBoard temp_board = current->board;

    while (depth>0 && !confirm_winner) {
        depth --;
        std::set<std::pair<int, int>> moves; MoveInfo move;
        moves = temp_board.get_legal_moves();
        if (moves.empty()) break;
        std::uniform_int_distribution<> dis(0, moves.size() - 1);
        auto iter = moves.begin();
        std::advance(iter, dis(gen));

        move = {iter->first,iter->second,current->Node_Player};
        if (temp_board.check_win(move)) {
            winner = current->Node_Player;
            loser = get_switched_role(winner);
            confirm_winner = 1;
            break;
        }
        temp_board.set(move.x, move.y, move.player);

        moves = temp_board.get_legal_moves();
        if (moves.empty()) break;
        std::uniform_int_distribution<> dis2(0, moves.size() - 1);
        iter = moves.begin();
        std::advance(iter, dis2(gen));

        move = {iter->first,iter->second,get_switched_role(current->Node_Player)};
        if (temp_board.check_win(move)) {
            winner = get_switched_role(current->Node_Player);
            loser = current->Node_Player;
            confirm_winner = 1;
            break;
        }
        temp_board.set(move.x, move.y, move.player);
    }

    if (!confirm_winner) {
        int score = temp_board.heuristic_evaluation(current->Node_Player);
        if (score > 0) {
            winner = current->Node_Player;
            loser = get_switched_role(winner);
        } else if (score < 0) {
            winner = get_switched_role(current->Node_Player);
            loser = current->Node_Player;
        }
    }

    // Backpropagation
    while (current != nullptr) {
        current->visits ++;
        if (current->Node_Player == winner) {
            current->wins ++;
        } else if (current->Node_Player == loser) {
            current->loses ++;
        }
        current = current->parent;
    }



}
