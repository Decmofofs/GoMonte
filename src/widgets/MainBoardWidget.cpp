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
    painter.setPen(Qt::black);

    painter.drawRect(rect());

    int cell_size = std::min(width(), height()) / S;
    for (int i = 0; i <= S; i++) {
        painter.drawLine(0, i * cell_size, S * cell_size, i * cell_size);
        painter.drawLine(i * cell_size, 0, i * cell_size, S * cell_size);
    }
}

void MainBoardWidget::handle_cell_click(int x, int y) {
    emit cell_clicked(x, y);
}

