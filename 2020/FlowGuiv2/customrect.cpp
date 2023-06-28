#include "customrect.h"
#include <QDebug>
#include <QtMath>
#include <QGraphicsScene>

CustomRect::CustomRect(QRectF boundRect)
    : boundRect(boundRect),
      mouseStartPos(-1,-1),
      clickPressed(false),
      itemDrag(false)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setAcceptDrops(true);
    setToolTip("This is an item");

    input.moveTo(5,15);
    input.lineTo(15,5);
    input.lineTo(5,-15);
//    path.clear();
    path.addRect(QRectF(5,5,90,90));
//    path.moveTo(50,50);
//    path.connectPath(input);
    input.translate(0,boundRect.height()/4.0);
    path.addPath(input);
    path = path.simplified();
    QTransform transf( +1 ,  0 , 0 ,
                        0 , +1 , 0 ,
                        0 ,  0 , 1 );
//    setTransformOriginPoint(-50,0);
//    setTransform(transf);
    path.addText(20,50,QFont("Times",10, QFont::Serif),"Block");


}

void CustomRect::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    QBrush brush(Qt::white);
    if( clickPressed )
        brush.setColor(Qt::blue);
    else
        brush.setColor(Qt::red);


/*    path.lineTo(100,50);
    path.lineTo(0,100);
    path.moveTo(0,0);
    path.lineTo(25,0);
    path.reserve(10);*/
//    path.lineTo(0,0);
//    path.closeSubpath();

//    path.addEllipse(boundRect);
    painter->fillPath(path,brush);
    QPen pen(QBrush(Qt::black),2);
    painter->setPen(pen);
    painter->drawPath(path);
//        first = false;
}

void CustomRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mouseStartPos = event->pos();
    clickPressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CustomRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    clickPressed = false;
    itemDrag = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void CustomRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
/*
    if( itemDrag == false )
    {
        if( (event->pos()-mouseStartPos).manhattanLength() > 10 )
            itemDrag = true;
        else
            return;
    }
    const double marginMoveFactor = 0.12;
    int xleft = int(double(boundRect.width()) * marginMoveFactor);
    int xright= int(double(boundRect.width()) * (1.0-marginMoveFactor));
    int yup   = int(double(boundRect.height()) * marginMoveFactor);
    int ydown = int(double(boundRect.height()) * (1.0-marginMoveFactor));
    QRectF moveRect;
    moveRect.setCoords(xleft,yup,xright,ydown);
    if( moveRect.contains(mouseStartPos) )
        QGraphicsItem::mouseMoveEvent(event);*/
}

void CustomRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverMoveEvent(event);
}

void CustomRect::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsItem::dragMoveEvent(event);
}

void CustomRect::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsItem::dragEnterEvent(event);
}

void CustomRect::focusInEvent(QFocusEvent *event)
{
    QGraphicsItem::focusInEvent(event);
}
