//
// Created by Decmo on 25-1-9.
//

#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H
#include <GameData.h>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QSlider>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
class MainMenuWidget : public QWidget {
    Q_OBJECT
public:
    explicit MainMenuWidget(QWidget *parent = nullptr);

signals:
    void start_game();
    void restart_game();
    void undo();
    void redo();
    void save();
    void load();
    void AI_depth_changed(int depth);
    void AI_time_changed(int time);
    void AI_simulations_changed(int simulations);

public slots:
    void handle_start_game();
    void handle_restart_game();
    void handle_undo();
    void handle_redo();
    void handle_save();
    void handle_load();

    void black_choice_button_checked();
    void white_choice_button_checked();

    void update_AI_depth(int depth);
    void update_AI_max_time(int time);
    void update_AI_max_simulations(int simulations);


private:
    QPushButton* start_button;
    QPushButton* restart_button;
    QPushButton* undo_button;
    QPushButton* redo_button;
    QPushButton* save_button;
    QPushButton* load_button;

    QRadioButton *black_button;
    QRadioButton *white_button;

    QSlider *AI_depth_controller;
    QLabel *AI_depth_label;

    QSlider *AI_time_controller;
    QLabel *AI_time_label;

    QSlider *AI_simulations_controller;
    QLabel *AI_simulations_label;

    QVBoxLayout * main_layout;
    QGridLayout * button_layout;
    QVBoxLayout * slider_layout;
    QGridLayout * BW_choice_layout;
    QButtonGroup * BW_choice_group;
};
#endif //MAINMENUWIDGET_H
