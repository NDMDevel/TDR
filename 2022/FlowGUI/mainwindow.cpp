#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //main menu
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exitApp()));

    //debug
    connect(ui->apply,
            SIGNAL(clicked()),
            this,
            SLOT(drawTestLine()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exitApp()
{
    close();
}

void MainWindow::drawTestLine()
{

}
