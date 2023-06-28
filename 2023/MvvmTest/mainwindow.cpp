#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "blockview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _vmBlock(_mBlock)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&_scene);

    //graphics view setupt
    ui->graphicsView->setOptimizationFlag(QGraphicsView::DontSavePainterState);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);


    BlockView *block = new BlockView(_vmBlock);
    _scene.addItem(block);

    connect(ui->spinBoxWidth,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(blockSizeChanged()));
    connect(ui->spinBoxHeigth,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(blockSizeChanged()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::blockSizeChanged()
{
    int w = ui->spinBoxWidth->value();
    int h = ui->spinBoxHeigth->value();
    _mBlock.size.set(QSize(w,h));
}

