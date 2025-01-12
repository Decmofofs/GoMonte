//
// Created by Decmo on 25-1-12.
//

#ifndef GAMESAVER_H
#define GAMESAVER_H

#include "GameData.h"
#include "GomokuBoard.h"

class GameSaver {
public:
    explicit GameSaver();
    void add_move(MoveInfo move);
    MoveInfo undo_move();
    MoveInfo redo_move();

    int get_savedMoveListPtr() const;
    int get_savedMoveListSize() const;


private:
    GomokuBoard savedBoard;
    std::vector<MoveInfo> savedMoveList;
    int savedMoveListPtr;
    PlayerOccupy savedCurPlayer;
    PlayerOccupy savedAIPlayer;
    PlayerOccupy savedHumanPlayer;
};

#endif //GAMESAVER_H
