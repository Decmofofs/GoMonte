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
private:
    GomokuBoard board;
    PlayerOccupy AI_Player;
    PlayerOccupy Human_Player;
    PlayerOccupy Node_Player;
    MCTSTree * parent;
    std::map<MoveInfo, MCTSTree *> children;
    int visits;
    int wins;
    int loses;

public:
    MCTSTree(const GomokuBoard & board, const MoveInfo & AI_Player, const MoveInfo & Human_Player, const MoveInfo & Node_Player, MCTSTree * parent = nullptr) :
           board(board), AI_Player(AI_Player.player), Human_Player(Human_Player.player), Node_Player(Node_Player.player), parent(parent), visits(0), wins(0), loses(0) {}

    int is_fully_expanded() const;

    MCTSTree * get_best_child() ;
};

class SearchAgentWorker : public QObject {
    Q_OBJECT
public:
    explicit SearchAgentWorker(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void search_best_move(const GomokuBoard & board, PlayerOccupy AI_Player, PlayerOccupy Human_Player, PlayerOccupy Node_Player, int time = MAX_TIME);



};
#endif //SEARCHAGENT_H
