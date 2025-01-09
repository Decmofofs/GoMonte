//
// Created by Decmo on 25-1-9.
//
#include <cmath>
#include <SearchAgent.h>

int MCTSTree::is_fully_expanded() const {
    return children.size() == board.get_legal_moves().size();
}


MCTSTree * MCTSTree::get_best_child()  {
    double best_score = -std::numeric_limits<double>::infinity();
    MoveInfo best_move;
    for (const auto & child : children) {
        if (child.second->visits == 0) return child.second;

        double exploitation = (double)child.second->wins / child.second->visits;
        double exploration = C * sqrt(log(visits) / child.second->visits);
        double score = exploitation + exploration;
        if (score > best_score) {
            best_score = score;
            best_move = child.first;
        }
    }
    return children[best_move];
}
