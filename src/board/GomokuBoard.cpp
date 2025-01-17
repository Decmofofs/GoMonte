//
// Created by Decmo on 25-1-8.
//
#include <GomokuBoard.h>
#include <HelperFunctions.h>
#include <QDebug>
#include <unordered_set>


PlayerOccupy GomokuBoard::at(const int x, const int y) const {
    return board[x][y];
}

int GomokuBoard::set(const int x, const int y, const PlayerOccupy state) {
    if (board[x][y] == PlayerOccupy::NONE && state != PlayerOccupy::NONE) {
        board[x][y] = state;
        places++;
        return 0;
    }
    else if (board[x][y] != PlayerOccupy::NONE && state == PlayerOccupy::NONE) {
        board[x][y] = state;
        places--;
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

QJsonObject GomokuBoard::to_json() const {
    QJsonObject ret;
    ret["S"] = S;
    ret["places"] = places;
    QJsonArray arr;
    for (int i=0;i<S;i++){
        QJsonArray row;
        for (int j=0;j<S;j++){
            row.append(static_cast<int>(board[i][j]));
        }
        arr.append(row);
    }
    ret["board"] = arr;
    return ret;
}

GomokuBoard GomokuBoard::json_to_gomoku_board(const QJsonObject& obj) {
    GomokuBoard ret;
    ret.S = obj["S"].toInt();
    ret.places = obj["places"].toInt();
    QJsonArray arr = obj["board"].toArray();
    for (int i=0;i<ret.S;i++){
        QJsonArray row = arr[i].toArray();
        for (int j=0;j<ret.S;j++){
            ret.board[i][j] = static_cast<PlayerOccupy>(row[j].toInt());
        }
    }
    return ret;
}


int GomokuBoard::check_win(const MoveInfo move) const {


    if (move.player == PlayerOccupy::NONE) return 0;

    int dir[][2] = {{1,0},{0,1},{1,1},{1,-1}};

    int r = move.x, c = move.y;

    for (int i=0;i<4;i++) {
        int cnt = 1;
        int nr = r + dir[i][0], nc = c + dir[i][1];
        while (nr>=0 && nr<S && nc>=0 && nc<S && board[nr][nc] == move.player) {
            cnt++;
            nr += dir[i][0];
            nc += dir[i][1];
        }
        nr = r - dir[i][0], nc = c - dir[i][1];
        while (nr>=0 && nr<S && nc>=0 && nc<S && board[nr][nc] == move.player) {
            cnt++;
            nr -= dir[i][0];
            nc -= dir[i][1];
        }
        if (cnt >= 5) {
            return 1;
        }
    }

    return 0;
}

int GomokuBoard::is_full() const {
    return places == S * S;
}

int GomokuBoard::is_forbidden(const MoveInfo move) const {
    if (move.player == PlayerOccupy::WHITE) return 0;
    int dir[][2] = {{1,0},{0,1},{1,1},{1,-1}};
    int r = move.x, c = move.y;
    // 分别统计四个方向的连子数以及活数
    int alive[4];
    int cnt[4];
    for (int i=0;i<4;i++) {
        cnt[i] = 1;
        alive[i] = 2;
        int nr = r + dir[i][0], nc = c + dir[i][1];
        while (nr>=0 && nr<S && nc>=0 && nc<S && board[nr][nc] == move.player) {
            cnt[i]++;
            nr += dir[i][0];
            nc += dir[i][1];
        }
        if (nr >=0 && nr<S && nc>=0 && nc<S && board[nr][nc] != PlayerOccupy::NONE) {
            alive[i]--;
        }
        nr = r - dir[i][0], nc = c - dir[i][1];
        while (nr>=0 && nr<S && nc>=0 && nc<S && board[nr][nc] == move.player) {
            cnt[i]++;
            nr -= dir[i][0];
            nc -= dir[i][1];
        }
        if ((nr >=0 && nr<S && nc>=0 && nc<S && board[nr][nc] != PlayerOccupy::NONE)) {
            alive[i]--;
        }
    }
    for (int i=0;i<4;i++) {
        qInfo() << cnt[i] << " " << alive[i] << "dir" << i;
    }
    if (*std::max_element(cnt,cnt+4)>5) return 2;
    int alive_33 = 0;
    for (int i=0;i<4;i++) {
        if (alive[i]==2 && cnt[i]==3) {
            alive_33++;
        }
    }
    if (alive_33>=2) return 1;

    // 考虑跳四
    int non_jump_4 = 0;
    for (int i=0;i<4;i++) {
        if (cnt[i] == 4) {
            non_jump_4++;
        }
    }

    int jump_4 = 0;



    for (int i=0;i<4;i++) {

        if (cnt[i] >= 4) continue;
        if (alive[i] == 0) continue;

        int p_x, p_y, m_x, m_y;

        int jump_p = 0;
        int jump_m = 0;
        // 寻找连子的两端
        int nr = r + dir[i][0], nc = c + dir[i][1];
        while (nr >=0 && nr<S && nc>=0 && nc<S && board[nr][nc] == PlayerOccupy::BLACK) {
            nr += dir[i][0];
            nc += dir[i][1];
        }
        p_x = nr;
        p_y = nc;

        nr = r - dir[i][0], nc = c - dir[i][1];
        while (nr >=0 && nr<S && nc>=0 && nc<S && board[nr][nc] == PlayerOccupy::BLACK) {
            nr -= dir[i][0];
            nc -= dir[i][1];
        }
        m_x = nr;
        m_y = nc;

        if (p_x>=0 && p_x<S && p_y>=0 && p_y<S && board[p_x][p_y] == PlayerOccupy::NONE) {
            p_x += dir[i][0];
            p_y += dir[i][1];
            while (p_x >=0 && p_x<S && p_y>=0 && p_y < S && board[p_x][p_y] == PlayerOccupy::BLACK) {
                jump_p ++;
                p_x += dir[i][0];
                p_y += dir[i][1];
            }
        }


        if (m_x >=0 && m_x < S && m_y >=0 && m_y < S && board[m_x][m_y] == PlayerOccupy::NONE) {
            m_x -= dir[i][0];
            m_y -= dir[i][1];
            while (m_x >=0 && m_x<S && m_y>=0 && m_y < S && board[m_x][m_y] == PlayerOccupy::BLACK) {
                jump_m ++;
                m_x -= dir[i][0];
                m_y -= dir[i][1];
            }
        }

        if (cnt[i] + std::max(jump_m, jump_p) >= 4) {
            jump_4++;
        }


    }
    if (jump_4 + non_jump_4 >= 2) return 1;

    return 0;
}

std::set<std::pair<int, int>> GomokuBoard::get_legal_moves() const {
    std::set<std::pair<int, int>> legal_moves = {{}};
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
                if (count == 4 && alive == 2) {
                    point_score += 10000000;
                }
                else
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



