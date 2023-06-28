#include "customgraphicsview.h"
#include "baseblock.h"
#include <QMenu>
#include <QIcon>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
   setRenderHints(QPainter::Antialiasing |
                  QPainter::SmoothPixmapTransform |
                  QPainter::HighQualityAntialiasing);

   setMouseTracking(true);

   setContextMenuPolicy(Qt::CustomContextMenu);
   connect(this,
           SIGNAL(customContextMenuRequested(const QPoint &)),
           this,
           SLOT(showContextMenu(const QPoint &)));
}

void CustomGraphicsView::showContextMenu(const QPoint &point)
{
    BaseBlock *block = static_cast<BaseBlock*>(itemAt(point));
    if( block != nullptr )
    {
        qDebug() << "CustomGraphicsView::showContextMenu() <Warning>: the next line may generate Seg Falut.";
        qDebug() << block->getName();
/*        QMenu contextMenu(tr("Context menu"), this);
        QIcon icon(":/icon7");

        QAction action1("Rename", this);
        action1.setIcon(icon);
        action1.setIconVisibleInMenu(true);
        QAction action2("Delete", this);
        contextMenu.setActiveAction(&action2);
        QAction action3("Properties", this);
        connect(&action1, SIGNAL(triggered()), this, SLOT(rename()));
        contextMenu.addAction(&action1);
        contextMenu.addAction(&action2);
        contextMenu.addAction(&action3);

        QMenu contextMenu1(tr("Block Options"), this);
        QAction action4("this", this);
        QAction action5("that", this);
        QAction action6("hello", this);
        action6.setCheckable(true);
        action6.setChecked(false);
        contextMenu1.addAction(&action4);
        contextMenu1.addAction(&action5);
        contextMenu1.addAction(&action6);

        contextMenu.insertMenu(&action3,&contextMenu1);
        contextMenu.insertSection(&action3,"section");
        contextMenu.exec(mapToGlobal(point));*/
        block->contextMenuExec(mapToGlobal(point));
    }
    else
        qDebug() << "ContextMenuRequested at:"
                 << point.x()
                 << " - "
                 << point.y();
}

/*
void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    update();
    QGraphicsView::mousePressEvent(event);
}
*/
void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug() << "CustomGraphicsView::mouseMoveEvent()" << event->pos();
    update();
    QGraphicsView::mouseMoveEvent(event);
}

