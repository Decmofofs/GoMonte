//
// Created by Decmo on 25-1-9.
//
#include <HelperFunctions.h>

#include "GomokuBoard.h"

void switch_role() {
    cur_player = (cur_player == PlayerOccupy::BLACK) ? PlayerOccupy::WHITE : PlayerOccupy::BLACK;
}

PlayerOccupy get_switched_role(PlayerOccupy player) {
    return (player == PlayerOccupy::BLACK) ? PlayerOccupy::WHITE : PlayerOccupy::BLACK;
}