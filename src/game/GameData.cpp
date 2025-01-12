//
// Created by Decmo on 25-1-9.
//
#include "GameData.h"
bool MoveInfo::operator<(const MoveInfo x) const {
    if (this->x<x.x) return true;
    if (this->x>x.x) return false;
    return this->y < x.y;
}

QJsonObject MoveInfo::to_json() const {
    QJsonObject obj;
    obj["x"] = x;
    obj["y"] = y;
    obj["player"] = static_cast<int>(player);
    return obj;
}

MoveInfo MoveInfo::json_to_move_info(const QJsonObject & obj) {
    return {obj["x"].toInt(), obj["y"].toInt(), static_cast<PlayerOccupy>(obj["player"].toInt())};
}

