//
// Created by Decmo on 25-1-11.
//

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMessageBox>
#include "GameData.h"
#include "MainBoardWidget.h"
#include "GomokuBoard.h"
#include "SearchAgent.h"

class GameController : public QObject {
    Q_OBJECT

public:
    explicit  GameController(QObject *parent = nullptr, MainBoardWidget * board = nullptr);
    ~GameController() override;
    void process_AI_response(int x, int y);
    void process_player_move(int x, int y);

    void process_forbidden_move(int x, int y);
    void process_game_over();

signals:
    void send_to_search_agent(const GomokuBoard & board);
    void send_state_to_widget(int x, int y, PlayerOccupy state);


public slots:
    void receive_best_move(MoveInfo move);
    void handle_player_move(int x, int y);
    void initialize_game();
    void restart_game();

private:
    GomokuBoard game_board;
    QThread * search_thread;
    QThread * file_thread;
    SearchAgentWorker * search_agent;

    MainBoardWidget * board_widget;
};

#endif //GAMECONTROLLER_H
