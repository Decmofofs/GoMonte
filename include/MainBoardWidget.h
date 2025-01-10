//
// Created by Decmo on 25-1-9.
//

#ifndef MAINBOARD_H
#define MAINBOARD_H

#include <QGridLayout>
#include <BoardCellWidget.h>
#include <GomokuBoard.h>
#include <GameData.h>
class MainBoardWidget : public QWidget {
    Q_OBJECT
public:
    explicit MainBoardWidget(QWidget *parent = nullptr, int size = BOARD_SIZE);

signals:
    void cell_clicked(int x, int y);
    void send_to_search_agent(const GomokuBoard & board);

public slots:
    void handle_cell_click(int x, int y);
    void receive_best_move(MoveInfo move);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int S;
    QGridLayout* layout;
    std::vector<std::vector<BoardCellWidget*>> cells;
    GomokuBoard board;

    void draw_board(QPainter &painter);
};

#endif //MAINBOARD_H
