//
// Created by Decmo on 25-1-11.
//

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMessageBox>

#include "FileOP.h"
#include "GameData.h"
#include "MainBoardWidget.h"
#include "GomokuBoard.h"
#include "SearchAgent.h"
#include "GameSaver.h"
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
    void send_load_game(const QString & file_path);
    void send_save_game(const QString & file_path, const GameSaver & saved_game);


public slots:
    void receive_best_move(MoveInfo move);
    void handle_player_move(int x, int y);
    void initialize_game();
    void restart_game();
    void undo_move();
    void redo_move();
    void save_game();
    void load_game();
    void handle_save_result(bool success);
    void handle_load_result(bool success, const GameSaver & saved_game);


private:
    GameSaver game_saver;
    GomokuBoard game_board;
    std::vector<MoveInfo> move_list;
    int move_list_ptr;
    QThread * search_thread;
    QThread * file_thread;
    SearchAgentWorker * search_agent;
    FileOPWorker * file_op;

    MainBoardWidget * board_widget;
};

#endif //GAMECONTROLLER_H
