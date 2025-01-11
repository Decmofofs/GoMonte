//
// Created by Decmo on 25-1-9.
//
#include <cmath>
#include <SearchAgent.h>
#include <stack>
#include <random>
#include <HelperFunctions.h>
#include <QDebug>

MCTSTree::MCTSTree(const GomokuBoard &bd, const PlayerOccupy &Node_Player, MCTSTree *parent) :
        board(bd), Node_Player(Node_Player), parent(parent), visits(0), wins(0), loses(0) {
    legal_moves = board.get_legal_moves();
    legal_move_cnt = legal_moves.size();
    is_end_state = 0;
}

int MCTSTree::is_fully_expanded() const {
    return children.size() == legal_move_cnt;
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

    while (current->is_fully_expanded() && !current->children.empty() && current->is_end_state == 0) {
        current = current->children[current->get_move_to_best_child(0)];
    }

    if (current->is_end_state== 1) {
        // Direct Backpropagation
        PlayerOccupy winn = current->parent->Node_Player;
        while (current != nullptr) {
            current->visits ++;
            if (current->Node_Player == winn) {
                current->wins --;
            } else {
                current->wins ++;
            }
            current = current->parent;
        }
        return;
    }

    // Expansion
    std::random_device rd;
    std::mt19937 gen(rd());


    MoveInfo this_move;
    MCTSTree* new_node = nullptr;
    if (!current->legal_moves.empty()) {
        auto iter = current->legal_moves.begin();
        std::uniform_int_distribution<> dis(0, current->legal_moves.size() - 1);
        std::advance(iter, dis(gen));

        this_move = {iter->first, iter->second, current->Node_Player};
        GomokuBoard tmp_board = current->board;
        tmp_board.set(this_move.x, this_move.y, this_move.player);
        current->children[this_move] = new MCTSTree(tmp_board, get_switched_role(current->Node_Player), current);
        current->legal_moves.erase(iter);
        new_node = current->children[this_move];

    }


    // Simulation
    PlayerOccupy winner = PlayerOccupy::NONE; int confirm_winner = 0;

    if (new_node->parent->board.check_win(this_move)) {
        winner = new_node->parent->Node_Player;

        confirm_winner = 1;
    }

    if (confirm_winner) {
        new_node->is_end_state = 1;
        // Backpropagation
        while (new_node != nullptr) {
            new_node->visits ++;
            if (winner == new_node->Node_Player) {
                new_node->wins --;
            } else {
                new_node->wins ++;
            }
            new_node = new_node->parent;
        }
        return;
    }


    int depth = MAX_DEPTH;

    GomokuBoard temp_board = new_node->board;

    while (depth>0) {
        depth --;
        std::set<std::pair<int, int>> moves; MoveInfo move;
        moves = temp_board.get_legal_moves();
        if (moves.empty()) break;
        std::uniform_int_distribution<> dis(0, moves.size() - 1);
        auto iter = moves.begin();
        std::advance(iter, dis(gen));

        move = {iter->first,iter->second,new_node->Node_Player};
        if (temp_board.check_win(move)) {
            winner = new_node->Node_Player;

            confirm_winner = 1;
            break;
        }
        temp_board.set(move.x, move.y, move.player);

        moves = temp_board.get_legal_moves();
        if (moves.empty()) break;
        std::uniform_int_distribution<> dis2(0, moves.size() - 1);
        iter = moves.begin();
        std::advance(iter, dis2(gen));

        move = {iter->first,iter->second,get_switched_role(new_node->Node_Player)};
        if (temp_board.check_win(move)) {
            winner = get_switched_role(new_node->Node_Player);

            confirm_winner = 1;
            break;
        }
        temp_board.set(move.x, move.y, move.player);
    }

    if (!confirm_winner) {
        int score = temp_board.heuristic_evaluation(new_node->Node_Player);
        if (score > 0) {
            winner = new_node->Node_Player;

        } else if (score < 0) {
            winner = get_switched_role(new_node->Node_Player);

        }
    }

    // Backpropagation
    while (new_node != nullptr) {
        new_node->visits ++;
        if (winner == new_node->Node_Player) {
            new_node->wins --;
        } else {
            new_node->wins ++;
        }
        new_node = new_node->parent;
    }



}
