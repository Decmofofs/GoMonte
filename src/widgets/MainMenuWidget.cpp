//
// Created by Decmo on 25-1-9.
//
#include "MainMenuWidget.h"

MainMenuWidget::MainMenuWidget(QWidget * parent) : QWidget(parent) {

    start_button = new QPushButton("开始游戏", this);
    restart_button = new QPushButton("重置游戏", this);
    undo_button = new QPushButton("悔棋", this);
    redo_button = new QPushButton("撤销", this);
    save_button = new QPushButton("保存存档", this);
    load_button = new QPushButton("加载存档", this);

    BW_choice_group = new QButtonGroup(this);
    black_button = new QRadioButton("我想执先手", this);
    white_button = new QRadioButton("我想执后手", this);
    black_button->setChecked(true);
    radio_black_checked = 1;
    BW_choice_group->addButton(black_button, 0);
    BW_choice_group->addButton(white_button, 1);

    start_button->setMaximumSize(90,20);
    restart_button->setMaximumSize(90,20);
    undo_button->setMaximumSize(90,20);
    redo_button->setMaximumSize(90,20);
    save_button->setMaximumSize(90,20);
    load_button->setMaximumSize(90,20);



    AI_depth_controller = new QSlider(Qt::Horizontal, this);
    AI_depth_label = new QLabel("AI 最大模拟深度: \n"+QString::number(MAX_DEPTH), this);

    AI_time_controller = new QSlider(Qt::Horizontal, this);
    AI_time_label = new QLabel("AI 最长思考时间（秒）: \n"+QString::number(MAX_TIME/1000), this);

    AI_simulations_controller = new QSlider(Qt::Horizontal, this);
    AI_simulations_label = new QLabel("AI 最大模拟次数: \n"+QString::number(MAX_SIMULATION), this);

    AI_depth_controller->setRange(1,30);
    AI_time_controller->setRange(1,60);
    AI_simulations_controller->setRange(1,20);

    AI_depth_controller->setValue(MAX_DEPTH);
    AI_time_controller->setValue(MAX_TIME/1000);
    AI_simulations_controller->setValue(MAX_SIMULATION/3000);

    main_layout = new QVBoxLayout(this);

    button_layout = new QGridLayout();
    button_layout->addWidget(start_button, 0, 0);
    button_layout->addWidget(restart_button, 0, 1);
    button_layout->addWidget(undo_button, 1, 0);
    button_layout->addWidget(redo_button, 1, 1);
    button_layout->addWidget(save_button, 2, 0);
    button_layout->addWidget(load_button, 2, 1);

    BW_choice_layout = new QGridLayout();
    BW_choice_layout->addWidget(black_button, 0, 0);
    BW_choice_layout->addWidget(white_button, 1, 0);

    slider_layout = new QVBoxLayout();
    slider_layout->addWidget(AI_depth_label);
    slider_layout->addWidget(AI_depth_controller);
    slider_layout->addWidget(AI_time_label);
    slider_layout->addWidget(AI_time_controller);
    slider_layout->addWidget(AI_simulations_label);
    slider_layout->addWidget(AI_simulations_controller);

    main_layout->addLayout(button_layout);
    main_layout->addLayout(BW_choice_layout);
    main_layout->addLayout(slider_layout);

    connect(start_button, &QPushButton::clicked, this, &MainMenuWidget::handle_start_game);
    connect(restart_button, &QPushButton::clicked, this, &MainMenuWidget::handle_restart_game);
    connect(undo_button, &QPushButton::clicked, this, &MainMenuWidget::handle_undo);
    connect(redo_button, &QPushButton::clicked, this, &MainMenuWidget::handle_redo);
    connect(save_button, &QPushButton::clicked, this, &MainMenuWidget::handle_save);
    connect(load_button, &QPushButton::clicked, this, &MainMenuWidget::handle_load);

    connect(black_button, &QRadioButton::clicked, this, &MainMenuWidget::black_choice_button_checked);
    connect(white_button, &QRadioButton::clicked, this, &MainMenuWidget::white_choice_button_checked);

    connect(AI_depth_controller, &QSlider::valueChanged, this, &MainMenuWidget::update_AI_depth);
    connect(AI_time_controller, &QSlider::valueChanged, this, &MainMenuWidget::update_AI_max_time);
    connect(AI_simulations_controller, &QSlider::valueChanged, this, &MainMenuWidget::update_AI_max_simulations);


}

void MainMenuWidget::handle_start_game() {
    if (initialized) return;
    emit start_game();
}

void MainMenuWidget::handle_restart_game() {
    emit restart_game();
}

void MainMenuWidget::handle_undo() {
    emit undo();
}

void MainMenuWidget::handle_redo() {
    emit redo();
}

void MainMenuWidget::handle_save() {
    emit save();
}

void MainMenuWidget::handle_load() {
    emit load();
}

void MainMenuWidget::black_choice_button_checked() {

    radio_black_checked = 1;
}

void MainMenuWidget::white_choice_button_checked() {


    radio_black_checked = 0;
}

void MainMenuWidget::update_AI_depth(int depth) {
    AI_depth_label->setText("AI 最大模拟深度: \n" + QString::number(depth));
    MAX_DEPTH = depth;
    emit AI_depth_changed(depth);
}

void MainMenuWidget::update_AI_max_time(int time) {
    AI_time_label->setText("AI 最长思考时间（秒）: \n" + QString::number(time));
    MAX_TIME = time * 1000;
    emit AI_time_changed(time);
}

void MainMenuWidget::update_AI_max_simulations(int simulations) {
    AI_simulations_label->setText("AI 最大模拟次数: \n" + QString::number(simulations*3000));
    MAX_SIMULATION = simulations * 3000;
    emit AI_simulations_changed(simulations);
}

