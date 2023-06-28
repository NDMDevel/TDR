#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->menuTest,SIGNAL(triggered(QAction*)),this,SLOT(menuClicked(QAction*)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(testAddBlock()));

    mainWindowRect = ui->graphicsView->visibleRegion().boundingRect();

    scene = new QGraphicsScene(/*0,0,500,500,*/this);
    ui->graphicsView->setScene(scene);
    uint32_t gridSize = 20;
    grid = new GridGraphicsItem(ui->graphicsView->rect(),gridSize);
    grid->setPen(Qt::red);
    scene->addItem(grid);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    if( mainWindowRect == ui->graphicsView->visibleRegion().boundingRect() )
        return;

    mainWindowRect = ui->graphicsView->visibleRegion().boundingRect();

    QRectF rect = ui->graphicsView->visibleRegion().boundingRect();
    rect.setWidth(rect.width()-20);
    rect.setHeight(rect.height()-20);
    scene->setSceneRect(rect);
    grid->updateBoundRect(scene->sceneRect());
    QMainWindow::paintEvent(e);

//    static int count = 0;
//    qDebug() << count++;
}

void MainWindow::testAddBlock()
{
    uint32_t gridSize = 20;

    block = new BaseBlock("Block Name","Block Type",gridSize);
    block->addConnector(BaseBlock::BlockFace::First,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger"));
    block->addConnector(BaseBlock::BlockFace::First,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger1"));
    block->addConnector(BaseBlock::BlockFace::Second,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger2"));
    block->addConnector(BaseBlock::BlockFace::Second,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger3"));
    block->addConnector(BaseBlock::BlockFace::Third,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger4"));
    block->addConnector(BaseBlock::BlockFace::Third,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger4"));
    block->addConnector(BaseBlock::BlockFace::Four,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger4"));
    block->addConnector(BaseBlock::BlockFace::Four,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Signal,
                                       "Trigger4"));
    block->addConnector(BaseBlock::BlockFace::Four,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger4"));
    block->addConnector(BaseBlock::BlockFace::Four,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Flow,
                                       "Trigger4"));
    block->addConnector(BaseBlock::BlockFace::Four,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Data,
                                       "Trigger4"));
    block->addConnector(BaseBlock::BlockFace::Third,
                        BlockConnector(BlockConnector::Direction::Input,
                                       BlockConnector::Type::Flow,
                                       "Trigger4"));
    block->updateShape();
    block->contextMenuAddAction("Trigger",this,SLOT(action()));

    //this order matters. First the grid, then the blocks
    scene->addItem(block);
}

void MainWindow::menuClicked(QAction *action)
{
    qDebug() << action->text();
    if( action->text() == "rotateClockWise"  )
        block->rotateClockWise();
    if( action->text() == "rotateCounterClockWise"  )
        block->rotateCounterClockWise();
    if( action->text() == "flipHorizontal"  )
        block->flipHorizontal();
    if( action->text() == "flipVertical"  )
        block->flipVertical();
    scene->update();
}

