//
// Created by Decmo on 25-1-9.
//
#include "BoardCellWidget.h"

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
        painter.drawEllipse(0, 0, width(), height());
    } else if (state == PlayerOccupy::WHITE) {
        painter.setBrush(Qt::white);
        painter.drawEllipse(0, 0, width(), height());
    }
}

void BoardCellWidget::mousePressEvent(QMouseEvent* event) {
    emit clicked(x, y);
}


