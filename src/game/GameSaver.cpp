//
// Created by Decmo on 25-1-12.
//
#include "GameSaver.h"
#include "HelperFunctions.h"
#include <QDebug>
GameSaver::GameSaver() {
    savedMoveListPtr = -1;
    savedBoard = GomokuBoard();
    savedMoveList = std::vector<MoveInfo>();
    if (radio_black_checked == 1) {
        savedCurPlayer = PlayerOccupy::BLACK;
        savedAIPlayer = PlayerOccupy::WHITE;
        savedHumanPlayer = PlayerOccupy::BLACK;
    } else {
        savedCurPlayer = PlayerOccupy::WHITE;
        savedAIPlayer = PlayerOccupy::BLACK;
        savedHumanPlayer = PlayerOccupy::WHITE;
        add_move({7,7,PlayerOccupy::BLACK});
    }
}

void GameSaver::add_move(MoveInfo move) {
    if (savedMoveListPtr != savedMoveList.size()-1 && savedMoveListPtr != -1 && savedMoveList.size() > 0) {
        savedMoveList.erase(savedMoveList.begin()+savedMoveListPtr+1, savedMoveList.end());
    }
    savedMoveList.push_back(move);
    savedMoveListPtr++;
    savedBoard.set(move.x, move.y, move.player);
    savedCurPlayer = get_switched_role(savedCurPlayer);
}

MoveInfo GameSaver::undo_move() {
    if (savedMoveListPtr == -1) {
        return {0,0,PlayerOccupy::NONE};
    }

    MoveInfo move = savedMoveList[savedMoveListPtr];
    qInfo() << savedMoveListPtr;
    savedMoveListPtr--;
    savedBoard.set(move.x, move.y, PlayerOccupy::NONE);
    savedCurPlayer = get_switched_role(savedCurPlayer);
    return move;
}

MoveInfo GameSaver::redo_move() {
    if (savedMoveListPtr == savedMoveList.size()-1) {
        return {0,0,PlayerOccupy::NONE};
    }

    savedMoveListPtr++;
    MoveInfo move = savedMoveList[savedMoveListPtr];
    savedBoard.set(move.x, move.y, move.player);
    savedCurPlayer = get_switched_role(savedCurPlayer);
    return move;
}

int GameSaver::get_savedMoveListPtr() const {
    return savedMoveListPtr;
}

int GameSaver::get_savedMoveListSize() const {
    return savedMoveList.size();
}

int GameSaver::save_to_file(const QString & file_path) const {
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qInfo() << "Failed to open file for writing";
        return 0;
    }

    QJsonObject save;
    save["savedBoard"] = savedBoard.to_json();

    QJsonArray savedMoveListArray;
    for (const auto & move : savedMoveList) {
        savedMoveListArray.append(move.to_json());
    }

    save["savedMoveList"] = savedMoveListArray;
    save["savedMoveListPtr"] = savedMoveListPtr;
    save["savedCurPlayer"] = static_cast<int>(savedCurPlayer);
    save["savedAIPlayer"] = static_cast<int>(savedAIPlayer);
    save["savedHumanPlayer"] = static_cast<int>(savedHumanPlayer);

    QJsonDocument doc(save);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return 1;
}

int GameSaver::load_from_file(const QString & file_path) {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qInfo() << "Failed to open file for reading";
        return 0;
    }

    QByteArray savedData = file.readAll();
    file.close();
    QJsonDocument doc(QJsonDocument::fromJson(savedData));

    if (doc.isNull() || doc.isEmpty()) {
        qInfo() << "Failed to parse JSON";

        return 0;
    }

    QJsonObject save = doc.object();
    savedBoard = GomokuBoard::json_to_gomoku_board(save["savedBoard"].toObject());
    savedMoveList = std::vector<MoveInfo>();
    QJsonArray savedMoveListArray = save["savedMoveList"].toArray();
    for (const auto & move : savedMoveListArray) {
        savedMoveList.push_back(MoveInfo::json_to_move_info(move.toObject()));
    }
    savedMoveListPtr = save["savedMoveListPtr"].toInt();
    savedCurPlayer = static_cast<PlayerOccupy>(save["savedCurPlayer"].toInt());
    savedAIPlayer = static_cast<PlayerOccupy>(save["savedAIPlayer"].toInt());
    savedHumanPlayer = static_cast<PlayerOccupy>(save["savedHumanPlayer"].toInt());

    return 1;
}