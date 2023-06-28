#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsLineItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->translate(0,0);

    customRect = new CustomRect();
    scene->addItem(customRect);

    connect(ui->graphicsView,
            SIGNAL(StartLine(const QPoint&)),
            this,
            SLOT(StartLine(const QPoint&)));

    connect(ui->graphicsView,
            SIGNAL(UpdateEndLine(const QPoint&)),
            this,
            SLOT(UpdateEndLine(const QPoint&)));

    connect(ui->graphicsView,
            SIGNAL(MouseRelease(const QPoint&)),
            this,
            SLOT(MouseRelease(const QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete customRect;
    delete scene;
}

void MainWindow::StartLine(const QPoint &point)
{
    lineStart = true;
    startLinePos = ui->graphicsView->mapToScene(point);
    line = scene->addLine(QLineF(startLinePos.x(),
                                 startLinePos.y(),
                                 startLinePos.x(),
                                 startLinePos.y()));
}

void MainWindow::UpdateEndLine(const QPoint& point)
{
    if( lineStart == true )
    {
        QPointF pointf = ui->graphicsView->mapToScene(point);
        line->setLine(startLinePos.x(),
                      startLinePos.y(),
                      pointf.x(),
                      pointf.y());
    }
}

void MainWindow::MouseRelease(const QPoint &)
{
    line->setFlag(QGraphicsItem::ItemIsMovable);
    lineStart = false;
}
