//
// Created by Decmo on 25-1-9.
//

#ifndef BOARDCELLWIDGET_H
#define BOARDCELLWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <GomokuBoard.h>
#include <GameData.h>
class BoardCellWidget : public QWidget {
    Q_OBJECT
public:
    explicit BoardCellWidget(QWidget *parent = nullptr, int _x = 0, int _y = 0);

    void set_state(PlayerOccupy state);

    PlayerOccupy get_state() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    //void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void clicked(int x, int y);

private:
    PlayerOccupy state;
    int x, y;
};

#endif //BOARDCELLWIDGET_H
