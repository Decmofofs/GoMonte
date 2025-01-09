#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MainBoardWidget.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void set_board(MainBoardWidget * board);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::MainWindow *ui;
    MainBoardWidget * m_Board;
};
#endif // MAINWINDOW_H
