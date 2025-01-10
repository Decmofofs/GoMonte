//
// Created by Decmo on 25-1-9.
//
#include "BoardCellWidget.h"
#include <HelperFunctions.h>
BoardCellWidget::BoardCellWidget(QWidget *parent, int _x, int _y) : QWidget(parent), x(_x), y(_y) {
    state = PlayerOccupy::NONE;
    //setAttribute(Qt::WA_Hover, true);
    setCursor(Qt::PointingHandCursor);
}

void BoardCellWidget::set_state(const PlayerOccupy state) {
    this->state = state;
    update();
}

PlayerOccupy BoardCellWidget::get_state() const {
    return state;
}

void BoardCellWidget::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);


    if (state == PlayerOccupy::BLACK) {
        painter.setBrush(Qt::black);
        painter.drawEllipse(width()/4, height()/4, width()/2, height()/2);
    } else if (state == PlayerOccupy::WHITE) {
        painter.setBrush(Qt::white);
        painter.drawEllipse(width()/4, width()/4, width()/2, height()/2);
    }
}

// TODO
void BoardCellWidget::mousePressEvent(QMouseEvent* event) {
    if (state != PlayerOccupy::NONE) return;
    if (is_AI_thinking) return;

    emit clicked(x, y);
}


