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
#include "MCTSTree.h"


class SearchAgentWorker : public QObject {
    Q_OBJECT
public:
    explicit SearchAgentWorker(QObject *parent = nullptr) : QObject(parent) {}


public slots:
    void search_best_move(const GomokuBoard & board);

signals:
    void send_best_move(MoveInfo move);
};
#endif //SEARCHAGENT_H
