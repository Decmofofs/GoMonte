//
// Created by Decmo on 25-1-9.
//
#include "MainBoardWidget.h"

MainBoardWidget::MainBoardWidget(QWidget *parent, int size) : QWidget(parent), S(size) {
    layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    cells.resize(size);
    for (int i = 0; i < size; i++) {
        cells[i].resize(size);
        for (int j = 0; j < size; j++) {
            cells[i][j] = new BoardCellWidget(this, i, j);
            layout->addWidget(cells[i][j], i, j);
            connect(cells[i][j], &BoardCellWidget::clicked, this, &MainBoardWidget::handle_cell_click);
        }
    }

    setLayout(layout);
}

void MainBoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    draw_board(painter);

}

void MainBoardWidget::draw_board(QPainter &painter) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor(253, 196, 6)));

    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(rect());

    pen.setWidth(1);
    painter.setPen(pen);
    int cell_size = std::min(width(), height()) / S;



    for (int i = 1; i <= S; i++) {
        painter.drawLine(cell_size/2, i * cell_size-cell_size/2, S * cell_size-cell_size/2, i * cell_size-cell_size/2);
        painter.drawLine(i * cell_size-cell_size/2, cell_size/2, i * cell_size-cell_size/2, S * cell_size-cell_size/2);
    }


    QVector<QPoint> dots = {
        {3, 3}, {3, 7}, {3, 11},
        {7, 3}, {7, 7}, {7, 11},
        {11, 3}, {11, 7}, {11, 11}
    };

    painter.setBrush(Qt::black);
    for (const QPoint &dot : dots) {
        int x = dot.x() * cell_size + cell_size / 2;
        int y = dot.y() * cell_size + cell_size / 2;
        painter.drawEllipse(QPoint(x, y), dot_radius, dot_radius);
    }
}

void MainBoardWidget::receive_instruction(int x, int y, PlayerOccupy state) {
    cells[x][y]->set_state(state);
}
// TODO
void MainBoardWidget::handle_cell_click(int x, int y) {
    if (is_AI_thinking) return;

    emit cell_clicked(x, y);
}


