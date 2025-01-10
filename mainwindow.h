#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MainBoardWidget.h>
#include <SearchAgent.h>

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
    //void set_search_agent(QThread * thread, SearchAgentWorker * worker);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::MainWindow *ui;
    MainBoardWidget * m_Board;
    QThread * m_SearchAgentThread;
    SearchAgentWorker * m_SearchAgentWorker;
};
#endif // MAINWINDOW_H
