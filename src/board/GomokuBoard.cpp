//
// Created by Decmo on 25-1-8.
//
#include <GomokuBoard.h>

PlayerOccupy GomokuBoard::at(const int x, const int y) const {
    return board[x][y];
}

int GomokuBoard::set(const int x, const int y, const PlayerOccupy state) {
    if (board[x][y] == PlayerOccupy::NONE) {
        board[x][y] = state;
        places++;
        return 0;
    }
    return -1;
}

int GomokuBoard::reset() {
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < S; j++) {
            board[i][j] = PlayerOccupy::NONE;
        }
    }
    places = 0;
    return 0;
}

int GomokuBoard::check_win(const MoveInfo move) const {
    const int x = move.x;
    const int y = move.y;
    const PlayerOccupy state = move.player;
    int count = 0;
    int i, j;

    // Check horizontal
    for (i = x - 4; i <= x + 4; i++) {
        if (i < 0 || i >= S) continue;
        if (board[i][y] == state) {
            count++;
            if (count == 5) return 1;
        } else {
            count = 0;
        }
    }

    // Check vertical
    count = 0;
    for (j = y - 4; j <= y + 4; j++) {
        if (j < 0 || j >= S) continue;
        if (board[x][j] == state) {
            count++;
            if (count == 5) return 1;
        } else {
            count = 0;
        }
    }

    // Check diagonal
    count = 0;
    for (i = x - 4, j = y - 4; i <= x + 4; i++, j++) {
        if (i < 0 || i >= S || j < 0 || j >= S) continue;
        if (board[i][j] == state) {
            count++;
            if (count == 5) return 1;
        } else {
            count = 0;
        }
    }

    // Check anti-diagonal
    count = 0;
    for (i = x - 4, j = y + 4; i <= x + 4; i++, j--) {
        if (i < 0 || i >= S || j < 0 || j >= S) continue;
        if (board[i][j] == state) {
            count++;
            if (count == 5) return 1;
        } else {
            count = 0;
        }
    }

    return 0;
}

int GomokuBoard::is_full() const {
    return places == S * S;
}

int GomokuBoard::is_forbidden(const MoveInfo move) const {
    const int x = move.x;
    const int y = move.y;
    if (x < 0 || x >= S || y < 0 || y >= S) return 1;
    return board[x][y] != PlayerOccupy::NONE;
}

std::set<std::pair<int, int>> GomokuBoard::get_legal_moves() const {
    std::set<std::pair<int, int>> legal_moves;
    for (int i=0;i<S;i++){
        for (int j=0;j<S;j++){
            if (board[i][j] == PlayerOccupy::NONE){
                std::pair<int,int> temp = std::make_pair(i,j);
                legal_moves.insert(temp);
            }
        }
    }
    return legal_moves;
}

