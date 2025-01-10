//
// Created by Decmo on 25-1-8.
//
#include <GomokuBoard.h>
#include <HelperFunctions.h>
bool MoveInfo::operator<(const MoveInfo x) const {
    if (this->x<x.x) return true;
    if (this->x>x.x) return false;
    return this->y < x.y;
}


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
            if (board[i][j] != PlayerOccupy::NONE) {
                for (int u=-2;u<=2;u++){
                    for (int v=-2;v<=2;v++){
                        if (i+u>=0 && i+u<S && j+v>=0 && j+v<S && board[i+u][j+v] == PlayerOccupy::NONE){
                            legal_moves.insert(std::make_pair(i+u, j+v));
                        }
                    }
                }
            }
        }
    }
    if (legal_moves.empty()) {
        legal_moves.insert(std::make_pair(S/2, S/2));
    }
    return legal_moves;
}

int GomokuBoard::heuristic_evaluation(PlayerOccupy player) {
    int black_score = 0;
    int white_score = 0;
    int dir[][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int i=0;i<S;i++) {
        for (int j=0;j<S;j++) {
            if (board[i][j] == PlayerOccupy::NONE) continue;

            int point_score = 0;

            for (int k=0;k<4;k++) {
                int count = 0;
                int alive = 2;
                int alive_pts = 0; int cnt_pts = 0;
                int ni = i-dir[k][0], nj = j-dir[k][1];
                if (ni>=0 && ni<S && nj>=0 && nj<S && board[ni][nj] == board[i][j]) {
                    continue;
                }
                if (ni<0 || ni>=S || nj<0 || nj>=S || (board[ni][nj] == get_switched_role(board[i][j]))) {
                    alive --;
                }
                ni = i+dir[k][0], nj = j+dir[k][1];
                while (ni>=0 && ni<S && nj>=0 && nj<S && board[ni][nj] == board[i][j]) {
                    count++;
                    ni += dir[k][0];
                    nj += dir[k][1];
                }

                if (ni<0 || ni>=S || nj<0 || nj>=S || (board[ni][nj] == get_switched_role(board[i][j]))) {
                    alive --;
                }

                if (alive == 0) alive_pts = 2;
                if (alive == 1) alive_pts = 5;
                if (alive == 2) alive_pts = 10;

                if (count == 1) continue;
                if (count == 2) cnt_pts = 10;
                if (count == 3) cnt_pts = 100;
                if (count == 4) cnt_pts = 1000;

                point_score += alive_pts*cnt_pts;

            }

            if (board[i][j] == PlayerOccupy::BLACK) {
                black_score += point_score;
            } else {
                white_score += point_score;
            }
        }
    }
    if (player == PlayerOccupy::BLACK) {
        return black_score - white_score;
    }

    return white_score - black_score;

}

