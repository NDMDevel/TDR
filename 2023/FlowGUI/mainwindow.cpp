#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FlowGUI/Utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //main menu
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exitApp()));

    //display mouse position on status bar
    connect(ui->view,
            SIGNAL(mouseMoved(QPoint,QPointF)),
            this,
            SLOT(displayMousePos(QPoint,QPointF)));
    //debug
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::exitApp()
{
    close();
}
void MainWindow::displayMousePos(QPoint pos, QPointF scenePos)
{
    QString msg;
    msg  = "pos: " + FlowGUI::pointToQString(pos);
    msg += " scene: " + FlowGUI::pointToQString(scenePos);
    ui->statusbar->showMessage(msg);
}
