#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowIcon(QIcon(":/resources/icons/GoMonte.png"));

    ui->setupUi(this);
    setWindowTitle("GoMonte v1.0");
}

MainWindow::~MainWindow()
{
    delete ui;
}
