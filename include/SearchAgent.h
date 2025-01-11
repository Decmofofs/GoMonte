//
// Created by Decmo on 25-1-9.
//

#ifndef SEARCHAGENT_H
#define SEARCHAGENT_H

#include <limits>
#include <GomokuBoard.h>
#include <map>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <chrono>
#include <QWaitCondition>
#include <GameData.h>

class MCTSTree {
public:
    GomokuBoard board;
    std::set<std::pair<int, int>> legal_moves;
    PlayerOccupy Node_Player;
    MCTSTree * parent;
    std::map<MoveInfo, MCTSTree *> children;
    int visits;
    int wins;
    int loses;
    int legal_move_cnt;
    int is_end_state;

    MCTSTree(const GomokuBoard & board, const PlayerOccupy & Node_Player, MCTSTree * parent = nullptr);


    int is_fully_expanded() const;

    MoveInfo get_move_to_best_child(int type) ;
    int delete_tree();
};

class SearchAgentWorker : public QObject {
    Q_OBJECT
public:
    explicit SearchAgentWorker(QObject *parent = nullptr) : QObject(parent) {}
    static void mcts_simulate(MCTSTree * node);

public slots:
    void search_best_move(const GomokuBoard & board);

signals:
    void send_best_move(MoveInfo move);
};
#endif //SEARCHAGENT_H
